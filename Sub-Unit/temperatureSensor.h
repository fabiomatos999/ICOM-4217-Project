#define TEMPBUFSIZE 100

float tempts[TEMPBUFSIZE];
unsigned int tempindex = 0;
float voltage = 0;

float average_celsius(){
    float total= 0;
    unsigned int i = 0;
    for(;i<TEMPBUFSIZE;i++){
        total +=tempts[i];
    }
    return total/((float)TEMPBUFSIZE);
}
