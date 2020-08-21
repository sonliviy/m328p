#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "4digdisplay.h"
#include "utils.h"

void dig4_send(uint8_t c);

const uint8_t dig4_digtable[] PROGMEM = {
    0xC0, //0
    0xF9, //1
    0xA4, //2
    0xB0, //3
    0x99, //4
    0x92, //5
    0x82, //6
    0xF8, //7
    0x80, //8
    0x90 //9
};

uint8_t dig4_buffer[4] = {0xFF,0xFF,0xFF,0xFF};

void dig4_init(){
    sbi(DIG4_DDR, DIG4_IO);
    sbi(DIG4_DDR, DIG4_SCLK);
    sbi(DIG4_DDR, DIG4_RCLK);
    cbi(DIG4_PORT, DIG4_IO);
    cbi(DIG4_PORT, DIG4_SCLK);
    cbi(DIG4_PORT, DIG4_RCLK);
    //INIT TIMER0
//    TCCR0B = (1<<CS02)|(1<<CS00);
//    TIMSK = (1<<TOIE0);
}


void dig4_send(uint8_t c){
    for(uint8_t i=8; i>0; i--){
        if(c&0x80)
            sbi(DIG4_PORT,DIG4_IO);
        else
            cbi(DIG4_PORT, DIG4_IO);
        c <<= 1;
        cbi(DIG4_PORT, DIG4_SCLK);
        sbi(DIG4_PORT, DIG4_SCLK);
    }
}

void dig4_int(uint16_t n){
    if(n>9999){
        char * t = "ovf";
//        dig4_text(t);
        return;
    }
    uint8_t n1,n2,n3,n4;
    n1 = (uint8_t) n / 1000;
    n = n%1000;
    n2 = (uint8_t) n / 100;
    n = n%100;
    n3 = (uint8_t) n / 10;
    n4 = n%10;
    dig4_buffer[0] = pgm_read_byte(&(dig4_digtable[n4]));
    dig4_buffer[1] = pgm_read_byte(&(dig4_digtable[n3]));
    dig4_buffer[2] = pgm_read_byte(&(dig4_digtable[n2]));
    dig4_buffer[3] = pgm_read_byte(&(dig4_digtable[n1]));

//    n1 = (uint8_t) n % 10;
//    n2 = (uint8_t) ((n % 100)-n1)/10;
//    n3 = (uint8_t) ((n % 1000) - n2 - n1) / 100;
//    n4 = (uint8_t) ((n % 10000) - n3 - n2 - n1) / 1000;

}

void dig4_clean(void){
    dig4_send(0xFF);
    dig4_send(0x00);
}

ISR(TIMER0_OVF_vect){
    dig4_send(dig4_buffer[0]);
    dig4_send(0b0001);
    cbi(DIG4_PORT, DIG4_RCLK);
    sbi(DIG4_PORT, DIG4_RCLK);
    dig4_send(dig4_buffer[1]);
    dig4_send(0b0001);
    cbi(DIG4_PORT, DIG4_RCLK);
    sbi(DIG4_PORT, DIG4_RCLK);
    dig4_send(dig4_buffer[2]);
    dig4_send(0b0001);
    cbi(DIG4_PORT, DIG4_RCLK);
    sbi(DIG4_PORT, DIG4_RCLK);
    dig4_send(dig4_buffer[3]);
    dig4_send(0b0001);
    cbi(DIG4_PORT, DIG4_RCLK);
    sbi(DIG4_PORT, DIG4_RCLK);
}
