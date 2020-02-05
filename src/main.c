#ifdef __GNUC__
#include "avr/io.h"
#include "avr/interrupt.h"
#include "avr/cpufunc.h"
#elif __ICCAVR__
#include "ioavr.h"
#include "inavr.h"
#endif

#include "usi_twi.h"

// Sample TWI transmission commands
#define TWI_CMD_MASTER_WRITE 0x10
#define TWI_CMD_MASTER_READ 0x20
#define TWI_CMD_MASTER_WRITE_ALOT 0x30

int main(void)
{
    uint8_t b[] = {1,2,3,4,5};
    twi_slave_init(0x10,b,5);
    sei();
    for (;;) {
    }
    return 0;
}
