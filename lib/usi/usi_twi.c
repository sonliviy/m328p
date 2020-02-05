/*
 * Based on Brian Starkey 2014 <stark3y@gmail.com>
 * https://github.com/usedbytes/usi_i2c_slave
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "usi_twi.h"

//clear all non start flags
#define USISR_INIT (1<<USI_START_COND_INT)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC)
//clear sif flag
#define USISR_FALSESTART 1<<USI_START_COND_INT
//clear oif flag and read/shift byte
#define USISR_BYTE (1<<USIOIF)|(1 << USIPF)|(1 << USIDC)|0
//clear oif flag and read/shift ack
#define USISR_ACK (1<<USIOIF)|(1 << USIPF)|(1 << USIDC)|0x0E


//init twowire mode, external clock source
#define USICR_INIT (1<<USISIE)|(1<<USIWM1)|(1<<USICS1)
//enable startint
#define USICR_START (1<<USISIE)|(1<<USIWM1)|(1<<USIWM0)|(1<<USICS1)
//enable overflowint + start
#define USICR_SLAREAD (1<<USIOIE)|(1<<USISIE)|(1<<USIWM1)|(1<<USICS1)

#define SDA_INPUT() cbi(DDR_USI,USI_SDA)
#define SDA_OUTPUT() sbi(DDR_USI,USI_SDA)

static volatile uint8_t TWSR = TW_NO_INFO;
static uint8_t sladdr;
static uint8_t *buffer;
static uint8_t index;
static uint8_t len;

void twi_slave_init(uint8_t addr, uint8_t * data, uint8_t size){
    sladdr = addr;
    buffer = data;
    len = size;

    PORT_USI |= (1 << PORT_USI_SCL)|(1 << PORT_USI_SDA);     // Set SCL SDA high
    DDR_USI |= (1 << PORT_USI_SCL);      // Set SCL as output
    SET_USI_TO_TWI_START_CONDITION_MODE()
}


ISR(USI_START_VECTOR)
{
    TWSR = USI_SLAVE_CHECK_ADDRESS;
    DDR_USI &= ~(1 << PORT_USI_SDA); // Set SDA as input
    while (((PIN_USI & (1 << USI_SCL))) && ((PIN_USI & (1 << USI_SDA)) == 0))
        ; // Wait for SCL to go low to ensure the "Start Condition" has completed.
          // If a Stop condition arises then leave the interrupt to prevent waiting forever.
    if (PIN_USI & (1 << USI_SCL)) {
        // Stop Condition (waiting for next Start Condition)
        USICR = (1 << USISIE) | (0 << USIOIE) | // Enable Start Condition Interrupt. Disable Overflow Interrupt.
                (1 << USIWM1) | (0 << USIWM0) | // Set USI in Two-wire mode. No USI Counter overflow prior
                                                // to first Start Condition (potentail failure)
                (1 << USICS1) | (0 << USICS0) | (0 << USICLK) | // Shift Register Clock Source = External, positive edge
                (0 << USITC);
    } else {
        // really Start Condition (Enable Overflow Interrupt)
        USICR = (1 << USISIE) | (1 << USIOIE)
                | // Enable Overflow and Start Condition Interrupt. (Keep StartCondInt to detect RESTART)
                (1 << USIWM1) | (1 << USIWM0) |                 // Set USI in Two-wire mode.
                (1 << USICS1) | (0 << USICS0) | (0 << USICLK) | // Shift Register Clock Source = External, positive edge
                (0 << USITC);
    }
    USISR = (1 << USI_START_COND_INT) | (1 << USIOIF) | (1 << USIPF) | (1 << USIDC) | // Clear flags
            (0x0 << USICNT0); // Set USI to sample 8 bits i.e. count 16 external pin toggles.
}

ISR(USI_OVERFLOW_VECTOR)
{
    unsigned char tmpRxHead;
    unsigned char tmpTxTail; // Temporary variables to store volatiles
    unsigned char tmpUSIDR;

    switch (TWSR) {
    // ---------- Address mode ----------
    // Check address and send ACK (and next USI_SLAVE_SEND_DATA) if OK, else reset USI.
    case USI_SLAVE_CHECK_ADDRESS:
        if ((USIDR == 0) || ((USIDR >> 1) == sladdr)) {
            if (USIDR & 0x01) {
                TWSR = USI_SLAVE_SEND_DATA;
            } else {
                TWSR = USI_SLAVE_REQUEST_DATA;
            }
            SET_USI_TO_SEND_ACK();
        } else {
            SET_USI_TO_TWI_START_CONDITION_MODE();
        }
        break;

    // ----- Master write data mode ------
    // Check reply and goto USI_SLAVE_SEND_DATA if OK, else reset USI.
    case USI_SLAVE_CHECK_REPLY_FROM_SEND_DATA:
        if (USIDR) // If NACK, the master does not want more data.
        {
            SET_USI_TO_TWI_START_CONDITION_MODE();
            return;
        }
    // From here we just drop straight into USI_SLAVE_SEND_DATA if the master sent an ACK

    // Copy data from buffer to USIDR and set USI to shift byte. Next USI_SLAVE_REQUEST_REPLY_FROM_SEND_DATA
    case USI_SLAVE_SEND_DATA:
        if(index < len){
            USIDR      = buffer[index];
        } else // If the buffer is empty then:
        {
            SET_USI_TO_TWI_START_CONDITION_MODE();
            return;
        }
        TWSR = USI_SLAVE_REQUEST_REPLY_FROM_SEND_DATA;
        SET_USI_TO_SEND_DATA();
        break;

    // Set USI to sample reply from master. Next USI_SLAVE_CHECK_REPLY_FROM_SEND_DATA
    case USI_SLAVE_REQUEST_REPLY_FROM_SEND_DATA:
        TWSR = USI_SLAVE_CHECK_REPLY_FROM_SEND_DATA;
        SET_USI_TO_READ_ACK();
        break;

    // ----- Master read data mode ------
    // Set USI to sample data from master. Next USI_SLAVE_GET_DATA_AND_SEND_ACK.
    case USI_SLAVE_REQUEST_DATA:
        TWSR = USI_SLAVE_GET_DATA_AND_SEND_ACK;
        SET_USI_TO_READ_DATA();
        break;

    // Copy data from USIDR and send ACK. Next USI_SLAVE_REQUEST_DATA
    case USI_SLAVE_GET_DATA_AND_SEND_ACK:
        // Put data into Buffer
        TWSR = USI_SLAVE_REQUEST_DATA;
        buffer[index]              = USIDR; // Not necessary, but prevents warnings
        if (index < len) {
            SET_USI_TO_SEND_ACK();
        } else // If the buffer is full then:
        {
            SET_USI_TO_SEND_NACK();
        }
        break;
    }
}
