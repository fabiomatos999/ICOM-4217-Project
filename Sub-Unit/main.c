#include <msp430.h> 
#include <temperatureSensor.h>

char at[] = "AT+ADDR?\n\r\0";

char BUFF[10];

unsigned int i = 0;

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
    RTCPS0CTL = RT0PSDIV_3;                   // ACLK, /8, start timer
    RTCPS1CTL = RT1SSEL_2 + RT1PSDIV_3;       // out from RT0PS, /16, start timer

    P1DIR |= BIT4;
    P1OUT &= ~BIT4;

    P3SEL = BIT4 + BIT5;
    UCA0CTL1 |= UCSWRST;
    UCA0CTL1 |= UCSSEL_2;
    UCA0BR0 = 109;
    UCA0BR1 = 0x00;
    UCA0MCTL = UCBRS_2+UCBRF_0;
    UCA0CTL1 &= ~UCSWRST;
    UCA0IE |= UCTXIE + UCRXIE;
    __enable_interrupt();

    __delay_cycles(10000);
    __bis_SR_register(LPM3_bits);
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=RTC_VECTOR
__interrupt void RTC_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(RTC_VECTOR))) RTC_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(RTCIV,16))
  {
    case 0: break;                          // No interrupts
    case 2: break;                          // RTCRDYIFG
    case 4:                                 // RTCEVIFG
        currHour += 1;
        unsigned int i = 0;
        for (; i < TEMPBUFSIZE; i++)
        {
            ADC12CTL0 |= ADC12SC;
            while (ADC12CTL1 & ADC12BUSY);
            voltage = ADC12MEM0;
            tempts[tempindex] = (voltage-500) / 10.0;
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
            P1OUT &= ~BIT4;
        }

        __bic_SR_register_on_exit(LPM3_bits);   // Exit active CPU
      break;
    case 6: break;                          // RTCAIFG
    case 8: break;                          // RT0PSIFG
    case 10: break;                         // RT1PSIFG
    case 12: break;                         // Reserved
    case 14: break;                         // Reserved
    case 16: break;                         // Reserved
    default: break;
  }
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
    P1OUT |= BIT4;
    sprintf(BTTXBUF,"%s %s",LOT,ERROR_STRING[err]);
    UCA0IE |= UCTXIE;

}

#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(){

    switch(__even_in_range(UCA0IV,4)){
    case 0: break;
    case 2:
        BUFF[i] = UCA0RXBUF;
        i = (++i)%10;
        volatile char buffcpy[10] = BUFF;
        break;
    case 4:
        UCA0TXBUF = 'A';
        break;
    default: break;
    }
}