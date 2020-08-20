#ifndef SPI_H
#define SPI_H

#include <avr/io.h>

#if defined (__AVR_ATmega328P__)
#define SPI_DDR DDRB
#define SPI_PORT PORTB
#define SPI_SCK PB5
#define SPI_MISO PB4
#define SPI_MOSI PB3
#define SPI_SS PB2
#endif

#define SPI_MASTER 1

uint8_t spi_send(uint8_t data);
uint8_t spi_isenabled(void);
void spi_setup(uint8_t m_mode, uint8_t d_order, /*uint8_t cpol, uint8_t cpha*/ uint8_t clock);


#endif // SPI_H
