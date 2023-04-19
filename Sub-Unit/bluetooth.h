#include <stdio.h>
#include <string.h>

#define BTBUFSIZE 81
char BTTXBUF[BTBUFSIZE];
char BTRXBUF[BTBUFSIZE];


unsigned int txcharindex = 0;
unsigned int rxcharindex = 0;


char* AT_COMMANDS[] = {
    "AT"            // 0 Test COmmand
    "AT+ADDR?",     // 1 Query module address
    "AT+BAUD?",     // 2 Query baud rate
    "AT+BAUD[]",    // 3 Set baud rate
    "AT+CLEAR",     // 4 Clear Last Connected device address
    "AT+CONNL",     // 5 Try connect to last succeeded device
    "AT+CON[]",     // 6 Try connect an address
    "AT+FILT?",     // 7 Query filter AT command parameter in AT+MODE value is 2
    "AT+FILT[]",    // 8 Set filter AT command parameter in AT+MODE value is 2
    "AT+HELP?",
    "AT+IMME?",
    "AT+IMME[]",
    "AT+MODE?",
    "AT+MODE[]",
    "AT+NOTI",
    "AT+NOTI[]",
    "AT+NAME?",
    "AT+NAME[]",
    "AT+PARI?",
    "AT+PARI[]",
    "AT+PIO1?",
    "AT+PIO1[]"
    "AT+PIO[]?"
    "AT+PIO[][]",
    "AT+PASS?"
    "AT+PIN[]",
    "AT+POWE?",
    "AT+POWE[]",
    "AT+PWRM?"
    "AT+PWRM[]",
    "AT+RENEW",
    "AT+RESET",
    "AT+ROLE?"
    "AT+ROLE[]",
    "AT+RSSI?",
    "AT+RADD",
    "AT+STOP?"
    "AT+STOP[]",
    "AT+START",
    "AT+SLEEP",
    "AT+SAVE?"
    "AT+SAVE[]",
    "AT+TCOM?",
    "AT+TCON[]"
    "AT+TYPE?"
    "AT+TYPE[]"
    "AT+VERR?"
    "AT+VERS?"
};

void resetTXBuffer(){
    unsigned int i = 0;
    for(;i<BTBUFSIZE;i++){
        BTTXBUF[i] = '\0';
    }
}
void resetRXBuffer(){
    unsigned int i = 0;
    for(;i<BTBUFSIZE;i++){
        BTRXBUF[i] = '\0';
    }
}
