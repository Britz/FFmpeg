################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../doc/examples/decoding_encoding.c \
../doc/examples/filtering_audio.c \
../doc/examples/filtering_video.c \
../doc/examples/metadata.c \
../doc/examples/muxing.c 

OBJS += \
./doc/examples/decoding_encoding.o \
./doc/examples/filtering_audio.o \
./doc/examples/filtering_video.o \
./doc/examples/metadata.o \
./doc/examples/muxing.o 

C_DEPS += \
./doc/examples/decoding_encoding.d \
./doc/examples/filtering_audio.d \
./doc/examples/filtering_video.d \
./doc/examples/metadata.d \
./doc/examples/muxing.d 


# Each subdirectory must supply rules for building sources it contributes
doc/examples/%.o: ../doc/examples/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


