################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libswscale/ppc/swscale_altivec.c \
../libswscale/ppc/yuv2rgb_altivec.c \
../libswscale/ppc/yuv2yuv_altivec.c 

OBJS += \
./libswscale/ppc/swscale_altivec.o \
./libswscale/ppc/yuv2rgb_altivec.o \
./libswscale/ppc/yuv2yuv_altivec.o 

C_DEPS += \
./libswscale/ppc/swscale_altivec.d \
./libswscale/ppc/yuv2rgb_altivec.d \
./libswscale/ppc/yuv2yuv_altivec.d 


# Each subdirectory must supply rules for building sources it contributes
libswscale/ppc/%.o: ../libswscale/ppc/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


