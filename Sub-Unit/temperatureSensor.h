#define TEMPBUFSIZE 100

float tempts[TEMPBUFSIZE];
unsigned int tempindex = 0;
unsigned int voltage = 0;

float average_celsius(){
    float total= 0;
    unsigned int i = 0;
    for(;i<TEMPBUFSIZE;i++){
        total +=tempts[i];
    }
    return total/TEMPBUFSIZE;
}

