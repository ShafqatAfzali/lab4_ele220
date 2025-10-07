#include <stdbool.h>
#include "main.h"
#include "cmsis_os2.h"
#include "my_printf.h"
#include <stdio.h>
#include "switch_monitor.h"

static osMessageQueueId_t queue_id = NULL;
static osEventFlagsId_t eventflag_id = NULL;
static osThreadId_t switch_monitor_thread = NULL;


#define EVENT_FLAG_SWITCH_1_PRESSED 0x01
#define EVENT_FLAG_SWITCH_2_PRESSED 0x02
#define EVENT_FLAG_SWITCH_3_PRESSED 0x04
#define EVENT_FLAG_SWITCH_4_PRESSED 0x08

typedef enum {
    SWITCH_PRESSED,
    SWITCH_RELEASED
} switch_action_t;

typedef struct {
    uint16_t pin;
    switch_action_t action;
} switch_message_t;

#define SwitchMonitorInterruptCB HAL_GPIO_EXTI_Callback

void SwitchMonitor(void *arg) {
    switch_message_t msg;

    while(true) {
        if (osMessageQueueGet(queue_id, &msg, NULL, osWaitForever) == osOK) {
            uint8_t switch_num = 0;
            const char *action_str = "";

            switch(msg.pin) {
                case SW1_Pin: switch_num = 1; break;
                case SW2_Pin: switch_num = 2; break;
                case SW3_Pin: switch_num = 3; break;
                case SW4_Pin: switch_num = 4; break;
                default: switch_num = 0; break;
            }

            if (msg.action == SWITCH_PRESSED) {
                action_str = "pressed";

                switch(msg.pin) {
                    case SW1_Pin:
                        osEventFlagsSet(eventflag_id, EVENT_FLAG_SWITCH_1_PRESSED);
                        break;
                    case SW2_Pin:
                        osEventFlagsSet(eventflag_id, EVENT_FLAG_SWITCH_2_PRESSED);
                        break;
                    case SW3_Pin:
                        osEventFlagsSet(eventflag_id, EVENT_FLAG_SWITCH_3_PRESSED);
                        break;
                    case SW4_Pin:
                        osEventFlagsSet(eventflag_id, EVENT_FLAG_SWITCH_4_PRESSED);
                        break;
                }
            } else {
                action_str = "released";
            }

            if (switch_num > 0) {
                MyPrintf("Switch %d is %s\n", switch_num, action_str);
            }
        }
    }
}

void SwitchMonitorInterruptCB(uint16_t pin) {
    __HAL_GPIO_EXTI_CLEAR_IT(pin);

    switch_message_t msg;
    msg.pin = pin;

    GPIO_TypeDef* port = NULL;

    switch(pin) {
        case SW1_Pin: port = SW1_GPIO_Port; break;
        case SW2_Pin: port = SW2_GPIO_Port; break;
        case SW3_Pin: port = SW3_GPIO_Port; break;
        case SW4_Pin: port = SW4_GPIO_Port; break;
        default: return;
    }

    if(pin==SW1_Pin || pin==SW3_Pin){
        if (HAL_GPIO_ReadPin(port, pin) == GPIO_PIN_RESET) {
            msg.action = SWITCH_PRESSED;
        } else {
            msg.action = SWITCH_RELEASED;
        }
    } else{
        if (HAL_GPIO_ReadPin(port, pin) == GPIO_PIN_RESET) {
            msg.action = SWITCH_RELEASED;
        } else {
            msg.action = SWITCH_PRESSED;
        }
    }


    osMessageQueuePut(queue_id, &msg, 0, 0);
}

bool SwitchMonitorInit(void) {
	queue_id = osMessageQueueNew(16, sizeof(switch_message_t), NULL);
    if (queue_id == NULL) {
        return false;
    }

    eventflag_id = osEventFlagsNew(NULL);
    if (eventflag_id == NULL) {
        return false;
    }

    const osThreadAttr_t switch_monitor_attributes = {
        .name = "SwitchMonitor",
        .stack_size = 1024,
        .priority = osPriorityNormal,
    };

    switch_monitor_thread = osThreadNew(SwitchMonitor, NULL, &switch_monitor_attributes);
    if (switch_monitor_thread == NULL) {
        return false;
    }

    return true;
}

osEventFlagsId_t getSwitcheventflag_id(void) {
    return eventflag_id;
}
