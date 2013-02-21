################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libavcodec/mips/dsputil_mmi.c \
../libavcodec/mips/idct_mmi.c \
../libavcodec/mips/mpegvideo_mmi.c 

OBJS += \
./libavcodec/mips/dsputil_mmi.o \
./libavcodec/mips/idct_mmi.o \
./libavcodec/mips/mpegvideo_mmi.o 

C_DEPS += \
./libavcodec/mips/dsputil_mmi.d \
./libavcodec/mips/idct_mmi.d \
./libavcodec/mips/mpegvideo_mmi.d 


# Each subdirectory must supply rules for building sources it contributes
libavcodec/mips/%.o: ../libavcodec/mips/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


