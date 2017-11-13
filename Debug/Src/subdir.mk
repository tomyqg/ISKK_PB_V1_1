################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/DpAppl.c \
../Src/DpCfg.c \
../Src/DpDiag.c \
../Src/DpPrm.c \
../Src/ISKK.c \
../Src/dma.c \
../Src/dp_if.c \
../Src/dp_isr.c \
../Src/fsmc.c \
../Src/gpio.c \
../Src/main.c \
../Src/spi.c \
../Src/stm32f1xx_hal_msp.c \
../Src/stm32f1xx_it.c \
../Src/system_stm32f1xx.c \
../Src/usart.c 

OBJS += \
./Src/DpAppl.o \
./Src/DpCfg.o \
./Src/DpDiag.o \
./Src/DpPrm.o \
./Src/ISKK.o \
./Src/dma.o \
./Src/dp_if.o \
./Src/dp_isr.o \
./Src/fsmc.o \
./Src/gpio.o \
./Src/main.o \
./Src/spi.o \
./Src/stm32f1xx_hal_msp.o \
./Src/stm32f1xx_it.o \
./Src/system_stm32f1xx.o \
./Src/usart.o 

C_DEPS += \
./Src/DpAppl.d \
./Src/DpCfg.d \
./Src/DpDiag.d \
./Src/DpPrm.d \
./Src/ISKK.d \
./Src/dma.d \
./Src/dp_if.d \
./Src/dp_isr.d \
./Src/fsmc.d \
./Src/gpio.d \
./Src/main.d \
./Src/spi.d \
./Src/stm32f1xx_hal_msp.d \
./Src/stm32f1xx_it.d \
./Src/system_stm32f1xx.d \
./Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F103xE -I"C:/Users/mikolajp/git1/ISKK_PB_V1_1/Inc" -I"C:/Users/mikolajp/git1/ISKK_PB_V1_1/Drivers/STM32F1xx_HAL_Driver/Inc" -I"C:/Users/mikolajp/git1/ISKK_PB_V1_1/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"C:/Users/mikolajp/git1/ISKK_PB_V1_1/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/mikolajp/git1/ISKK_PB_V1_1/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


