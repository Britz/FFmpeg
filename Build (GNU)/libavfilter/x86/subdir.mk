################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../libavfilter/x86/gradfun.o 

C_SRCS += \
../libavfilter/x86/gradfun.c \
../libavfilter/x86/yadif.c \
../libavfilter/x86/yadif_template.c 

OBJS += \
./libavfilter/x86/gradfun.o \
./libavfilter/x86/yadif.o \
./libavfilter/x86/yadif_template.o 

C_DEPS += \
./libavfilter/x86/gradfun.d \
./libavfilter/x86/yadif.d \
./libavfilter/x86/yadif_template.d 


# Each subdirectory must supply rules for building sources it contributes
libavfilter/x86/%.o: ../libavfilter/x86/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


