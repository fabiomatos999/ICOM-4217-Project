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

void raiseError(ERROR err);

unsigned char ownerAddr[12];
unsigned char subAddr[12];
const unsigned int expDateHours = 17520;
ERROR error = NONE;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer


//    PM5CTL0 &= ~LOCKLPM5;//sends GPIO pins to low impedance mode
    P2OUT &= ~(BIT1 + BIT0);
    P2DIR |= BIT1+BIT0;//P2.1 = GREEN LED, P2.0= RED LED, P2.2= Voltage sensing pin
    P2OUT |= BIT1;
    P1IES |= BIT3;
    P1REN |= BIT3;
    P1IE |= BIT3;

    P1IFG &= ~BIT3;
//    P1SEL0 |= BIT6 + BIT7;//UART Pins P1.6=RX, P1.7=TX
//    UCA0CTLW0 |= UCSWRST;
//    UCA0CTLW0 |= UCSSEL_2;
//    UCA0BR0 = 6; //baud rate
//    UCA0BR1 = 0x00;
//    UCA0MCTLW = UCOS16 + (13<<4) + (0x22<<8);
//    UCA0CTLW0 &= ~UCSWRST;
//    UCA0IE |= UCRXIE;//enable interrupts for the receiver
    P3SEL = BIT4 + BIT5;
    UCA0CTL1 |= UCSWRST;
    UCA0CTL1 |= UCSSEL_2;
    UCA0BR0 = 109;
    UCA0BR1 = 0x00;
    UCA0MCTL = UCBRS_2+UCBRF_0;
    UCA0CTL1 &= ~UCSWRST;
    UCA0IE |= UCTXIE + UCRXIE;
    UCA0TXBUF = 'A';
//    executeAT_Command("AT+IMME[]", "1", NULL);
//    executeAT_Command("AT+ROLE[]", "1", NULL);
//    ownerAddr=executeAT_Command("AT+ADDR?", NULL, NULL);
//    subAddr= executeAT_Command("AT+DISC?", NULL, NULL);
    __bis_SR_register(LPM3_bits | GIE);
}

#pragma vector=PORT2_VECTOR
__interrupt void PORT2_ISR(void)
{
    if(P2IFG & BIT3){
        P1IES ^= BIT3;//toggles interrupt edge
        if (((P1IN>>3)& 1)==1){//verifies that power is present
            P2OUT &= ~BIT0;
            P2OUT |= BIT1;
            //toggles LEDs
        }
        else {
            P2OUT &= ~BIT1;
            P2OUT |= BIT1;
            //toggles LEDs
        }
        P1IFG &= ~BIT3;//clears P2.2 flag
    }
    else{
        P1IFG &= BIT3;//clears all flags except P2.2
    }

}



#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(){
    switch(__even_in_range(UCA0IV,4)){//takes first 3 bits
    case 0: break; //Irrelevant
    case 2:
        if (rxcharindex < BTBUFSIZE && rxcharindex < strlen(BTRXBUF)){
            UCA0RXBUF = BTRXBUF[rxcharindex];//receiver buffer
            rxcharindex += 1;
        }
        break;
    case 4:
        if (txcharindex < BTBUFSIZE && txcharindex < strlen(BTTXBUF)){
            UCA0TXBUF = BTTXBUF[txcharindex];//transmitter buffer
            txcharindex += 1;
        }
        break;
    default: break;
    }
}
