################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libavcodec/sh4/dsputil_align.c \
../libavcodec/sh4/dsputil_sh4.c \
../libavcodec/sh4/idct_sh4.c \
../libavcodec/sh4/qpel.c 

OBJS += \
./libavcodec/sh4/dsputil_align.o \
./libavcodec/sh4/dsputil_sh4.o \
./libavcodec/sh4/idct_sh4.o \
./libavcodec/sh4/qpel.o 

C_DEPS += \
./libavcodec/sh4/dsputil_align.d \
./libavcodec/sh4/dsputil_sh4.d \
./libavcodec/sh4/idct_sh4.d \
./libavcodec/sh4/qpel.d 


# Each subdirectory must supply rules for building sources it contributes
libavcodec/sh4/%.o: ../libavcodec/sh4/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


