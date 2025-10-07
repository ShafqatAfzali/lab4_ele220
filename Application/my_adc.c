#include "my_adc.h"
#include "main.h"
#include <stdio.h>
#include "my_printf.h"

extern ADC_HandleTypeDef hadc1;

#ifndef VREFINT_CAL_VREF
#define VREFINT_CAL_VREF  (3300UL)
#endif

#ifndef VREFINT_CAL_ADDR
#define VREFINT_CAL_ADDR  ((uint16_t*) (0x1FFF7A2AU))
#endif

#define VREFINT_CAL       (*((uint16_t*) VREFINT_CAL_ADDR))


uint16_t MyADC_ReadVrefintRaw(void)
{
    HAL_ADC_Start(&hadc1);
    (void)HAL_ADC_PollForConversion(&hadc1, 10);
    uint16_t raw = (uint16_t)HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);
    return raw;
}


uint32_t MyADC_ComputeVrefPlus_mV(uint16_t vrefint_data)
{
    if (vrefint_data == 0) return 0;
    return ( (uint32_t)VREFINT_CAL_VREF * (uint32_t)VREFINT_CAL ) / (uint32_t)vrefint_data;
}


void MyADC_PrintOnce(void)
{
    uint16_t raw = MyADC_ReadVrefintRaw();
    uint32_t vrefp_mv = MyADC_ComputeVrefPlus_mV(raw);

    MyPrintf("VREFINT raw: 0x%03X\n", raw);

    MyPrintf("VREF+ kalibrert: %lu mV\n", (unsigned long)vrefp_mv);
}

