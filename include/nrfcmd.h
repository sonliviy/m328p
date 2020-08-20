#ifndef NRFCMD_H
#define NRFCMD_H
#include "gateway.h"

void nrfcmd_resolver_init(uint8_t * addr);
uint8_t nrfcmd_checkout(void);
char * nrfcmd_execute(uint8_t len);

#endif // NRFCMD_H
