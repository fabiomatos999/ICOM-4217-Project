#include <stdio.h>

#define BTBUFSIZE 81
char BTTXBUF[BTBUFSIZE];
char BTRXBUF[BTBUFSIZE];

unsigned int txcharindex = 0;
unsigned int rxcharindex = 0;

char *AT_COMMANDS[] = {
        "AT\r\n",           // 0 Test Command
        "AT+ADDR?\r\n",     // 1 Query module address
        "AT+BAUD?\r\n",     // 2 Query baud rate
        "AT+BAUD[]\r\n",    // 3 Set baud rate
        "AT+CLEAR\r\n",     // 4 Clear Last Connected device address
        "AT+CONNL\r\n",     // 5 Try connect to last succeeded device
        "AT+CON[]\r\n",     // 6 Try connect an address
        "AT+FILT?\r\n",     // 7 Query filter AT command parameter in AT+MODE value is 2
        "AT+FILT[]\r\n",    // 8 Set filter AT command parameter in AT+MODE value is 2
        "AT+HELP?\r\n",     // 9 System Help Information
        "AT+IMME?\r\n",     // 10 Query Module work type
        "AT+IMME[]\r\n",    // 11 Set Module work type
        "AT+MODE?\r\n",     // 12 Query Module Work Mode
        "AT+MODE[]\r\n",    // 13 Set Module Work Mode
        "AT+NOTI\r\n",      // 14 Query Notify information
        "AT+NOTI[]\r\n",    // 15 Set Notify information
        "AT+NAME?\r\n",     // 16 Query Module name
        "AT+NAME[]\r\n",    // 17 Set Module name
        "AT+PARI?\r\n",     // 18 Query Parity bit
        "AT+PARI[]\r\n",    // 19 Set Parity bit
        "AT+PIO1?\r\n",     // 20 Query PIO1 output status (System LED)
        "AT+PIO1[]\r\n",    // 21 Set PIO1 output status (System LED)
        "AT+PIO[]?\r\n"     // 22 Query/Set PIO pins output high or low (Only this time, when module next power on, this value is not be used)
        "AT+PIO[][]\r\n",   // 23 Query/Set PIO pins output high or low (Only this time, when module next power on, this value is not be used)
        "AT+PASS?\r\n",     // 24 Query Pin Code
        "AT+PIN[]\r\n",     // 25 Set Pin Code
        "AT+POWE?\r\n",     // 26 Query Module Power
        "AT+POWE[]\r\n",    // 27 Set Module Power
        "AT+PWRM?\r\n",     // 28 Query Module sleep type
        "AT+PWRM[]\r\n",    // 29 Set Module sleep type
        "AT+RENEW\r\n",     // 30 Restore all setup value to factory setup
        "AT+RESET\r\n",     // 31 Restart module
        "AT+ROLE?\r\n",     // 32 Query Master and Slaver Role
        "AT+ROLE[]\r\n",    // 33 Set Master and Slaver Role
        "AT+RSSI?\r\n",     // 34 Query RSSI Value
        "AT+RADD\r\n",      // 35 Query Last Connected Device Address
        "AT+STOP?\r\n"      // 36 Query Stop bit
        "AT+STOP[]\r\n",    // 37 Set Stop bit
        "AT+START\r\n",     // 38 Work immediately
        "AT+SLEEP\r\n",     // 39 Query Module into sleep mode
        "AT+SAVE?\r\n",     // 40 Query Module save connected address parameter
        "AT+SAVE[]\r\n",    // 41 Set Module save connected address parameter
        "AT+TCON?\r\n",     // 42 Query module connect remote device timeout value
        "AT+TCON[]\r\n",    // 43 Set module connect remote device timeout value
        "AT+TYPE?\r\n",     // 44 Query Module Bond Mode
        "AT+TYPE[]\r\n",    // 45 Set Module Bond Mode
        "AT+VERR?\r\n",     // 46 Query Software Version
        "AT+VERS?\r\n"      // 47 Query Software Version
        };

void resetTXBuffer()
{
    unsigned int i = 0;
    for (; i < BTBUFSIZE; i++)
    {
        BTTXBUF[i] = '\0';
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
    resetTXBuffer();
    sprintf(buff, "%s", command);
}

void printBufferOneArg(char *buff, char *command, char *parm1)
{
    resetTXBuffer();
    char commandCopy[BTBUFSIZE];
    strcpy(commandCopy, buff);
    unsigned int i = 0;
    for (; i < BTBUFSIZE; i++)
    {
        if (commandCopy[i] == '[' || commandCopy[i] == ']')
            commandCopy[i] = '\0';
    }
    sprintf(buff, "%s%s\r\n", command, parm1);
}

void printBufferTwoArg(char *buff, char *command, char *parm1, char *parm2)
{
    resetTXBuffer();
    char commandCopy[BTBUFSIZE];
    strcpy(commandCopy, buff);
    unsigned int i = 0;
    for (; i < BTBUFSIZE; i++)
    {
        if (commandCopy[i] == '[' || commandCopy[i] == ']')
            commandCopy[i] = '\0';
    }
    sprintf(buff, "%s%s%s\r\n", command, parm1, parm2);
}

void executeAT_Command(char *command, char *parm1, char *parm2)
{
    switch (commandPosition(command))
    {
    case 0:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IFG |= BIT1;
        break;
    case 1:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IFG |= BIT1;
        break;
    case 2:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IFG |= BIT1;
        break;
    case 3:
        printBufferOneArg(BTTXBUF, command, parm1);
        UCA0IFG |= BIT1;
        break;
    case 4:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IFG |= BIT1;
        break;
    case 5:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IFG |= BIT1;
        break;
    case 6:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IFG |= BIT1;
        break;
    case 7:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IFG |= BIT1;
        break;
    case 8:
        printBufferOneArg(BTTXBUF, command, parm1);
        UCA0IFG |= BIT1;
        break;
    case 9:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IFG |= BIT1;
        break;
    case 10:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IFG |= BIT1;
        break;
    case 11:
        printBufferOneArg(BTTXBUF, command, parm1);
        UCA0IFG |= BIT1;
        break;
    case 12:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IFG |= BIT1;
        break;
    case 13:
        printBufferOneArg(BTTXBUF, command, parm1);
        UCA0IFG |= BIT1;
        break;
    case 14:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IFG |= BIT1;
        break;
    case 15:
        printBufferOneArg(BTTXBUF, command, parm1);
        UCA0IFG |= BIT1;
        break;
    case 16:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IFG |= BIT1;
        break;
    case 17:
        printBufferOneArg(BTTXBUF, command, parm1);
        UCA0IFG |= BIT1;
        break;
    case 18:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IFG |= BIT1;
        break;
    case 19:
        printBufferOneArg(BTTXBUF, command, parm1);
        UCA0IFG |= BIT1;
        break;
    case 20:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IFG |= BIT1;
        break;
    case 21:
        printBufferOneArg(BTTXBUF, command, parm1);
        UCA0IFG |= BIT1;
        break;
    case 22:
        printBufferOneArg(BTTXBUF, command, parm1);
        UCA0IFG |= BIT1;
        break;
    case 23:
        printBufferTwoArg(BTTXBUF, command, parm1,parm2);
        UCA0IFG |= BIT1;
        break;
    case 24:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IFG |= BIT1;
        break;
    case 25:
        printBufferOneArg(BTTXBUF, command, parm1);
        UCA0IFG |= BIT1;
        break;
    case 26:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IFG |= BIT1;
        break;
    case 27:
        printBufferOneArg(BTTXBUF, command, parm1);
        UCA0IFG |= BIT1;
        break;
    case 28:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IFG |= BIT1;
        break;
    case 29:
        printBufferOneArg(BTTXBUF, command, parm1);
        UCA0IFG |= BIT1;
        break;
    case 30:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IFG |= BIT1;
        break;
    case 31:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IFG |= BIT1;
        break;
    case 32:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IFG |= BIT1;
        break;
    case 33:
        printBufferOneArg(BTTXBUF, command, parm1);
        UCA0IFG |= BIT1;
        break;
    case 34:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IFG |= BIT1;
        break;
    case 35:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IFG |= BIT1;
        break;
    case 36:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IFG |= BIT1;
        break;
    case 37:
        printBufferOneArg(BTTXBUF, command, parm1);
        UCA0IFG |= BIT1;
        break;
    case 38:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IFG |= BIT1;
        break;
    case 39:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IFG |= BIT1;
        break;
    case 40:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IFG |= BIT1;
        break;
    case 41:
        printBufferOneArg(BTTXBUF, command, parm1);
        UCA0IFG |= BIT1;
        break;
    case 42:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IFG |= BIT1;
        break;
    case 43:
        printBufferOneArg(BTTXBUF, command, parm1);
        UCA0IFG |= BIT1;
        break;
    case 44:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IFG |= BIT1;
        break;
    case 45:
        printBufferOneArg(BTTXBUF, command, parm1);
        UCA0IFG |= BIT1;
        break;
    case 46:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IFG |= BIT1;
        break;
    case 47:
        printBufferNoArgs(BTTXBUF, command);
        UCA0IFG |= BIT1;
        break;
    default:
        break;
    }
}
