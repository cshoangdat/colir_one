################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/bmp5_api/Src/bmp5.c 

OBJS += \
./Drivers/bmp5_api/Src/bmp5.o 

C_DEPS += \
./Drivers/bmp5_api/Src/bmp5.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/bmp5_api/Src/%.o Drivers/bmp5_api/Src/%.su Drivers/bmp5_api/Src/%.cyclo: ../Drivers/bmp5_api/Src/%.c Drivers/bmp5_api/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"C:/Users/Administrator/Downloads/rocket_code/Drivers/bmp5_api/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-bmp5_api-2f-Src

clean-Drivers-2f-bmp5_api-2f-Src:
	-$(RM) ./Drivers/bmp5_api/Src/bmp5.cyclo ./Drivers/bmp5_api/Src/bmp5.d ./Drivers/bmp5_api/Src/bmp5.o ./Drivers/bmp5_api/Src/bmp5.su

.PHONY: clean-Drivers-2f-bmp5_api-2f-Src

