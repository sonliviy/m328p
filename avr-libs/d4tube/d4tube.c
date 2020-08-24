#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <util/atomic.h>
#include <stdlib.h>
#include <string.h>
#include "d4tube.h"
#include "uart.h"
#include "utils.h"

void d4tube_send(uint8_t c);

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

uint8_t dig4_buffer[4] = {0xC0,0xF9,0xA4,0xB0};

void d4tube_init(){
    sbi(DIG4_DDR, DIG4_IO);
    cbi(DIG4_PORT, DIG4_IO);
    sbi(DIG4_DDR, DIG4_SCLK);
    cbi(DIG4_PORT, DIG4_SCLK);
    sbi(DIG4_DDR, DIG4_RCLK);
    cbi(DIG4_PORT, DIG4_RCLK);
#ifdef DIG4_TIMER0_OCRA
    sbi(TCCR0A,WGM01);
    TCCR0B = 0b00000101;
    sbi(TIMSK0, OCIE0A);
    OCR0A = 20;
#endif
}

#ifdef DIG4_TIMER0_OCRA
ISR(TIMER0_COMPA_vect){
    d4tube_render();
}
#endif


void d4tube_send(uint8_t c){
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

void d4tube_i(uint16_t n){
    if(n>9999){
        n=9999;
    }
    uint16_t old = n;
    uint8_t d0,d1,d2,d3;
    d3 = n / 1000;
    n = n % 1000;
    d2 = n / 100;
    n = n % 100;
    d1 = n / 10;
    d0 = n % 10;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
        dig4_buffer[0] = pgm_read_byte(&(dig4_digtable[d0]));
        if(old > 9)
            dig4_buffer[1] = pgm_read_byte(&(dig4_digtable[d1]));
        else
            dig4_buffer[1] = 0xFF;
        if(old > 99)
            dig4_buffer[2] = pgm_read_byte(&(dig4_digtable[d2]));
        else
            dig4_buffer[2] = 0xFF;
        if(old>999)
            dig4_buffer[3] = pgm_read_byte(&(dig4_digtable[d3]));
        else
            dig4_buffer[3] = 0xFF;    dig4_buffer[0] = pgm_read_byte(&(dig4_digtable[d0]));
        if(old > 9)
            dig4_buffer[1] = pgm_read_byte(&(dig4_digtable[d1]));
        else
            dig4_buffer[1] = 0xFF;
        if(old > 99)
            dig4_buffer[2] = pgm_read_byte(&(dig4_digtable[d2]));
        else
            dig4_buffer[2] = 0xFF;
        if(old>999)
            dig4_buffer[3] = pgm_read_byte(&(dig4_digtable[d3]));
        else
            dig4_buffer[3] = 0xFF;
    }

}

void d4tube_f(float f, uint8_t ap){
    char tmp[25];
    dtostrf(f,0,ap,tmp);
    uart_puts(tmp);
    uart_putc('\n');
    size_t l = strlen(tmp);
    char * c = &tmp;
    uint8_t i = 4;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
        while(i  && *c){
            switch (*c){
            case '.':
                dig4_buffer[i] &= 0b01111111;
                break;
            case '-':
                dig4_buffer[i-1] = 0b10111111;
                i--;
                break;
            default:
                dig4_buffer[i-1] = pgm_read_byte(&(dig4_digtable[*c-48]));
                i--;
            }
            c++;
        }
    }
}


inline void d4tube_clean(void){
    d4tube_send(0xFF);
    d4tube_send(0x00);
    cbi(DIG4_PORT, DIG4_RCLK);
    sbi(DIG4_PORT, DIG4_RCLK);
}

inline void d4tube_render(void){
    d4tube_send(dig4_buffer[0]);
    d4tube_send(0b0001);
    cbi(DIG4_PORT, DIG4_RCLK);
    sbi(DIG4_PORT, DIG4_RCLK);
    d4tube_send(dig4_buffer[1]);
    d4tube_send(0b0010);
    cbi(DIG4_PORT, DIG4_RCLK);
    sbi(DIG4_PORT, DIG4_RCLK);
    d4tube_send(dig4_buffer[2]);
    d4tube_send(0b0100);
    cbi(DIG4_PORT, DIG4_RCLK);
    sbi(DIG4_PORT, DIG4_RCLK);
    d4tube_send(dig4_buffer[3]);
    d4tube_send(0b1000);
    cbi(DIG4_PORT, DIG4_RCLK);
    sbi(DIG4_PORT, DIG4_RCLK);
    d4tube_clean();
}
