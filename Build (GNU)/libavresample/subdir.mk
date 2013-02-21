################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libavresample/audio_convert.c \
../libavresample/audio_data.c \
../libavresample/audio_mix.c \
../libavresample/audio_mix_matrix.c \
../libavresample/avresample-test.c \
../libavresample/options.c \
../libavresample/resample.c \
../libavresample/utils.c 

OBJS += \
./libavresample/audio_convert.o \
./libavresample/audio_data.o \
./libavresample/audio_mix.o \
./libavresample/audio_mix_matrix.o \
./libavresample/avresample-test.o \
./libavresample/options.o \
./libavresample/resample.o \
./libavresample/utils.o 

C_DEPS += \
./libavresample/audio_convert.d \
./libavresample/audio_data.d \
./libavresample/audio_mix.d \
./libavresample/audio_mix_matrix.d \
./libavresample/avresample-test.d \
./libavresample/options.d \
./libavresample/resample.d \
./libavresample/utils.d 


# Each subdirectory must supply rules for building sources it contributes
libavresample/%.o: ../libavresample/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


