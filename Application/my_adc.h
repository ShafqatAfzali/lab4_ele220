#ifndef MY_ADC_H
#define MY_ADC_H

#include <stdint.h>


uint16_t MyADC_ReadVrefintRaw(void);
uint32_t MyADC_ComputeVrefPlus_mV(uint16_t raw);
void     MyADC_PrintOnce(void);

#endif
