################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../tools/aviocat.c \
../tools/cws2fws.c \
../tools/enum_options.c \
../tools/ffeval.c \
../tools/graph2dot.c \
../tools/ismindex.c \
../tools/lavfi-showfiltfmts.c \
../tools/pktdumper.c \
../tools/probetest.c \
../tools/qt-faststart.c \
../tools/trasher.c 

OBJS += \
./tools/aviocat.o \
./tools/cws2fws.o \
./tools/enum_options.o \
./tools/ffeval.o \
./tools/graph2dot.o \
./tools/ismindex.o \
./tools/lavfi-showfiltfmts.o \
./tools/pktdumper.o \
./tools/probetest.o \
./tools/qt-faststart.o \
./tools/trasher.o 

C_DEPS += \
./tools/aviocat.d \
./tools/cws2fws.d \
./tools/enum_options.d \
./tools/ffeval.d \
./tools/graph2dot.d \
./tools/ismindex.d \
./tools/lavfi-showfiltfmts.d \
./tools/pktdumper.d \
./tools/probetest.d \
./tools/qt-faststart.d \
./tools/trasher.d 


# Each subdirectory must supply rules for building sources it contributes
tools/%.o: ../tools/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


