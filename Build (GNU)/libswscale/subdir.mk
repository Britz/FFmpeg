################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../libswscale/input.o \
../libswscale/options.o \
../libswscale/output.o \
../libswscale/rgb2rgb.o \
../libswscale/swscale.o \
../libswscale/swscale_unscaled.o \
../libswscale/utils.o \
../libswscale/yuv2rgb.o 

C_SRCS += \
../libswscale/colorspace-test.c \
../libswscale/input.c \
../libswscale/options.c \
../libswscale/output.c \
../libswscale/rgb2rgb.c \
../libswscale/rgb2rgb_template.c \
../libswscale/swscale-test.c \
../libswscale/swscale.c \
../libswscale/swscale_unscaled.c \
../libswscale/utils.c \
../libswscale/yuv2rgb.c 

OBJS += \
./libswscale/colorspace-test.o \
./libswscale/input.o \
./libswscale/options.o \
./libswscale/output.o \
./libswscale/rgb2rgb.o \
./libswscale/rgb2rgb_template.o \
./libswscale/swscale-test.o \
./libswscale/swscale.o \
./libswscale/swscale_unscaled.o \
./libswscale/utils.o \
./libswscale/yuv2rgb.o 

C_DEPS += \
./libswscale/colorspace-test.d \
./libswscale/input.d \
./libswscale/options.d \
./libswscale/output.d \
./libswscale/rgb2rgb.d \
./libswscale/rgb2rgb_template.d \
./libswscale/swscale-test.d \
./libswscale/swscale.d \
./libswscale/swscale_unscaled.d \
./libswscale/utils.d \
./libswscale/yuv2rgb.d 


# Each subdirectory must supply rules for building sources it contributes
libswscale/%.o: ../libswscale/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


