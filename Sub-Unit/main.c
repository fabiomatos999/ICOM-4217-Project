#include <msp430.h> 
#include <temperatureSensor.h>
#include <string.h>
#define BTBUFSIZE 81

char BUFF[BTBUFSIZE];
char BTTXBUF[BTBUFSIZE];

unsigned int txcharindex = 0;
unsigned int startup = 0;

void resetTXBUFFER(){
    unsigned int i = 0;
    for(;i<BTBUFSIZE;i++){
        BTTXBUF[i] = '\0';
    }
}

void AT(){
    char AT[] = "AT\r\n";
    unsigned int i = 0;
    for(;i<strlen(AT);i++){
        while((UCA0STAT&UCBUSY));
        UCA0TXBUF= AT[i];
    }
}

void ATRESET(){
    char AT[] = "AT+RESET\r\n";
    unsigned int i = 0;
    for(;i<strlen(AT);i++){
        while((UCA0STAT&UCBUSY));
        UCA0TXBUF= AT[i];
    }
}

void ATNAME(){
    char AT[] = "AT+NAMEOWO\r\n";
    unsigned int i = 0;
    for(;i<strlen(AT);i++){
        while((UCA0STAT&UCBUSY));
        UCA0TXBUF= AT[i];
    }
}

void ATRENEW(){
    char AT[] = "AT+RENEW\r\n";
    unsigned int i = 0;
    for(;i<strlen(AT);i++){
        while((UCA0STAT&UCBUSY));
        UCA0TXBUF= AT[i];
    }
}

void ATCON(char ADDR[]){
    char AT[19] = "AT+CON";
    strcat(AT,ADDR);
    strcat(AT,"\r\n");

    unsigned int i = 0;
    for(;i<strlen(AT);i++){
        while((UCA0STAT&UCBUSY));
        UCA0TXBUF= AT[i];
    }
}

unsigned int i = 0;

typedef enum
{
    OLD, FROZEN, COLD, WARM, HOT, TRASH, NONE
} ERROR;

char*ERROR_STRING[7] = {
    "OLD   ",
    "FROZEN",
    "COLD  ",
    "WARM  ",
    "HOT   ",
    "TRASH ",
    "NONE  "
};

char LOT[30] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234";

void checkExpDate();
void raiseError(ERROR err);


unsigned int currHour = 0;
const unsigned int expDateHours = 17520;
ERROR error = NONE;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    resetTXBUFFER();

    P3SEL = BIT4 + BIT5;                             // P3.4,5 = USCI_A0 TXD/RXD
    UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
    UCA0CTL1 |= UCSSEL_2;
    UCA0CTL0 |= UCMODE_3;// SMCLK
    UCA0BR0 = 109;                              // 1MHz 115200 (see User's Guide)
    UCA0BR1 = 0;                              // 1MHz 115200
    UCA0MCTL |= UCBRS_2 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    ATRENEW();
    __delay_cycles(100000);
    ATRESET();
    __delay_cycles(100000);
    ATRENEW();
    __delay_cycles(100000);
    ATCON("845F042FF7F3");
    __delay_cycles(100000);
    startup = 1;
    UCA0IE |= UCTXIE + UCRXIE;



    P6SEL |= BIT7;
    ADC12CTL0 |= ADC12SHT0_15 + ADC12ON;
    ADC12CTL1 = ADC12SHP;
    ADC12MCTL0 |= ADC12INCH_7 + ADC12SREF_7;
    ADC12CTL0 |= ADC12ENC;

    RTCCTL01 = RTCTEVIE + RTCSSEL_2 + RTCTEV_0; // Counter Mode, RTC1PS, 8-bit ovf
                                              // overflow interrupt enable
    RTCPS0CTL = RT0PSDIV_3;                   // ACLK, /8, start timer
    RTCPS1CTL = RT1SSEL_2 + RT1PSDIV_4;       // out from RT0PS, /16, start timer

    P1DIR |= BIT4;
    P1OUT &= ~BIT4;

    __enable_interrupt();

    __bis_SR_register(LPM0_bits);
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
  __bis_SR_register_on_exit(LPM0_bits);

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
    txcharindex = 0;
    resetTXBUFFER();
    for(;txcharindex<30;txcharindex++){
        BTTXBUF[txcharindex] = LOT[txcharindex];
    }
    BTTXBUF[30] = ' ';
    txcharindex = 31;
    for(;txcharindex<38;txcharindex++){
        BTTXBUF[txcharindex] = ERROR_STRING[err][txcharindex-31];
      }
    BTTXBUF[37] = '\r';
    BTTXBUF[38] = '\n';
    BTTXBUF[39] = '\0';
    txcharindex=0;
    UCA0IFG |= BIT1;
}

// Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
  switch(__even_in_range(UCA0IV,4))
  {
  case 0:break;                             // Vector 0 - no interrupt
  case 2:
      UCA0RXBUF = UCA0RXBUF;// Vector 2 - RXIFG
    break;
  case 4:
      if ((error == NONE || txcharindex >= strlen(BTTXBUF) )&& startup == 1) {
          unsigned int x = 0;
          resetTXBUFFER();
          txcharindex = 0;
      }
      else {
          UCA0TXBUF = BTTXBUF[txcharindex];
          txcharindex++;
      }
      break;
  default: break;
  }
}

