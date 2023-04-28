#include <msp430.h> 
#include <temperatureSensor.h>
#include "stdio.h"

#define BUFFSIZE 40

void resetBuffer(char *buf)
{
    unsigned int i = 0;
    for (; i < 38; i++)
    {
        buf[i] = ' ';
    }
}

char at[] = "AT+ADDR?\r\n";

char BUFF[BUFFSIZE];

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
    ADC12CTL0 |= ADC12ENC;
//
//
    RTCCTL01 = RTCTEVIE + RTCSSEL_2 + RTCTEV_0; // Counter Mode, RTC1PS, 8-bit ovf
                                              // overflow interrupt enable
    RTCPS0CTL = RT0PSDIV_3;                   // ACLK, /8, start timer
    RTCPS1CTL = RT1SSEL_2 + RT1PSDIV_4;       // out from RT0PS, /16, start timer
//
    P1DIR |= BIT4;
    P1OUT &= ~BIT4;

    P3SEL = BIT4 + BIT5;                             // P3.4,5 = USCI_A0 TXD/RXD
    UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
    UCA0CTL1 |= UCSSEL_2;
    UCA0CTL0 |= UCMODE_3;// SMCLK
    UCA0BR0 = 109;                              // 1MHz 115200 (see User's Guide)
    UCA0BR1 = 0;                              // 1MHz 115200
    UCA0MCTL |= UCBRS_2 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    __enable_interrupt();

    while(1){
    }
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
            tempts[tempindex] = ((voltage-650.0)/10.0);
            tempindex = (++tempindex) % TEMPBUFSIZE;
        }
        checkExpDate();
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

        if (error == NONE)
        {
            P1OUT &= ~BIT4;
        }

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
    else {
        error = NONE;
    }
}

void raiseError(ERROR err)
{
    error = err;
    P1OUT |= BIT4;
    sprintf(BUFF ,"%s %s\r\n",LOT,ERROR_STRING[err]);
    UCA0IE |= UCTXIE;
}

// Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
  switch(__even_in_range(UCA0IV,4))
  {
  case 0:break;                             // Vector 0 - no interrupt
  case 2:                                   // Vector 2 - RXIFG

    break;
  case 4:
      if (error == NONE || i >= BUFFSIZE) {
          i = 0;
          resetBuffer(BUFF);
          UCA0IE &= ~UCTXIE;
      }
      else {
          UCA0TXBUF = BUFF[i];
          i++;
      }
      break;
  default: break;
  }
}

