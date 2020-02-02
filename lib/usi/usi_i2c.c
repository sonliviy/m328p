/*
 * Based on Brian Starkey 2014 <stark3y@gmail.com>
 * https://github.com/usedbytes/usi_i2c_slave
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "usi.h"
#include "usi_i2c.h"
#include "util/twi.h"

static volatile uint8_t TWSR = TW_NO_INFO;
static uint8_t SLADDR;
static uint8_t *buffer;
static uint8_t index;
static uint8_t len;

#define TW_SR_DATA_TO_ACK (TW_SR_DATA_ACK + 1)
#define TW_ST_DATA_TO_ACK (TW_ST_DATA_ACK + 1)

#define I2C_SDA_DIR_OUT 1
#define I2C_SDA_DIR_IN 0

#define NAK() USIDR = 0x80
#define ACK() USIDR = 0x00
#define IDLE_SDA() USIDR = 0xff


void i2c_slave_init(uint8_t addr, uint8_t * data, uint8_t size, void (*callback)(uint8_t)){
    SLADDR = addr;
}

void usi_set_idle(void){
    USICR = (1<<USISIE)|(1<<USIWM1)|(1<<USICS1);
    USISR = (1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|0;
}

void usi_read_data(void){
    DDR_USI &= ~(1<<PIN_USI_SDA);
    USISR = (1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|0;
}

void usi_read_ack(void){
    DDR_USI &= ~(1<<PIN_USI_SDA);
    USIDR = 0;
    USISR = (1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|0x0E;
}

void usi_send_data(void){
    DDR_USI |= 1<<PIN_USI_SDA;
    USISR = (1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|0;
}

void usi_send_ack(void){
    USIDR = 0;
    DDR_USI |= 1 << PIN_USI_SDA;
    USISR = (1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|0x0E;
}

void usi_send_nack(void){
    USIDR = 1;
    DDR_USI |= 1 << PIN_USI_SDA;
    USISR = (1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|0x0E;
}

ISR(USI_START_VECTOR)
{
    DDR_USI &= ~_BV(PIN_USI_SDA);
    while (!(PIN_USI & _BV(PIN_USI_SDA)) && (PIN_USI & _BV(PIN_USI_SCL)));
//    stop condition
    if(PIN_USI & _BV(PIN_USI_SDA)){
        USICR = (1<<USISIE)|(1<<USIOIE)|(3<<USIWM0)|(1<<USICS1); //Avaiting start condition
    }
    else{
        if(USISR & (1<<USIPF)){
            TWSR = TW_REP_START;
        }
        else
            TWSR = TW_START;
    }
    USISR |= (1<<USISIF)|(1<<USIPF)|(1<<USIOIF)|(1<<USIDC)|0x0;//release scl - we are ready for 8 bit addr
}

ISR(USI_OVERFLOW_VECTOR)
{
        static uint8_t post_ack = 0;
        /* Writing USISR directly has side effects! */
        /* start flag, overflow flag, datacollision flag*/
        uint8_t usisr_tmp = 0xD0;
        uint8_t tmp;
        switch(TWSR){
        case TW_START:
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
                return;
            }
            break;

            //Recv
        case TW_SR_SLA_ACK:
        case TW_SR_DATA_ACK:
            TWSR = TW_SR_DATA_TO_ACK;
            usi_read_data();
            break;
        case TW_SR_DATA_TO_ACK:
            buffer[index] = USIDR;
            if(index < len - 1){
                usi_send_ack();
                TWSR = TW_SR_DATA_ACK;
            }
            else{
                usi_send_nack();
                TWSR = TW_SR_DATA_NACK;
            }
            break;
        case TW_SR_DATA_NACK:
            TWSR = TW_NO_INFO;
            usi_set_idle();
            break;

            //Send
        case TW_ST_SLA_ACK:
            USIDR = buffer[index++];
            TW_ST_DATA_ACK;



        }

}
