#ifndef SERVICE_NRF_H
#define SERVICE_NRF_H
#include "gateway.h"

extern const char service_nrf_prefix[];
extern const char service_nrf_infostr[];
uint8_t service_nrf_exec(const char * buf);

#endif // SERVICE_NRF_H
