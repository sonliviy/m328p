#ifndef RESOLVER_UART_H
#define RESOLVER_UART_H
#define UART_RXn_BUFFER_SIZE 32
#define UART_TXn_BUFFER_SIZE 32


#include "resolver.h"
#include "uart.h"


void resolver_uart_init(uint16_t baud);
uint8_t resolver_uart_checkout(void);

#endif // RESOLVER_UART_H
