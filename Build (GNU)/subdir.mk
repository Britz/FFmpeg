################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../cmdutils.o \
../ffmpeg.o \
../ffplay.o \
../ffprobe.o \
../ffserver.o 

C_SRCS += \
../cmdutils.c \
../ffmpeg.c \
../ffplay.c \
../ffprobe.c \
../ffserver.c 

ASM_SRCS += \
../config.asm 

OBJS += \
./cmdutils.o \
./config.o \
./ffmpeg.o \
./ffplay.o \
./ffprobe.o \
./ffserver.o 

C_DEPS += \
./cmdutils.d \
./ffmpeg.d \
./ffplay.d \
./ffprobe.d \
./ffserver.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: GCC Assembler'
	as  -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


