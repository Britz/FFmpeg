################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libavutil/arm/cpu.c 

OBJS += \
./libavutil/arm/cpu.o 

C_DEPS += \
./libavutil/arm/cpu.d 


# Each subdirectory must supply rules for building sources it contributes
libavutil/arm/%.o: ../libavutil/arm/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


