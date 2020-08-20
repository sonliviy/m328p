#ifndef TSL2561_H
#define TSL2561_H
#include <stdint.h>


#define TSLA_GND 0x29
#define TSLA_FLT 0x39
#define TSLA_VDD 0x49
#define TSLI_014MS 0
#define TSLI_100MS 1
#define TSLI_400MS 2
#define TSLG_1X 0
#define TSLG_16 0x10

uint8_t tsl2561_init(uint8_t tsla, uint8_t options);
uint8_t tsl2561_read(uint8_t tsla, uint16_t * ir, uint16_t * full);
uint8_t tsl2561_update(uint8_t tsla, uint8_t options);

#endif // TSL2561_H
