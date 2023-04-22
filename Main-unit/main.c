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
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    ADCCTL0 |= ADCSHT_15 + ADCON;//Sample hold time 15, 1024 cycles
    ADCCTL1 = ADCSHP;//Sample and hold pulse, sends a pulse when ready
    ADCMCTL0 |= ADCINCH_5 + ADCSREF_7;//Voltage reference, ADCINCH_X= PIN 1.X, for temperature sensor
    ADCIE |= ADCIE0;//sends interrupt when conversion is completed
    ADCCTL0 |= ADCENC;//enable encoding for ADC

    PM5CTL0 &= ~LOCKLPM5;//sends GPIO pins to low impedance mode

    RTCMOD = (3600*32) - 1;//setting value for RTC to be an hour
    RTCCTL = RTCSS__XT1CLK | RTCSR | RTCPS__1024 | RTCIE;//setting clock source, *, pre-devider 1024, enable interrupt

    P2DIR |= BIT1+BIT0;//P2.1 = GREEN LED, P2.0= RED LED, P2.2= Voltage sensing pin
    P2OUT |= BIT1;
    P2IES |= BIT2;
    P2IFG &= ~BIT2;
    P2IE |= BIT2;
    //P1.2=VREF-, P1.0=VREF+, P1.5=Capture A5
    P1SEL0 |= BIT6 + BIT7;//UART Pins P1.6=RX, P1.7=TX
    UCA0CTLW0 |= UCSWRST;
    UCA0CTLW0 |= UCSSEL_2;
    UCA0BRW = 6; //baud rate
    UCA0MCTLW = UCOS16 + (13<<4) + (0x22<<8);
    UCA0CTLW0 &= ~UCSWRST;
    UCA0IE |= UCRXIE;//enable interrupts for the receiver

    __bis_SR_register(LPM4_bits + GIE);
}

#pragma vector=ADC_VECTOR
__interrupt void ADC12ISR(void)
{
    voltage = ADCMEM0;//in memory, reference to 3.3V , offset from [600mV - 3.3V]
    tempts[tempindex] = (voltage - 600) / 10.0;//offset convert to degrees, (Voltage-600mV)/(10mV/C)
    tempindex = (++tempindex) % TEMPBUFSIZE;//circular index for temperature
}

#pragma vector=PORT2_VECTOR
__interrupt void PORT2_ISR(void)
{
    if(P2IFG & BIT2){
        P2IES ^= BIT2;//toggles interrupt edge
        if (((P2IN>>2)& 1)==1){//verifies that power is present
            P2OUT &= BIT0;
            P2OUT |= BIT1;
            //toggles LEDs
        }
        else {
            P2OUT |= BIT0;
            P2OUT &= BIT1;
            //toggles LEDs
        }
        P2IFG &= ~BIT2;//clears P2.2 flag
    }
    else{
        P2IFG &= BIT2;//clears all flags except P2.2
    }

}


#pragma vector=RTC_VECTOR
__interrupt void RTC_ISR(void)
{

}

void raiseError(ERROR err)
{
    error = err;
    sprintf(BTTXBUF,"%s %s",LOT,ERROR_STRING[err]);
    UCA0IE |= UCTXIE;

}

#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(){
    switch(__even_in_range(UCA0IV,4)){//takes first 3 bits
    case 0: break; //irrelavent
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
