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
    P4OUT &= ~(BIT1 + BIT0);
    P4DIR |= BIT1+BIT0;//P2.1 = GREEN LED, P2.0= RED LED, P2.2= Voltage sensing pin
    P1REN |= BIT3;
    P1IE |= BIT3;
    if (((P1IN>>3)& 1)==1){//verifies that power is present
                P4OUT &= ~BIT0;
                P4OUT |= BIT1;
                P1IES |= BIT3;
                //toggles LEDs
            }
            else {
                P4OUT &= ~BIT1;
                P4OUT |= BIT0;
                P1IES &= ~BIT3;

                //toggles LEDs
            }
    P1IFG &= ~BIT3;
    P1SEL0 |= BIT6 + BIT7;//UART Pins P1.6=RX, P1.7=TX
    UCA0CTLW0 |= UCSWRST;
    UCA0CTLW0 |= UCSSEL_2;
    UCA0BR0 = 6; //baud rate
    UCA0BR1 = 0x00;
    UCA0MCTLW = UCOS16 + (13<<4) + (0x22<<8);
    UCA0CTLW0 &= ~UCSWRST;
    UCA0IE |= UCRXIE;//enable interrupts for the receiver
    P3SEL = BIT4 + BIT5;
    UCA0CTL1 |= UCSWRST;
    UCA0CTL1 |= UCSSEL_2;
    UCA0BR0 = 109;
    UCA0BR1 = 0x00;
    UCA0MCTL = UCBRS_2+UCBRF_0;
    UCA0CTL1 &= ~UCSWRST;
    UCA0IE |= UCTXIE + UCRXIE;
    UCA0TXBUF = 'A';
    __enable_interrupt();
    __bis_SR_register(LPM3_bits | GIE);
    __no_operation();
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

            //toggles LEDs
        }
        else {
            P4OUT &= ~BIT1;
            P4OUT |= BIT0;
            P1IES &= ~BIT3;
            //toggles LEDs
        }
    break;
    default:
    break;
    }
    __bis_SR_register_on_exit(LPM3_bits);

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
