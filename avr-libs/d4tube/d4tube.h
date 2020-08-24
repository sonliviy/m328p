/*
  Library for use 4LED display with 2 TM74HC595.
*/
#ifndef D4TUBE_H
#define D4TUBE_H

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

#define DIG4_PORT PORTD
#define DIG4_DDR DDRD
#define DIG4_SCLK PD7
#define DIG4_RCLK PD6
#define DIG4_IO PD5
#define DIG4_TIMER0_OCRA

extern const uint8_t dig4_digtable[];

void d4tube_init(void);
void d4tube_render(void);
void d4tube_clean();
void d4tube_i(uint16_t n);
void d4tube_f(float f, uint8_t ap);


#endif // D4TUBE_H
