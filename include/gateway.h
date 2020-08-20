#ifndef FSM_H
#define FSM_H
#include <stdint.h>
#include <avr/pgmspace.h>

#define CMD_DELIMITER ':'
#define PREFIX_SIZE 4
#define GATE_MAXRESBUF 60
#define GATE_MAXCMDBUF 32
#define GATE_MAXHEXBUF 40
/**
 * For ascii buffer based functions
 */
typedef uint8_t (*func_t)(const char*);
/**
 * For binary buffer based functions
 */
typedef uint8_t (*func_b_t)(const uint8_t*, uint8_t);

typedef struct{
    PGM_P prefix;
    func_t exec_a;
} service_t;

const service_t * cmd_resolve(void);
uint8_t cmd_checkprefix_p(const char * cmd_prefix, const char * prefix);
//Typical errors
extern const char err_service_noavail[];
extern const char err_service_noarg[];
extern const char err_service_wrongcmd[];
extern const char err_service_bufovf[];
//MAIN TABLE
extern char resbuffer[GATE_MAXRESBUF];
extern uint8_t cmdbuffer[GATE_MAXCMDBUF];
//extern const service_t service_table[2];
#endif // FSM_H
