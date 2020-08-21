#ifndef FOURDIGDISPLAY_H
#define FOURDIGDISPLAY_H

#include <avr/io.h>

#define DIG4_0 0xC0 //0
#define DIG4_1 0xF9 //1
#define DIG4_2 0xA4 //2
#define DIG4_3 0xB0 //3
#define DIG4_4 0x99 //4
#define DIG4_5 0x92 //5
#define DIG4_6 0x82 //6
#define DIG4_7 0xF8 //7
#define DIG4_8 0x80 //8
#define DIG4_9 0x90 //9
//#define DIG4_0 0x88; //A
//#define DIG4_0 0x83; //b
//#define DIG4_0 0xC6; //C
//#define DIG4_0 0xA1; //d
//#define DIG4_0 0x86; //E
//#define DIG4_0 0x8E; //F
//#define DIG4_0 0xC2; //G
//#define DIG4_0 0x89; //H
//#define DIG4_0 0xF9; //I
//#define DIG4_0 0xF1; //J
//#define DIG4_0 0xC3; //L
//#define DIG4_0 0xA9; //n
//#define DIG4_0 0xC0; //O
//#define DIG4_0= 0x8C; //P
//#define DIG4_0 0x98; //q
//#define DIG4_0 0x92; //S
//#define DIG4_0 0xC1; //U
//#define DIG4_0 0x91; //Y
//#define DIG4_0 0xFE; //hight -

#define DIG4_PORT PORTB
#define DIG4_DDR DDRB
#define DIG4_SCLK PB1
#define DIG4_RCLK PB4
#define DIG4_IO PB3

extern const uint8_t dig4_digtable[];

void dig4_init(void);
void dig4_send(uint8_t c);
void dig4_int(uint16_t n);
void dig4_clean(void);

#endif // FOURDIGDISPLAY_H
