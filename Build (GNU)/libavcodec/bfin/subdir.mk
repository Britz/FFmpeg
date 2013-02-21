################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libavcodec/bfin/dsputil_bfin.c \
../libavcodec/bfin/mpegvideo_bfin.c \
../libavcodec/bfin/vp3_bfin.c 

S_UPPER_SRCS += \
../libavcodec/bfin/fdct_bfin.S \
../libavcodec/bfin/idct_bfin.S \
../libavcodec/bfin/pixels_bfin.S \
../libavcodec/bfin/vp3_idct_bfin.S 

OBJS += \
./libavcodec/bfin/dsputil_bfin.o \
./libavcodec/bfin/fdct_bfin.o \
./libavcodec/bfin/idct_bfin.o \
./libavcodec/bfin/mpegvideo_bfin.o \
./libavcodec/bfin/pixels_bfin.o \
./libavcodec/bfin/vp3_bfin.o \
./libavcodec/bfin/vp3_idct_bfin.o 

C_DEPS += \
./libavcodec/bfin/dsputil_bfin.d \
./libavcodec/bfin/mpegvideo_bfin.d \
./libavcodec/bfin/vp3_bfin.d 


# Each subdirectory must supply rules for building sources it contributes
libavcodec/bfin/%.o: ../libavcodec/bfin/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

libavcodec/bfin/%.o: ../libavcodec/bfin/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: GCC Assembler'
	as  -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


