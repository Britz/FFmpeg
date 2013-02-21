################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../mt-work/yuvcmp.c 

OBJS += \
./mt-work/yuvcmp.o 

C_DEPS += \
./mt-work/yuvcmp.d 


# Each subdirectory must supply rules for building sources it contributes
mt-work/%.o: ../mt-work/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


