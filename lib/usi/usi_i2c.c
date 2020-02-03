/*
 * Based on Brian Starkey 2014 <stark3y@gmail.com>
 * https://github.com/usedbytes/usi_i2c_slave
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "usi.h"
#include "utils.h"
#include "usi_i2c.h"
#include "util/twi.h"


static volatile uint8_t TWSR = TW_NO_INFO;
static uint8_t SLADDR;
static uint8_t *buffer;
static uint8_t index;
static uint8_t len;

//clear all non start flags
#define USISR_INIT (1<<USISIF)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC)
//clear sif flag
#define USISR_FALSESTART 1<<USISIF
//clear oif flag and read/shift byte
#define USISR_BYTE (1<<USIOIF)|0
//clear oif flag and read/shift ack
#define USISR_ACK (1<<USIOIF)|0x0E


//init twowire mode, external clock source
#define USICR_INIT (1<<USIWM1)|(1<<USICS1)
//enable startint
#define USICR_START (1<<USISIE)|USICR_INIT
//enable overflowint + start
#define USICR_SLAREAD (1<<USIOIE)|USICR_START

#define TW_SR_DATA_TO_ACK (TW_SR_DATA_ACK + 1)
#define TW_ST_DATA_ACK_WAIT (TW_ST_DATA_ACK + 1)
#define TW_ST_DATA_ACK_CHCK (TW_ST_DATA_ACK + 2)

#define SDA_OUTPUT() cbi(DDR_USI,PIN_USI_SDA)
#define SDA_INPUT() sbi(DDR_USI,PIN_USI_SDA)
#define NAK() USIDR = 0x80
#define ACK() USIDR = 0x00
#define IDLE_SDA() USIDR = 0xff


void i2c_slave_init(uint8_t addr, uint8_t * data, uint8_t size, void (*callback)(uint8_t)){
    SLADDR = addr;
}

void usi_set_idle(void){
    SDA_INPUT();
    USICR = USICR_START;
    USISR = USISR_INIT;
}

void usi_read_data(void){
    SDA_INPUT();//SDA as input
    USISR = USISR_BYTE; //clear all non start flags and store 8bit data
}

void usi_read_ack(void){
    SDA_INPUT();//SDA as input
    USIDR = 0;
    USISR = USISR_ACK; //clear all non start flags and store 1 bit data
}

void usi_send_data(void){
    SDA_OUTPUT();//SDA as output
    USISR = USISR_BYTE; //clear all non start flags and shift 8 bit data
}

void usi_send_ack(void){
    USIDR = 0;
    SDA_OUTPUT();//SDA as output
    USISR = USISR_ACK;
}

void usi_send_nack(void){
    SDA_INPUT();//SDA as output
    USISR = USISR_ACK;
}

ISR(USI_START_VECTOR)
{
    while (!rbi(PIN_USI,PIN_USI_SDA) && rbi(PIN_USI,PIN_USI_SCL));//Avaiting proceed start condition
    if(rbi(PIN_USI,PIN_USI_SDA)){
        USISR = USISR_FALSESTART;
    }
    else{
        if(rbi(USISR,USIPF)){
            TWSR = TW_REP_START;
        }
        else
            TWSR = TW_START;
        USICR = USICR_SLAREAD;
        USISR = USISR_INIT;//release scl - we are ready for 8 bit addr
    }

}

ISR(USI_OVERFLOW_VECTOR)
{
    static uint8_t mode;
    /* TODO: Check Writing USISR directly has side effects! */
    switch(TWSR){
    case TW_START:
        index = 0;
    case TW_REP_START:
        if((USIDR >> 1) == SLADDR){
            if(USIDR & 0x01)
                TWSR = TW_ST_SLA_ACK;
            else
                TWSR = TW_SR_SLA_ACK;
            usi_send_ack();
        }
        else{
            TWSR = TW_NO_INFO;
            usi_set_idle();
        }
        break;

        //Recv
    case TW_SR_SLA_ACK:
        mode = 1;
    case TW_SR_DATA_ACK:
        usi_read_data();
        TWSR = TW_SR_DATA_TO_ACK;
        break;
    case TW_SR_DATA_TO_ACK:
        if(mode){
            index = USIDR;
            mode = 0;
            TWSR = TW_SR_DATA_ACK;
            usi_send_ack();
            break;
        }

        buffer[index] = USIDR;
        if(index++ < len){
            TWSR = TW_SR_DATA_ACK;
            usi_send_ack();
        }
        else{
            TWSR = TW_SR_DATA_NACK;
            usi_send_nack();
        }
        break;
    case TW_SR_DATA_NACK:
        TWSR = TW_NO_INFO;
        usi_set_idle();
        break;

        //Send
    case TW_ST_DATA_ACK_CHCK:
        if(USIDR || (index > len)){//got NACK or out of data
            TWSR = TW_NO_INFO;
            usi_set_idle();
            break;
        }
    case TW_ST_SLA_ACK:
    case TW_ST_DATA_ACK:
        USIDR = buffer[index++];
        TWSR = TW_ST_DATA_ACK_WAIT;
        usi_send_data();
        break;
    case TW_ST_DATA_ACK_WAIT:
        TWSR = TW_ST_DATA_ACK_CHCK;
        usi_read_ack();
        break;
    }//switch
}//ISR
