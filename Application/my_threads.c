#include <stdbool.h>
#include "main.h"
#include "cmsis_os2.h"
#include "my_printf.h"
#include "switch_monitor.h"
#include <stdio.h>

typedef struct {
	GPIO_TypeDef* EXT_LED_GPIO_Port;
	uint16_t EXT_LED_Pin;
	uint32_t led_delay;
	char* thread_name;
	bool led_pin_def;
	bool led_active;
	uint32_t thread_flags;
	osThreadId_t thread_id;

} thread_props;

thread_props my_thread_props[4]={
		{EXT_LED1_GPIO_Port,EXT_LED1_Pin,400,"led_1_thread",false,false,0x01,NULL},
		{EXT_LED2_GPIO_Port,EXT_LED2_Pin,500,"led_2_thread",true,false,0x02,NULL},
		{EXT_LED3_GPIO_Port,EXT_LED3_Pin,600,"led_3_thread",false,false,0x04,NULL},
		{EXT_LED4_GPIO_Port,EXT_LED4_Pin,700,"led_4_thread",true,false,0x08,NULL}
};


osSemaphoreId_t semaphor_id;



void Idle_Thread_Func(void *arg)
{
    while (true) {
		HAL_GPIO_WritePin(SLEEP_LED_GPIO_Port, SLEEP_LED_Pin,GPIO_PIN_SET);

    	__WFE();

		HAL_GPIO_WritePin(SLEEP_LED_GPIO_Port, SLEEP_LED_Pin,GPIO_PIN_RESET);

    }
}


void Thread_Func(void *arg)
{
    uint32_t thread_number = (uint32_t) arg;
    thread_props this_thread_props = my_thread_props[thread_number];

    if(!this_thread_props.led_pin_def){
        HAL_GPIO_WritePin(this_thread_props.EXT_LED_GPIO_Port, this_thread_props.EXT_LED_Pin, GPIO_PIN_SET);
    }

    while(true){
        uint32_t my_flag = osEventFlagsWait(getSwitcheventflag_id(), this_thread_props.thread_flags, osFlagsWaitAny, osWaitForever);

        if(my_flag & this_thread_props.thread_flags) {
            this_thread_props.led_active = true;
            osEventFlagsClear(getSwitcheventflag_id(), this_thread_props.thread_flags);
        }


        while (this_thread_props.led_active){
            osSemaphoreAcquire(semaphor_id, osWaitForever);

            if(this_thread_props.led_pin_def){
                HAL_GPIO_WritePin(this_thread_props.EXT_LED_GPIO_Port, this_thread_props.EXT_LED_Pin, GPIO_PIN_SET);
                MyPrintf("\nLED %u ON", (thread_number+1));
                osDelay(this_thread_props.led_delay);

                HAL_GPIO_WritePin(this_thread_props.EXT_LED_GPIO_Port, this_thread_props.EXT_LED_Pin, GPIO_PIN_RESET);
                MyPrintf("\nLED %u OFF", (thread_number+1));
                osDelay(this_thread_props.led_delay);
            } else{
                HAL_GPIO_WritePin(this_thread_props.EXT_LED_GPIO_Port, this_thread_props.EXT_LED_Pin, GPIO_PIN_RESET);
                MyPrintf("\nLED %u ON", (thread_number+1));
                osDelay(this_thread_props.led_delay);

                HAL_GPIO_WritePin(this_thread_props.EXT_LED_GPIO_Port, this_thread_props.EXT_LED_Pin, GPIO_PIN_SET);
                MyPrintf("\nLED %u OFF", (thread_number+1));
                osDelay(this_thread_props.led_delay);
            }

            osSemaphoreRelease(semaphor_id);
            uint32_t now_flag = osEventFlagsGet(getSwitcheventflag_id());
            if(now_flag){
            	this_thread_props.led_active = false;
            	osEventFlagsClear(getSwitcheventflag_id(), this_thread_props.thread_flags);
            }

        }
    }
}


bool My_Threads_Init(uint32_t nmb_of_leds)
{


	if(nmb_of_leds>4){
		MyPrintf("\ntoo many leds to turn on");
		return false;
	} else{

		semaphor_id=osSemaphoreNew(nmb_of_leds,nmb_of_leds,NULL);
		MyPrintf("\n%u token at start",osSemaphoreGetCount(semaphor_id));

		for(int i=0; i<4; i++){
			osThreadAttr_t thread_spes={};

			thread_spes.name=my_thread_props[i].thread_name;

			my_thread_props[i].thread_id = osThreadNew(Thread_Func, (void*)(uint32_t)i, &thread_spes);
			//MyPrintf("\n!!!!!!!!!!!!!  starta thread %i  !!!!!!!!!!!!!",i);
			if(my_thread_props[i].thread_id==NULL){
				MyPrintf("failed to create thread %i",i);
				break;
				return false;
			}
		}


		//lager idle thread slik at det går i sleep mode når denne kjøres;
	    osThreadAttr_t idle_spes = {};
	    idle_spes.name="idle_thread";
	    idle_spes.priority=osPriorityIdle;
	    osThreadNew(Idle_Thread_Func, NULL, &idle_spes);

		return true;
	}



}
