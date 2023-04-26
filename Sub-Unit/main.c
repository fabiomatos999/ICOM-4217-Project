#include <msp430.h> 
#include <temperatureSensor.h>
#include <bluetooth.h>

typedef enum
{
    OLD, FROZEN, COLD, WARM, HOT, TRASH, NONE
} ERROR;

char*ERROR_STRING[7] = {
    "OLD",
    "FROZEN",
    "COLD",
    "WARM",
    "HOT",
    "TRASH",
    "NONE"
};

char LOT[30] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234";

void checkExpDate();
void raiseError(ERROR err);


unsigned int currHour = 0;
const unsigned int expDateHours = 17520;
ERROR error = NONE;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

    P6SEL |= BIT7;
    ADC12CTL0 |= ADC12SHT0_15 + ADC12ON;
    ADC12CTL1 = ADC12SHP;
    ADC12MCTL0 |= ADC12INCH_7 + ADC12SREF_7;
    ADC12IE |= BIT0;
    ADC12CTL0 |= ADC12ENC;


    RTCCTL01 = RTCTEVIE + RTCSSEL_2 + RTCTEV_0; // Counter Mode, RTC1PS, 8-bit ovf
                                              // overflow interrupt enable
    RTCPS0CTL = RT0PSDIV_2;                   // ACLK, /8, start timer
    RTCPS1CTL = RT1SSEL_2 + RT1PSDIV_3;       // out from RT0PS, /16, start timer

    P2DIR |= BIT0;
    P2OUT &= ~BIT0;

    P3SEL = BIT4 + BIT5;
    UCA0CTL1 |= UCSWRST;
    UCA0CTL1 |= UCSSEL_2;
    UCA0BR0 = 109;
    UCA0BR1 = 0x00;
    UCA0MCTL = UCBRS_2+UCBRF_0;
    UCA0CTL1 &= ~UCSWRST;
    UCA0IE |= UCTXIE + UCRXIE;
    __enable_interrupt();

    executeAT_Command("AT",NULL,NULL);
    __bis_SR_register(LPM4_bits);
}

#pragma vector=RTC_VECTOR
__interrupt void RTC_ISR(void)
{
    currHour += 1;
    unsigned int i = 0;
    for (; i < TEMPBUFSIZE; i++)
    {
        ADC12CTL0 |= ADC12SC;
        while (ADC12CTL1 & ADC12BUSY);
        voltage = ADC12MEM0;
        tempts[tempindex] = (voltage) / 10.0;
        tempindex = (++tempindex) % TEMPBUFSIZE;
    }
    float average = average_celsius();
    if (average < 2 || average > 8)
    {
        if (average <= 0)
        {
            raiseError(FROZEN);
        }
        else if (average > 0 && average < 2)
        {
            raiseError(COLD);
        }
        else if (average > 8 && average < 15)
        {
            raiseError(WARM);
        }
        else if (average >= 15 && average < 30)
        {
            raiseError(WARM);
        }
        else
        {
            raiseError(TRASH);
        }
    }
    checkExpDate();
    if (error == NONE)
    {
        P2OUT &= ~BIT0;
    }

    __bic_SR_register_on_exit(LPM4_bits);   // Exit active CPU
}

void checkExpDate()
{
    if (currHour > expDateHours)
    {
        error = OLD;
    }
}

void raiseError(ERROR err)
{
    error = err;
    P2OUT |= BIT0;
    sprintf(BTTXBUF,"%s %s",LOT,ERROR_STRING[err]);
    UCA0IE |= UCTXIE;

}

#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(){
    switch(__even_in_range(UCA0IV,4)){
    case 0: break;
    case 2:
        if (rxcharindex < BTBUFSIZE && rxcharindex < strlen(BTRXBUF)){
            UCA0RXBUF = BTRXBUF[rxcharindex];
            rxcharindex += 1;
        }
        else {
            resetBuffer(BTRXBUF);
            rxcharindex = 0;
        }
        break;
    case 4:
        if (txcharindex < BTBUFSIZE && txcharindex < strlen(BTTXBUF)){
            UCA0TXBUF = BTTXBUF[txcharindex];
            txcharindex += 1;
        }
        else {
            txcharindex = 0;
            UCA0IE &= ~UCTXIE;
        }
        break;
    default: break;
    }
}




