#ifndef AVR_TEMP_H
#define AVR_TEMP_H
#include "gateway.h"
extern const char avrtemp_prefix[];
extern const char avrtemp_infostr[];
uint16_t avrtemp_read(void);
uint8_t avrtemp_testcmd(const char * buf);
uint8_t avrtemp_exec(const char * buf);

#define ADC_VR_AREF (0 << 6)
#define ADC_VR_AVCC (1 << 6)
#define ADC_VR_1V1 (3 << 6)

#define ADC_ICS_TEMP 8
#define ADC_ICS_1V1 15

#define ADC_PS_2 0
#define ADC_PS_4 2
#define ADC_PS_8 3
#define ADC_PS_16 4
#define ADC_PS_32 5
#define ADC_PS_64 6
#define ADC_PS_128 7

#endif //AVR_TEMP_H
