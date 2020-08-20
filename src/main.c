/*
 *  Copyright (c) 2020
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 */
#include <stdio.h>
#include "resolver.h"

int main(void)
{   char temp[100];
    resolver_uart_init(UART_BAUD_SELECT(BAUD, F_CPU));
    sprintf(temp,"%d %f\n", 100, 3.1415);
    uart_puts(temp);
    while(1);
//    while(1){
//    if(resolver_uart_checkout())
//        uart_puts(resolver_uart_execute());
//    }
//    nrf24_setup();
//    nrf24_setup_ch(100);
//    nrf24_createpipe(1,addr,0,1,1);
//    nrf24_startlistening();
//    sei();
//    while(1){
//        if(nrf24_rxcheckout()!=6){
//            size = nrf24_receive((uint8_t*)cmdbuf);
//            cmdbuf[size] = 0;
//            uart_puts(cmdbuf);
//            uart_putc('\n');
//        }
//    }
}




