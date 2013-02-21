################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libswscale/sparc/yuv2rgb_vis.c 

OBJS += \
./libswscale/sparc/yuv2rgb_vis.o 

C_DEPS += \
./libswscale/sparc/yuv2rgb_vis.d 


# Each subdirectory must supply rules for building sources it contributes
libswscale/sparc/%.o: ../libswscale/sparc/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


