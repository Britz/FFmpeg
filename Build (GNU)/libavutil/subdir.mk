################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../libavutil/adler32.o \
../libavutil/aes.o \
../libavutil/audio_fifo.o \
../libavutil/audioconvert.o \
../libavutil/avstring.o \
../libavutil/base64.o \
../libavutil/bprint.o \
../libavutil/cpu.o \
../libavutil/crc.o \
../libavutil/des.o \
../libavutil/dict.o \
../libavutil/error.o \
../libavutil/eval.o \
../libavutil/fifo.o \
../libavutil/file.o \
../libavutil/imgutils.o \
../libavutil/intfloat_readwrite.o \
../libavutil/inverse.o \
../libavutil/lfg.o \
../libavutil/lls.o \
../libavutil/log.o \
../libavutil/lzo.o \
../libavutil/mathematics.o \
../libavutil/md5.o \
../libavutil/mem.o \
../libavutil/opt.o \
../libavutil/parseutils.o \
../libavutil/pixdesc.o \
../libavutil/random_seed.o \
../libavutil/rational.o \
../libavutil/rc4.o \
../libavutil/samplefmt.o \
../libavutil/sha.o \
../libavutil/timecode.o \
../libavutil/tree.o \
../libavutil/utils.o 

C_SRCS += \
../libavutil/adler32.c \
../libavutil/aes.c \
../libavutil/audio_fifo.c \
../libavutil/audioconvert.c \
../libavutil/avstring.c \
../libavutil/base64.c \
../libavutil/bprint.c \
../libavutil/cpu.c \
../libavutil/crc.c \
../libavutil/des.c \
../libavutil/dict.c \
../libavutil/error.c \
../libavutil/eval.c \
../libavutil/fifo.c \
../libavutil/file.c \
../libavutil/imgutils.c \
../libavutil/integer.c \
../libavutil/intfloat_readwrite.c \
../libavutil/inverse.c \
../libavutil/lfg.c \
../libavutil/lls.c \
../libavutil/log.c \
../libavutil/lzo.c \
../libavutil/mathematics.c \
../libavutil/md5.c \
../libavutil/mem.c \
../libavutil/opt.c \
../libavutil/parseutils.c \
../libavutil/pca.c \
../libavutil/pixdesc.c \
../libavutil/random_seed.c \
../libavutil/rational.c \
../libavutil/rc4.c \
../libavutil/samplefmt.c \
../libavutil/sha.c \
../libavutil/softfloat.c \
../libavutil/timecode.c \
../libavutil/tree.c \
../libavutil/utils.c 

OBJS += \
./libavutil/adler32.o \
./libavutil/aes.o \
./libavutil/audio_fifo.o \
./libavutil/audioconvert.o \
./libavutil/avstring.o \
./libavutil/base64.o \
./libavutil/bprint.o \
./libavutil/cpu.o \
./libavutil/crc.o \
./libavutil/des.o \
./libavutil/dict.o \
./libavutil/error.o \
./libavutil/eval.o \
./libavutil/fifo.o \
./libavutil/file.o \
./libavutil/imgutils.o \
./libavutil/integer.o \
./libavutil/intfloat_readwrite.o \
./libavutil/inverse.o \
./libavutil/lfg.o \
./libavutil/lls.o \
./libavutil/log.o \
./libavutil/lzo.o \
./libavutil/mathematics.o \
./libavutil/md5.o \
./libavutil/mem.o \
./libavutil/opt.o \
./libavutil/parseutils.o \
./libavutil/pca.o \
./libavutil/pixdesc.o \
./libavutil/random_seed.o \
./libavutil/rational.o \
./libavutil/rc4.o \
./libavutil/samplefmt.o \
./libavutil/sha.o \
./libavutil/softfloat.o \
./libavutil/timecode.o \
./libavutil/tree.o \
./libavutil/utils.o 

C_DEPS += \
./libavutil/adler32.d \
./libavutil/aes.d \
./libavutil/audio_fifo.d \
./libavutil/audioconvert.d \
./libavutil/avstring.d \
./libavutil/base64.d \
./libavutil/bprint.d \
./libavutil/cpu.d \
./libavutil/crc.d \
./libavutil/des.d \
./libavutil/dict.d \
./libavutil/error.d \
./libavutil/eval.d \
./libavutil/fifo.d \
./libavutil/file.d \
./libavutil/imgutils.d \
./libavutil/integer.d \
./libavutil/intfloat_readwrite.d \
./libavutil/inverse.d \
./libavutil/lfg.d \
./libavutil/lls.d \
./libavutil/log.d \
./libavutil/lzo.d \
./libavutil/mathematics.d \
./libavutil/md5.d \
./libavutil/mem.d \
./libavutil/opt.d \
./libavutil/parseutils.d \
./libavutil/pca.d \
./libavutil/pixdesc.d \
./libavutil/random_seed.d \
./libavutil/rational.d \
./libavutil/rc4.d \
./libavutil/samplefmt.d \
./libavutil/sha.d \
./libavutil/softfloat.d \
./libavutil/timecode.d \
./libavutil/tree.d \
./libavutil/utils.d 


# Each subdirectory must supply rules for building sources it contributes
libavutil/%.o: ../libavutil/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


