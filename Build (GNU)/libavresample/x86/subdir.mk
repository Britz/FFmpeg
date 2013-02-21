################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libavresample/x86/audio_convert_init.c \
../libavresample/x86/audio_mix_init.c 

ASM_SRCS += \
../libavresample/x86/audio_convert.asm \
../libavresample/x86/audio_mix.asm 

OBJS += \
./libavresample/x86/audio_convert.o \
./libavresample/x86/audio_convert_init.o \
./libavresample/x86/audio_mix.o \
./libavresample/x86/audio_mix_init.o 

C_DEPS += \
./libavresample/x86/audio_convert_init.d \
./libavresample/x86/audio_mix_init.d 


# Each subdirectory must supply rules for building sources it contributes
libavresample/x86/%.o: ../libavresample/x86/%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: GCC Assembler'
	as  -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

libavresample/x86/%.o: ../libavresample/x86/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


