#ifndef service_I2C_H
#define service_I2C_H
#include "gateway.h"
extern const char service_i2c_prefix[];
extern const char service_i2c_infostr[];
uint8_t service_i2c_exec(const char * buf);
#endif //service_I2C_H
