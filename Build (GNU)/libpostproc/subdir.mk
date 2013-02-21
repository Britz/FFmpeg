################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../libpostproc/postprocess.o 

C_SRCS += \
../libpostproc/postprocess.c \
../libpostproc/postprocess_altivec_template.c \
../libpostproc/postprocess_template.c 

OBJS += \
./libpostproc/postprocess.o \
./libpostproc/postprocess_altivec_template.o \
./libpostproc/postprocess_template.o 

C_DEPS += \
./libpostproc/postprocess.d \
./libpostproc/postprocess_altivec_template.d \
./libpostproc/postprocess_template.d 


# Each subdirectory must supply rules for building sources it contributes
libpostproc/%.o: ../libpostproc/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


