/*
 * Based on Brian Starkey 2014 <stark3y@gmail.com>
 * https://github.com/usedbytes/usi_i2c_slave
 */

#ifndef USI_GPIO_H
#define USI_GPIO_H
/* Set these appropriately for your platform */
#if defined( __AVR_ATtiny25__ ) | \
     defined( __AVR_ATtiny45__ ) | \
     defined( __AVR_ATtiny85__ )
#  define DDR_USI             DDRB
#  define PORT_USI            PORTB
#  define PIN_USI             PINB
#  define PORT_USI_SDA        PB0
#  define PORT_USI_SCL        PB2
#  define USI_SDA         PINB0
#  define USI_SCL         PINB2
#  define USI_START_COND_INT  USISIF
#  define USI_START_VECTOR    USI_START_vect
#  define USI_OVERFLOW_VECTOR USI_OVF_vect
#endif

/* Set these appropriately for your application */
#define I2C_N_REG 2
#define I2C_SLAVE_ADDR 0x40

#endif // USI_GPIO_H
