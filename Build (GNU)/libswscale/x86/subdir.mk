################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../libswscale/x86/input.o \
../libswscale/x86/output.o \
../libswscale/x86/rgb2rgb.o \
../libswscale/x86/scale.o \
../libswscale/x86/swscale_mmx.o \
../libswscale/x86/yuv2rgb_mmx.o 

C_SRCS += \
../libswscale/x86/rgb2rgb.c \
../libswscale/x86/rgb2rgb_template.c \
../libswscale/x86/swscale_mmx.c \
../libswscale/x86/swscale_template.c \
../libswscale/x86/w64xmmtest.c \
../libswscale/x86/yuv2rgb_mmx.c \
../libswscale/x86/yuv2rgb_template.c 

ASM_SRCS += \
../libswscale/x86/input.asm \
../libswscale/x86/output.asm \
../libswscale/x86/scale.asm 

OBJS += \
./libswscale/x86/input.o \
./libswscale/x86/output.o \
./libswscale/x86/rgb2rgb.o \
./libswscale/x86/rgb2rgb_template.o \
./libswscale/x86/scale.o \
./libswscale/x86/swscale_mmx.o \
./libswscale/x86/swscale_template.o \
./libswscale/x86/w64xmmtest.o \
./libswscale/x86/yuv2rgb_mmx.o \
./libswscale/x86/yuv2rgb_template.o 

C_DEPS += \
./libswscale/x86/rgb2rgb.d \
./libswscale/x86/rgb2rgb_template.d \
./libswscale/x86/swscale_mmx.d \
./libswscale/x86/swscale_template.d \
./libswscale/x86/w64xmmtest.d \
./libswscale/x86/yuv2rgb_mmx.d \
./libswscale/x86/yuv2rgb_template.d 


# Each subdirectory must supply rules for building sources it contributes
libswscale/x86/%.o: ../libswscale/x86/%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: GCC Assembler'
	as  -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

libswscale/x86/%.o: ../libswscale/x86/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


