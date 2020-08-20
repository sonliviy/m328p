#ifndef RESOLVER_H
#define RESOLVER_H
#define REACTIONS_SIZE 2
#define CMDBUFFER_SIZE 32

#include "resolver_uart.h"


enum response_type_e{
    s8_t,
    s16_t,
    s32_t,
    u8_t,
    u16_t,
    u32_t,
    flt_t
};
union response_u{
    int8_t s8_t;
    int16_t s16_t;
    int32_t s32_t;
    uint8_t u8_t;
    uint16_t u16_t;
    uint32_t u32_t;
    float flt_t;
};
typedef struct
{
    enum response_type_e type;
    union response_u data;
} response_t;
typedef response_t * (*reaction_t)(const uint8_t, const uint8_t *);

extern reaction_t reactions[];
uint8_t cmdbuffer[CMDBUFFER_SIZE];

void resolver_init(void);


#endif // RESOLVER_H
