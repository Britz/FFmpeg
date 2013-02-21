################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../libswresample/audioconvert.o \
../libswresample/dither.o \
../libswresample/rematrix.o \
../libswresample/resample.o \
../libswresample/swresample.o 

C_SRCS += \
../libswresample/audioconvert.c \
../libswresample/dither.c \
../libswresample/rematrix.c \
../libswresample/rematrix_template.c \
../libswresample/resample.c \
../libswresample/resample_template.c \
../libswresample/swresample-test.c \
../libswresample/swresample.c 

OBJS += \
./libswresample/audioconvert.o \
./libswresample/dither.o \
./libswresample/rematrix.o \
./libswresample/rematrix_template.o \
./libswresample/resample.o \
./libswresample/resample_template.o \
./libswresample/swresample-test.o \
./libswresample/swresample.o 

C_DEPS += \
./libswresample/audioconvert.d \
./libswresample/dither.d \
./libswresample/rematrix.d \
./libswresample/rematrix_template.d \
./libswresample/resample.d \
./libswresample/resample_template.d \
./libswresample/swresample-test.d \
./libswresample/swresample.d 


# Each subdirectory must supply rules for building sources it contributes
libswresample/%.o: ../libswresample/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


