################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libswscale/bfin/swscale_bfin.c \
../libswscale/bfin/yuv2rgb_bfin.c 

S_UPPER_SRCS += \
../libswscale/bfin/internal_bfin.S 

OBJS += \
./libswscale/bfin/internal_bfin.o \
./libswscale/bfin/swscale_bfin.o \
./libswscale/bfin/yuv2rgb_bfin.o 

C_DEPS += \
./libswscale/bfin/swscale_bfin.d \
./libswscale/bfin/yuv2rgb_bfin.d 


# Each subdirectory must supply rules for building sources it contributes
libswscale/bfin/%.o: ../libswscale/bfin/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: GCC Assembler'
	as  -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

libswscale/bfin/%.o: ../libswscale/bfin/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


