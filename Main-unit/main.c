#include <msp430.h> 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define BTBUFSIZE 81

char BTRXBUF[BTBUFSIZE];
char BTTXBUF[BTBUFSIZE];

unsigned int txcharindex = 0;
unsigned int rxcharindex = 0;
unsigned int startup = 0;

unsigned int index = 0;

unsigned int year = 0;
unsigned int month = 0;
unsigned int dom = 0;
unsigned int dow = 0;
unsigned int hour = 0;
unsigned int min = 0;
unsigned int sec = 0;

// C program to implement itoa()
#include <stdbool.h>
#include <stdio.h>

// A utility function to reverse a string
void reverse(char str[], int length)
{
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        end--;
        start++;
    }
}
// Implementation of citoa()
char* citoa(int num, char* str, int base)
{
    int i = 0;
    bool isNegative = false;

    /* Handle 0 explicitly, otherwise empty string is
     * printed for 0 */
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // In standard itoa(), negative numbers are handled
    // only with base 10. Otherwise numbers are
    // considered unsigned.
    if (num < 0 && base == 10) {
        isNegative = true;
        num = -num;
    }

    // Process individual digits
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';

    str[i] = '\0'; // Append string terminator

    // Reverse the string
    reverse(str, i);

    return str;
}


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

void ATSTART(){
    char AT[] = "AT+START\r\n";
    unsigned int i = 0;
    for(;i<strlen(AT);i++){
        while((UCA0STAT&UCBUSY));
        UCA0TXBUF= AT[i];
    }
}

void ATBAUD(){
    char AT[] = "AT+BAUD0\r\n";
    unsigned int i = 0;
    for(;i<strlen(AT);i++){
        while((UCA0STAT&UCBUSY));
        UCA0TXBUF= AT[i];
    }
}

void ATIMME(){
    char AT[] = "AT+IMME1\r\n";
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
    char AT[] = "AT+NAMEMAIN-UNIT\r\n";
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

bool power = false;


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer


//    PM5CTL0 &= ~LOCKLPM5;//sends GPIO pins to low impedance mode


    //Initialize UART
    P3SEL = BIT4 + BIT5;    //Select ports P3.4 and P3.5
    UCA0CTL1 |= UCSWRST;    //Enable reset
    UCA0CTL1 |= UCSSEL_2;
    UCA0CTL0 |= UCMODE_3;
    UCA0BR0 = 109;          //Baud Rate
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS_2+UCBRF_0;
    UCA0CTL1 &= ~UCSWRST;
    ATRENEW();
    __delay_cycles(100000);
    ATIMME();
    __delay_cycles(100000);
    ATBAUD();
    __delay_cycles(100000);
    ATNAME();
    __delay_cycles(100000);
    ATCON("5065839DE6E3");
    __delay_cycles(100000);
    ATSTART();
    __delay_cycles(100000);
    startup =1;
    UCA0IE |= UCTXIE + UCRXIE;

    P4OUT &= ~(BIT1 + BIT0);
    P4DIR |= BIT1+BIT0;
    P1REN |= BIT3;
    P1IE |= BIT3;

    if (((P1IN>>3)& 1)==1){//verifies that power is present
       P4OUT &= ~BIT0;
       P4OUT |= BIT1;
       P1IES |= BIT3;
       power = true;
       //toggles LEDs
    }
    else {
       P4OUT &= ~BIT1;
       P4OUT |= BIT0;
       P1IES &= ~BIT3;
       power = false;
       //toggles LEDs
    }
    P1IFG &= ~BIT3;

    RTCSEC = 0;
    RTCMIN = 30;
    RTCHOUR = 15;
    RTCDOW = 2;
    RTCDAY = 1;
    RTCMON = 5;
    RTCYEARL = 0x23;
    RTCYEARH = 0x21;

    RTCCTL01 = RTCTEVIE + RTCSSEL_2 + RTCTEV_0;
    RTCPS0CTL = RT0PSDIV_2;
    RTCPS1CTL = RT1SSEL_2 + RT1PSDIV_3;

    __enable_interrupt();
    while(1);

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
    case 2:
        break;                          // RTCRDYIFG
    case 4:

        year = (RTCYEARH-1) + RTCYEARL;
        month = RTCMON;
        dom = RTCDAY;
        dow = RTCDOW;
        hour = RTCHOUR;
        min = RTCMIN;
        sec = RTCSEC;
        resetTXBUFFER();
        BTTXBUF[0] = power + 48;
        UCA0IE |= UCTXIE;
        UCA0IFG |= UCTXIFG;
      break;
    case 6:
        break;                          // RTCAIFG
    case 8: break;                          // RT0PSIFG
    case 10: break;                         // RT1PSIFG
    case 12: break;                         // Reserved
    case 14: break;                         // Reserved
    case 16: break;                         // Reserved
    default: break;
  }
  __bis_SR_register_on_exit(LPM0_bits);

}

#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
    switch(__even_in_range(P1IV,16)){
    case 8:
        if (((P1IN>>3)& 1)==1){//verifies that power is present
            P4OUT &= ~BIT0;
            P4OUT |= BIT1;
            P1IES |= BIT3;
            power = true;
            //toggles LEDs
        }
        else {
            P4OUT &= ~BIT1;
            P4OUT |= BIT0;
            P1IES &= ~BIT3;
            power = false;
            //toggles LEDs
        }
    break;
    default:
    break;
    }
    __bis_SR_register_on_exit(LPM0_bits);
}



#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(){
    switch(__even_in_range(UCA0IV,4)){//takes first 3 bits
    case 0: break; //Irrelevant
    case 2:
        break;
    case 4:
        if (txcharindex >= strlen(BTTXBUF) && startup == 1) {
                  resetTXBUFFER();
                  txcharindex = 0;
                  UCA0IE &= ~UCTXIE;
              }
              else {
                  UCA0TXBUF = BTTXBUF[txcharindex];
                  txcharindex++;
              }
        break;
    default: break;
    }
}
