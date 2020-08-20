#ifndef I2C_H
#define I2C_H
#include <avr/io.h>

#define TWCR_CMD_INIT (1<<TWEN)
#define TWCR_CMD_START (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWSTA)
#define TWCR_CMD_CONTINUE (1<<TWEN)|(1<<TWIE)|(1<<TWINT)
#define TWCR_CMD_ACK (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA)
#define TWCR_CMD_NACK (1<<TWEN)|(1<<TWIE)|(1<<TWINT)
#define TWCR_CMD_STOP (1<<TWEN)|(1<<TWINT)|(1<<TWSTO)
#define TWCR_NOCMD (1<<TWEN)

#ifndef I2C_SPEED
#define I2C_SPEED 100000ul
#endif

enum i2c_status{
    I2C_STA_NONE,
    I2C_STA_BUSY,
    I2C_ERR_ADDR,
    I2C_ERR_SEND,
    I2C_ERR_RECV
};

void i2c_init(void);
uint8_t i2c_checkout(uint8_t addr);
struct i2c_packet * i2c_master_write(uint8_t addr, uint8_t raddr, uint8_t * data, uint8_t size);
struct i2c_packet * i2c_master_read(uint8_t addr, uint8_t raddr, uint8_t * data, uint8_t size);

#endif // I2C_H
