################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../doc/print_options.o 

C_SRCS += \
../doc/print_options.c 

OBJS += \
./doc/print_options.o 

C_DEPS += \
./doc/print_options.d 


# Each subdirectory must supply rules for building sources it contributes
doc/%.o: ../doc/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


