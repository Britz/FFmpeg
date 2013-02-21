################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../tests/audiogen.c \
../tests/base64.c \
../tests/rotozoom.c \
../tests/tiny_psnr.c \
../tests/utils.c \
../tests/videogen.c 

OBJS += \
./tests/audiogen.o \
./tests/base64.o \
./tests/rotozoom.o \
./tests/tiny_psnr.o \
./tests/utils.o \
./tests/videogen.o 

C_DEPS += \
./tests/audiogen.d \
./tests/base64.d \
./tests/rotozoom.d \
./tests/tiny_psnr.d \
./tests/utils.d \
./tests/videogen.d 


# Each subdirectory must supply rules for building sources it contributes
tests/%.o: ../tests/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


