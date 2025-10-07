#ifndef SWITCH_MONITOR_H
#define SWITCH_MONITOR_H

#include "main.h"
#include "cmsis_os.h"
#include <stdbool.h>


bool SwitchMonitorInit(void);
void SwitchMonitorInterruptCB(uint16_t pin);
osEventFlagsId_t getSwitcheventflag_id(void);

#endif
