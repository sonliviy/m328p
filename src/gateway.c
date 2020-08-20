#include <util/delay.h>
#include <avr/pgmspace.h>
#include <string.h>
#include "gateway.h"
//#include "service_nrf.h"
#include "service_i2c.h"
//#include "service_temp.h"
#include "uart.h"
#include "utils.h"


const char err_service_noarg[] PROGMEM =   "ERR:WRONGARGSCOUNT";
const char err_service_noavail[] PROGMEM = "ERR:NOTAVAILABLE";
const char err_service_wrongcmd[] PROGMEM =   "ERR:WRONGCMDSIZE";
const char err_service_bufovf[] PROGMEM =  "ERR:BUFFLESSTHANDATA";
#ifdef DEBUG
const char test_prefix[] PROGMEM = "TST";
uint8_t testcmd_a(const char* buf);
uint8_t testcmd_b(const uint8_t * buf, uint8_t size);
#endif

char resbuffer[GATE_MAXRESBUF];//response
uint8_t cmdbuffer[GATE_MAXCMDBUF];

static const service_t service_table[] = {
    #ifdef DEBUG
    {test_prefix, testcmd_a, testcmd_b},
    #endif
//    {service_nrf_prefix, service_nrf_exec},
    {service_i2c_prefix, service_i2c_exec},
//    {service_tsl2561_prefix, service_tsl2561_exec}
//    {avrtemp_prefix, avrtemp_exec, NULL}
};

const service_t * cmd_resolve(){
    for(uint8_t i = 0; i < sizeof (service_table)/sizeof (service_t); i++){
        uint8_t prefix_size = cmd_checkprefix_p(service_table[i].prefix, (char *)cmdbuffer);
        if(!prefix_size)
            continue;
        else{
            #ifdef DEBUG
                uart_puts_P("Match:");
                uart_puts_p(service_table[i].prefix);
                uart_putc('\n');
            #endif
            strcpy_P(resbuffer, service_table[i].prefix);
            strcat_P(resbuffer, PSTR(":"));
            return &service_table[i];
        }
    }
    #ifdef DEBUG
        uart_puts_P("NOCMD\n");
    #endif
    return NULL;
}

uint8_t cmd_checkprefix_p(const char * cmd_prefix, const char * prefix){
    char ch;
    uint8_t i = 0;
    while((ch = pgm_read_byte(cmd_prefix+i)))
        if(ch == *(prefix+i)){
            i++;
            continue;
        }
        else
            return 0;
    return i;
}


#ifdef DEBUG
uint8_t testcmd_a(const char * buf){
    strcat(resbuffer,buf);
    return 0;
}

uint8_t testcmd_b(const uint8_t * buf, uint8_t size){
    hextoan(buf, resbuffer, size);
    uart_puts(resbuffer);
    return 0;
}
#endif
