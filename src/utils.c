#include "utils.h"
#include "errno.h"


int8_t atohex(const char * input, uint8_t * output, size_t maxsize){
    uint8_t i = 0;
    while(input[i]) i++;
//    uint8_t len = i / 2;
    if(i % 2 > 0) //size mismatch
        return 0;
    if(i/2 > maxsize) //overflow
        return 0;
    i=0;
    while(input[i]){
        uint8_t t = input[i++];
        if(t>64)
            t-=55;
        else
            t-=48;
        //TODO: CHECK INT
        *output = t<<4;
        t = input[i++];
        if(t>64)
           t-=55;
        else
           t-=48;
        *(output++) |= t;
    }
    return i/2;
}

void atohex_fromslice(const char * input_start, uint8_t size, uint8_t * output){
    for(uint8_t i=0;i<size*2;i++){
        uint8_t t = input_start[i++];
        if(t>64)
            t-=55;
        else
            t-=48;
        *output = t<<4;
        t = input_start[i];
        if(t>64)
           t-=55;
        else
           t-=48;
        *(output++) |= t;
    }
}

char * hextoa(const uint8_t * input_b, char * output_b){
        uint8_t t = *input_b >> 4;
        if(t<10)
            t+= 48;
        else
            t+= 55;
        *output_b++ = t;
        t = *input_b & 0x0F;
        if(t<10)
            t+= 48;
        else
            t+= 55;
        *output_b = t;
    return --output_b;
}

char * hextoa_bend16(const uint8_t * input_b, char * output_b){
        uint8_t t = input_b[1] >> 4;
        if(t<10)
            t+= 48;
        else
            t+= 55;
        output_b[0] = t;

        t = input_b[1] & 0x0F;
        if(t<10)
            t+= 48;
        else
            t+= 55;
        output_b[1] = t;

        t = input_b[0] >> 4;
        if(t<10)
            t+= 48;
        else
            t+= 55;
        output_b[2] = t;
        t = input_b[0] & 0x0F;
        if(t<10)
            t+= 48;
        else
            t+= 55;
        output_b[3] = t;
    return output_b;
}

char * hextoan(const uint8_t * input_b, char * output_b, uint8_t len){
    for(uint8_t i = 0; i<len;i++){
        hextoa(input_b+i,output_b+i*2);
    }
    output_b[len*2] = '\0';
    return output_b;
}
