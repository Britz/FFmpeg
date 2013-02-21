################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../libavutil/x86/cpu.o 

C_SRCS += \
../libavutil/x86/cpu.c 

ASM_SRCS += \
../libavutil/x86/x86inc.asm \
../libavutil/x86/x86util.asm 

OBJS += \
./libavutil/x86/cpu.o \
./libavutil/x86/x86inc.o \
./libavutil/x86/x86util.o 

C_DEPS += \
./libavutil/x86/cpu.d 


# Each subdirectory must supply rules for building sources it contributes
libavutil/x86/%.o: ../libavutil/x86/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

libavutil/x86/%.o: ../libavutil/x86/%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: GCC Assembler'
	as  -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


