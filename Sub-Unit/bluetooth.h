#include <stdio.h>
#include <string.h>

#define BTBUFSIZE 81
char BTTXBUF[BTBUFSIZE];
char BTRXBUF[BTBUFSIZE];

unsigned int txcharindex = 0;
unsigned int rxcharindex = 0;

char *AT_COMMANDS[] = {
        "AT\0",           // 0 Test Command
        "AT+ADDR?\0",     // 1 Query module address
        "AT+BAUD?\0",     // 2 Query baud rate
        "AT+BAUD[]\0",    // 3 Set baud rate
        "AT+CLEAR\0",     // 4 Clear Last Connected device address
        "AT+CONNL\0",     // 5 Try connect to last succeeded device
        "AT+CON[]\0",     // 6 Try connect an address
        "AT+FILT?\0",     // 7 Query filter AT command parameter in AT+MODE value is 2
        "AT+FILT[]\0",    // 8 Set filter AT command parameter in AT+MODE value is 2
        "AT+HELP?\0",     // 9 System Help Information
        "AT+IMME?\0",     // 10 Query Module work type
        "AT+IMME[]\0",    // 11 Set Module work type
        "AT+MODE?\0",     // 12 Query Module Work Mode
        "AT+MODE[]\0",    // 13 Set Module Work Mode
        "AT+NOTI\0",      // 14 Query Notify information
        "AT+NOTI[]\0",    // 15 Set Notify information
        "AT+NAME?\0",     // 16 Query Module name
        "AT+NAME[]\0",    // 17 Set Module name
        "AT+PARI?\0",     // 18 Query Parity bit
        "AT+PARI[]\0",    // 19 Set Parity bit
        "AT+PIO1?\0",     // 20 Query PIO1 output status (System LED)
        "AT+PIO1[]\0",    // 21 Set PIO1 output status (System LED)
        "AT+PIO[]?\0"     // 22 Query/Set PIO pins output high or low (Only this time, when module next power on, this value is not be used)
        "AT+PIO[][]\0",   // 23 Query/Set PIO pins output high or low (Only this time, when module next power on, this value is not be used)
        "AT+PASS?\0",     // 24 Query Pin Code
        "AT+PIN[]\0",     // 25 Set Pin Code
        "AT+POWE?\0",     // 26 Query Module Power
        "AT+POWE[]\0",    // 27 Set Module Power
        "AT+PWRM?\0",     // 28 Query Module sleep type
        "AT+PWRM[]\0",    // 29 Set Module sleep type
        "AT+RENEW\0",     // 30 Restore all setup value to factory setup
        "AT+RESET\0",     // 31 Restart module
        "AT+ROLE?\0",     // 32 Query Master and Slaver Role
        "AT+ROLE[]\0",    // 33 Set Master and Slaver Role
        "AT+RSSI?\0",     // 34 Query RSSI Value
        "AT+RADD\0",      // 35 Query Last Connected Device Address
        "AT+STOP?\0"      // 36 Query Stop bit
        "AT+STOP[]\0",    // 37 Set Stop bit
        "AT+START\0",     // 38 Work immediately
        "AT+SLEEP\0",     // 39 Query Module into sleep mode
        "AT+SAVE?\0",     // 40 Query Module save connected address parameter
        "AT+SAVE[]\0",    // 41 Set Module save connected address parameter
        "AT+TCON?\0",     // 42 Query module connect remote device timeout value
        "AT+TCON[]\0",    // 43 Set module connect remote device timeout value
        "AT+TYPE?\0",     // 44 Query Module Bond Mode
        "AT+TYPE[]\0",    // 45 Set Module Bond Mode
        "AT+VERR?\0",     // 46 Query Software Version
        "AT+VERS?\0"      // 47 Query Software Version
        };

void resetBuffer(char *buf)
{
    unsigned int i = 0;
    for (; i < BTBUFSIZE; i++)
    {
        buf[i] = '\0';
    }
}

int commandPosition(char *command)
{
    unsigned int i = 0;
    for (; i < 47; i++)
    {
        if (strcmp(AT_COMMANDS[i], command) == 0)
        {
            return i;
        }
    }
    return -1;
}

void printBufferNoArgs(char *buff, char *command)
{
    resetBuffer(buff);
    sprintf(buff, "%s", command);
}

void printBufferOneArg(char *buff, char *command, char *parm1)
{
    resetBuffer(buff);
    char commandCopy[BTBUFSIZE];
    strcpy(commandCopy, buff);
    unsigned int i = 0;
    for (; i < BTBUFSIZE; i++)
    {
        if (commandCopy[i] == '[' || commandCopy[i] == ']')
            commandCopy[i] = '\0';
    }
    sprintf(buff, "%s[%s]", command, parm1);
}

void printBufferTwoArg(char *buff, char *command, char *parm1, char *parm2)
{
    resetBuffer(buff);
    char commandCopy[BTBUFSIZE];
    strcpy(commandCopy, buff);
    unsigned int i = 0;
    for (; i < BTBUFSIZE; i++)
    {
        if (commandCopy[i] == '[' || commandCopy[i] == ']')
            commandCopy[i] = '\0';
    }
    sprintf(buff, "%s[%s][%s]", command, parm1, parm2);
}

void executeAT_Command(char *command, char *parm1, char *parm2)
{
    switch (commandPosition(command))
    {
    case 0:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IE |= UCRXIE;
        break;
    case 1:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IE |= UCRXIE;
        break;
    case 2:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IE |= UCRXIE;
        break;
    case 3:
        printBufferOneArg(BTTXBUF, command, parm1);
        UCA0IE |= UCRXIE;
        break;
    case 4:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IE |= UCRXIE;
        break;
    case 5:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IE |= UCRXIE;
        break;
    case 6:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IE |= UCRXIE;
        break;
    case 7:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IE |= UCRXIE;
        break;
    case 8:
        printBufferOneArg(BTTXBUF, command, parm1);
        UCA0IE |= UCRXIE;
        break;
    case 9:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IE |= UCRXIE;
        break;
    case 10:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IE |= UCRXIE;
        break;
    case 11:
        printBufferOneArg(BTTXBUF, command, parm1);
        UCA0IE |= UCRXIE;
        break;
    case 12:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IE |= UCRXIE;
        break;
    case 13:
        printBufferOneArg(BTTXBUF, command, parm1);
        UCA0IE |= UCRXIE;
        break;
    case 14:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IE |= UCRXIE;
        break;
    case 15:
        printBufferOneArg(BTTXBUF, command, parm1);
        UCA0IE |= UCRXIE;
        break;
    case 16:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IE |= UCRXIE;
        break;
    case 17:
        printBufferOneArg(BTTXBUF, command, parm1);
        UCA0IE |= UCRXIE;
        break;
    case 18:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IE |= UCRXIE;
        break;
    case 19:
        printBufferOneArg(BTTXBUF, command, parm1);
        UCA0IE |= UCRXIE;
        break;
    case 20:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IE |= UCRXIE;
        break;
    case 21:
        printBufferOneArg(BTTXBUF, command, parm1);
        UCA0IE |= UCRXIE;
        break;
    case 22:
        printBufferOneArg(BTTXBUF, command, parm1);
        UCA0IE |= UCRXIE;
        break;
    case 23:
        printBufferTwoArg(BTTXBUF, command, parm1,parm2);
        UCA0IE |= UCRXIE;
        break;
    case 24:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IE |= UCRXIE;
        break;
    case 25:
        printBufferOneArg(BTTXBUF, command, parm1);
        UCA0IE |= UCRXIE;
        break;
    case 26:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IE |= UCRXIE;
        break;
    case 27:
        printBufferOneArg(BTTXBUF, command, parm1);
        UCA0IE |= UCRXIE;
        break;
    case 28:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IE |= UCRXIE;
        break;
    case 29:
        printBufferOneArg(BTTXBUF, command, parm1);
        UCA0IE |= UCRXIE;
        break;
    case 30:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IE |= UCRXIE;
        break;
    case 31:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IE |= UCRXIE;
        break;
    case 32:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IE |= UCRXIE;
        break;
    case 33:
        printBufferOneArg(BTTXBUF, command, parm1);
        UCA0IE |= UCRXIE;
        break;
    case 34:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IE |= UCRXIE;
        break;
    case 35:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IE |= UCRXIE;
        break;
    case 36:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IE |= UCRXIE;
        break;
    case 37:
        printBufferOneArg(BTTXBUF, command, parm1);
        UCA0IE |= UCRXIE;
        break;
    case 38:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IE |= UCRXIE;
        break;
    case 39:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IE |= UCRXIE;
        break;
    case 40:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IE |= UCRXIE;
        break;
    case 41:
        printBufferOneArg(BTTXBUF, command, parm1);
        UCA0IE |= UCRXIE;
        break;
    case 42:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IE |= UCRXIE;
        break;
    case 43:
        printBufferOneArg(BTTXBUF, command, parm1);
        UCA0IE |= UCRXIE;
        break;
    case 44:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IE |= UCRXIE;
        break;
    case 45:
        printBufferOneArg(BTTXBUF, command, parm1);
        UCA0IE |= UCRXIE;
        break;
    case 46:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IE |= UCRXIE;
        break;
    case 47:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IE |= UCRXIE;
        break;
    default:
        break;
    }
}
