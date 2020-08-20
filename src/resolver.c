#include "resolver.h"
response_t * testreaction(const uint8_t cmd, const uint8_t *data);
response_t * testreactionf(const uint8_t cmd, const uint8_t *data);
static response_t r;
reaction_t reactions[] = {
    testreaction,
    testreactionf
};

void resolver_init(void){
    resolver_init_uart(0);
}

response_t * testreaction(const uint8_t cmd, const uint8_t *data){
    r.type = u8_t;
    if(cmd == 'w')
        r.data.u8_t = *((uint8_t*)data);
    else
        r.data.u8_t = 3;
    return &r;
}

response_t * testreactionf(const uint8_t cmd, const uint8_t *data){
    r.type = flt_t;
    if(cmd == 'w')
        r.data.flt_t = *((float*)data);
    else
        r.data.flt_t = 3.1415;
    return &r;
}
