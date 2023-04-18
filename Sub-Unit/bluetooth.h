#include <stdio.h>
#define BLUETOOTHBUFSIZE 37
char BLUETOOTHBUFFER[BLUETOOTHBUFSIZE];

unsigned int charindex = 0;

void resetBuffer(){
    unsigned int i = 0;
    for(;i<BLUETOOTHBUFSIZE;i++){
        BLUETOOTHBUFFER[i] = '\0';
    }
}
