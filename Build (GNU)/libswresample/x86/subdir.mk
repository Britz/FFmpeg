################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../libswresample/x86/audio_convert.o \
../libswresample/x86/swresample_x86.o 

C_SRCS += \
../libswresample/x86/swresample_x86.c 

ASM_SRCS += \
../libswresample/x86/audio_convert.asm 

OBJS += \
./libswresample/x86/audio_convert.o \
./libswresample/x86/swresample_x86.o 

C_DEPS += \
./libswresample/x86/swresample_x86.d 


# Each subdirectory must supply rules for building sources it contributes
libswresample/x86/%.o: ../libswresample/x86/%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: GCC Assembler'
	as  -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

libswresample/x86/%.o: ../libswresample/x86/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


