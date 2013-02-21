################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libavcodec/alpha/dsputil_alpha.c \
../libavcodec/alpha/motion_est_alpha.c \
../libavcodec/alpha/mpegvideo_alpha.c \
../libavcodec/alpha/simple_idct_alpha.c 

S_UPPER_SRCS += \
../libavcodec/alpha/dsputil_alpha_asm.S \
../libavcodec/alpha/motion_est_mvi_asm.S 

OBJS += \
./libavcodec/alpha/dsputil_alpha.o \
./libavcodec/alpha/dsputil_alpha_asm.o \
./libavcodec/alpha/motion_est_alpha.o \
./libavcodec/alpha/motion_est_mvi_asm.o \
./libavcodec/alpha/mpegvideo_alpha.o \
./libavcodec/alpha/simple_idct_alpha.o 

C_DEPS += \
./libavcodec/alpha/dsputil_alpha.d \
./libavcodec/alpha/motion_est_alpha.d \
./libavcodec/alpha/mpegvideo_alpha.d \
./libavcodec/alpha/simple_idct_alpha.d 


# Each subdirectory must supply rules for building sources it contributes
libavcodec/alpha/%.o: ../libavcodec/alpha/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

libavcodec/alpha/%.o: ../libavcodec/alpha/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: GCC Assembler'
	as  -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


