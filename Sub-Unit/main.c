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

    ADCCTL0 |= ADCSHT_15 + ADCON;
    ADCCTL1 = ADCSHP;
    ADCMCTL0 |= ADCINCH_5 + ADCSREF_7;
    ADCIE |= ADCIE0;
    ADCCTL0 |= ADCENC;

    PM5CTL0 &= ~LOCKLPM5;

    RTCMOD = (3600*32) - 1;
    RTCCTL = RTCSS__XT1CLK | RTCSR | RTCPS__1024 | RTCIE;

    P2DIR |= BIT0;
    P2OUT &= ~BIT0;

    P1SEL0 |= BIT6 + BIT7;
    UCA0CTLW0 |= UCSWRST;
    UCA0CTLW0 |= UCSSEL_2;
    UCA0BRW= 6;
    UCA0MCTLW = UCOS16 + (13<<4) + (0x22<<8);
    UCA0CTLW0 &= ~UCSWRST;
    UCA0IE |= UCRXIE;

    __bis_SR_register(LPM4_bits + GIE);
}

#pragma vector=ADC_VECTOR
__interrupt void ADC12ISR(void)
{
    voltage = ADCMEM0;
    tempts[tempindex] = (voltage - 600) / 10.0;
    tempindex = (++tempindex) % TEMPBUFSIZE;
}

#pragma vector=RTC_VECTOR
__interrupt void RTC_ISR(void)
{
    currHour += 1;
    unsigned int i = 0;
    for (; i < TEMPBUFSIZE; i++)
    {
        while (ADCCTL1 & ADCBUSY)
            ;
        ADCCTL0 |= ADCSC;
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
        break;
    case 4:
        if (txcharindex < BTBUFSIZE && txcharindex < strlen(BTTXBUF)){
            UCA0TXBUF = BTTXBUF[txcharindex];
            txcharindex += 1;
        }
        break;
    default: break;
    }
}
