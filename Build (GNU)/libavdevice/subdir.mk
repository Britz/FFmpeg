################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../libavdevice/alldevices.o \
../libavdevice/alsa-audio-common.o \
../libavdevice/alsa-audio-dec.o \
../libavdevice/alsa-audio-enc.o \
../libavdevice/avdevice.o \
../libavdevice/dv1394.o \
../libavdevice/fbdev.o \
../libavdevice/jack_audio.o \
../libavdevice/lavfi.o \
../libavdevice/oss_audio.o \
../libavdevice/sdl.o \
../libavdevice/timefilter.o \
../libavdevice/v4l2.o \
../libavdevice/x11grab.o 

C_SRCS += \
../libavdevice/alldevices.c \
../libavdevice/alsa-audio-common.c \
../libavdevice/alsa-audio-dec.c \
../libavdevice/alsa-audio-enc.c \
../libavdevice/avdevice.c \
../libavdevice/bktr.c \
../libavdevice/dshow.c \
../libavdevice/dshow_common.c \
../libavdevice/dshow_enummediatypes.c \
../libavdevice/dshow_enumpins.c \
../libavdevice/dshow_filter.c \
../libavdevice/dshow_pin.c \
../libavdevice/dv1394.c \
../libavdevice/fbdev.c \
../libavdevice/jack_audio.c \
../libavdevice/lavfi.c \
../libavdevice/libcdio.c \
../libavdevice/libdc1394.c \
../libavdevice/openal-dec.c \
../libavdevice/oss_audio.c \
../libavdevice/pulse.c \
../libavdevice/sdl.c \
../libavdevice/sndio_common.c \
../libavdevice/sndio_dec.c \
../libavdevice/sndio_enc.c \
../libavdevice/timefilter.c \
../libavdevice/v4l.c \
../libavdevice/v4l2.c \
../libavdevice/vfwcap.c \
../libavdevice/x11grab.c 

OBJS += \
./libavdevice/alldevices.o \
./libavdevice/alsa-audio-common.o \
./libavdevice/alsa-audio-dec.o \
./libavdevice/alsa-audio-enc.o \
./libavdevice/avdevice.o \
./libavdevice/bktr.o \
./libavdevice/dshow.o \
./libavdevice/dshow_common.o \
./libavdevice/dshow_enummediatypes.o \
./libavdevice/dshow_enumpins.o \
./libavdevice/dshow_filter.o \
./libavdevice/dshow_pin.o \
./libavdevice/dv1394.o \
./libavdevice/fbdev.o \
./libavdevice/jack_audio.o \
./libavdevice/lavfi.o \
./libavdevice/libcdio.o \
./libavdevice/libdc1394.o \
./libavdevice/openal-dec.o \
./libavdevice/oss_audio.o \
./libavdevice/pulse.o \
./libavdevice/sdl.o \
./libavdevice/sndio_common.o \
./libavdevice/sndio_dec.o \
./libavdevice/sndio_enc.o \
./libavdevice/timefilter.o \
./libavdevice/v4l.o \
./libavdevice/v4l2.o \
./libavdevice/vfwcap.o \
./libavdevice/x11grab.o 

C_DEPS += \
./libavdevice/alldevices.d \
./libavdevice/alsa-audio-common.d \
./libavdevice/alsa-audio-dec.d \
./libavdevice/alsa-audio-enc.d \
./libavdevice/avdevice.d \
./libavdevice/bktr.d \
./libavdevice/dshow.d \
./libavdevice/dshow_common.d \
./libavdevice/dshow_enummediatypes.d \
./libavdevice/dshow_enumpins.d \
./libavdevice/dshow_filter.d \
./libavdevice/dshow_pin.d \
./libavdevice/dv1394.d \
./libavdevice/fbdev.d \
./libavdevice/jack_audio.d \
./libavdevice/lavfi.d \
./libavdevice/libcdio.d \
./libavdevice/libdc1394.d \
./libavdevice/openal-dec.d \
./libavdevice/oss_audio.d \
./libavdevice/pulse.d \
./libavdevice/sdl.d \
./libavdevice/sndio_common.d \
./libavdevice/sndio_dec.d \
./libavdevice/sndio_enc.d \
./libavdevice/timefilter.d \
./libavdevice/v4l.d \
./libavdevice/v4l2.d \
./libavdevice/vfwcap.d \
./libavdevice/x11grab.d 


# Each subdirectory must supply rules for building sources it contributes
libavdevice/%.o: ../libavdevice/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


