#include <stdio.h>
#include <string.h>

#define BTBUFSIZE 81
char BTTXBUF[BTBUFSIZE];
char BTRXBUF[BTBUFSIZE];


unsigned int txcharindex = 0;
unsigned int rxcharindex = 0;


char* AT_COMMANDS[] = {
    "AT",           // 0 Test Command
    "AT+ADDR?",     // 1 Query module address
    "AT+BAUD?",     // 2 Query baud rate
    "AT+BAUD[]",    // 3 Set baud rate
    "AT+CLEAR",     // 4 Clear Last Connected device address
    "AT+CONNL",     // 5 Try connect to last succeeded device
    "AT+CON[]",     // 6 Try connect an address
    "AT+FILT?",     // 7 Query filter AT command parameter in AT+MODE value is 2
    "AT+FILT[]",    // 8 Set filter AT command parameter in AT+MODE value is 2
    "AT+HELP?",     // 9 System Help Information
    "AT+IMME?",     // 10 Query Module work type
    "AT+IMME[]",    // 11 Set Module work type
    "AT+MODE?",     // 12 Query Module Work Mode
    "AT+MODE[]",    // 13 Set Module Work Mode
    "AT+NOTI",      // 14 Query Notify information
    "AT+NOTI[]",    // 15 Set Notify information
    "AT+NAME?",     // 16 Query Module name
    "AT+NAME[]",    // 17 Set Module name
    "AT+PARI?",     // 18 Query Parity bit
    "AT+PARI[]",    // 19 Set Parity bit
    "AT+PIO1?",     // 20 Query PIO1 output status (System LED)
    "AT+PIO1[]",    // 21 Set PIO1 output status (System LED)
    "AT+PIO[]?"     // 22 Query/Set PIO pins output high or low (Only this time, when module next power on, this value is not be used)
    "AT+PIO[][]",   // 23 Query/Set PIO pins output high or low (Only this time, when module next power on, this value is not be used)
    "AT+PASS?",     // 24 Query Pin Code
    "AT+PIN[]",     // 25 Set Pin Code
    "AT+POWE?",     // 26 Query Module Power
    "AT+POWE[]",    // 27 Set Module Power
    "AT+PWRM?",     // 28 Query Module sleep type
    "AT+PWRM[]",    // 29 Set Module sleep type
    "AT+RENEW",     // 30 Restore all setup value to factory setup
    "AT+RESET",     // 31 .Restart module
    "AT+ROLE?",     // 32 Query Master and Slaver Role
    "AT+ROLE[]",    // 33 Set Master and Slaver Role
    "AT+RSSI?",     // 34 Query RSSI Value
    "AT+RADD",      // 35 Query Last Connected Device Address
    "AT+STOP?"      // 36 Query Stop bit
    "AT+STOP[]",    // 37 Set Stop bit
    "AT+START",     // 38 Work immediately
    "AT+SLEEP",     // 39 Query Module into sleep mode
    "AT+SAVE?",     // 40 Query Module save connected address parameter
    "AT+SAVE[]",    // 41 Set Module save connected address parameter
    "AT+TCON?",     // 42 Query module connect remote device timeout value
    "AT+TCON[]",    // 43 Set module connect remote device timeout value
    "AT+TYPE?",     // 44 Query Module Bond Mode
    "AT+TYPE[]",    // 45 Set Module Bond Mode
    "AT+VERR?",     // 46 Query Software Version
    "AT+VERS?"      // 47 Query Software Version
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