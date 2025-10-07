#include "blinky.h"
#include "main.h"
#include "stm32f4xx_hal_conf.h"
#include "stm32f4xx_it.h"

void blink(void){
        HAL_GPIO_TogglePin(EXT_LED3_GPIO_Port, EXT_LED3_GPIO_Port);
}
