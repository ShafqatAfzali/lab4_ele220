################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/blinky.c \
../Application/my_adc.c \
../Application/my_printf.c \
../Application/my_threads.c \
../Application/switch_monitor.c 

OBJS += \
./Application/blinky.o \
./Application/my_adc.o \
./Application/my_printf.o \
./Application/my_threads.o \
./Application/switch_monitor.o 

C_DEPS += \
./Application/blinky.d \
./Application/my_adc.d \
./Application/my_printf.d \
./Application/my_threads.d \
./Application/switch_monitor.d 


# Each subdirectory must supply rules for building sources it contributes
Application/%.o Application/%.su Application/%.cyclo: ../Application/%.c Application/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Application -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application

clean-Application:
	-$(RM) ./Application/blinky.cyclo ./Application/blinky.d ./Application/blinky.o ./Application/blinky.su ./Application/my_adc.cyclo ./Application/my_adc.d ./Application/my_adc.o ./Application/my_adc.su ./Application/my_printf.cyclo ./Application/my_printf.d ./Application/my_printf.o ./Application/my_printf.su ./Application/my_threads.cyclo ./Application/my_threads.d ./Application/my_threads.o ./Application/my_threads.su ./Application/switch_monitor.cyclo ./Application/switch_monitor.d ./Application/switch_monitor.o ./Application/switch_monitor.su

.PHONY: clean-Application

