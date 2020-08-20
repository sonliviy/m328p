#include <stdio.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "resolver_uart.h"
#include "utils.h"

char respbuffer[64];

void resolver_uart_init(uint16_t baud){
//For m328p
//    if(UCSR0B & (1<<RXEN0)) return;
    uart_init(baud);
//    sei();
    uart_puts_P("uart resolver inited\n");
}

uint8_t resolver_uart_checkout(void){
    uint8_t ch;
    while(uart_available()){
        if((ch = uart_getc()) == ':'){
            _delay_ms(3); //avaiting all packet 3ms for 32Bytes at 115200
            char inputbuffer[UART_RXn_BUFFER_SIZE];
            uint8_t i = 0;
            while((ch = uart_getc())){
                if((ch == '\r') | (ch == '\n')){
                    inputbuffer[i] = '\0';
                    break;
                }
                inputbuffer[i++] = ch;
                if(i == UART_RXn_BUFFER_SIZE)
                    return 0; //OVERFLOW
            }
            cmdbuffer[0] = inputbuffer[0];
            if(i = atohex(inputbuffer+1, cmdbuffer+1, CMDBUFFER_SIZE-1))
                return i;
            else
                return 0;
        }
    }
    return 0;
}

char * resolver_uart_execute(void){
    uint8_t index = cmdbuffer[1];
    if (index > REACTIONS_SIZE - 1)
        return NULL;
    response_t *r = reactions[index](*cmdbuffer, cmdbuffer+2);
    switch(r->type){
    case s8_t:
        sprintf(respbuffer, "%d\n", r->data.s8_t);
        break;
    case s16_t:
        sprintf(respbuffer, "%d\n", r->data.s16_t);
        break;
    case s32_t:
        sprintf(respbuffer, "%ld\n", r->data.s32_t);
        break;
    case u8_t:
        sprintf(respbuffer, "%u\n", r->data.u8_t);
        break;
    case u16_t:
        sprintf(respbuffer, "%u\n", r->data.u16_t);
        break;
    case u32_t:
        sprintf(respbuffer, "%ul\n", r->data.u32_t);
        break;
    case flt_t:
//        dtostrf(r->data.flt_t, 3, 4, respbuffer );
        sprintf(respbuffer, "F:%.2f\n", r->data.flt_t);
        break;
    }
    return respbuffer;
}
