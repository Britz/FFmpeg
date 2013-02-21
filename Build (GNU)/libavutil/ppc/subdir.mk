################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libavutil/ppc/cpu.c 

OBJS += \
./libavutil/ppc/cpu.o 

C_DEPS += \
./libavutil/ppc/cpu.d 


# Each subdirectory must supply rules for building sources it contributes
libavutil/ppc/%.o: ../libavutil/ppc/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


