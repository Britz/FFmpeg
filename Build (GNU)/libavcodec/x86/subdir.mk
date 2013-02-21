################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../libavcodec/x86/ac3dsp.o \
../libavcodec/x86/ac3dsp_mmx.o \
../libavcodec/x86/cavsdsp_mmx.o \
../libavcodec/x86/dct32_sse.o \
../libavcodec/x86/deinterlace.o \
../libavcodec/x86/diracdsp_mmx.o \
../libavcodec/x86/diracdsp_yasm.o \
../libavcodec/x86/dnxhd_mmx.o \
../libavcodec/x86/dsputil_mmx.o \
../libavcodec/x86/dsputil_yasm.o \
../libavcodec/x86/dsputilenc_mmx.o \
../libavcodec/x86/dsputilenc_yasm.o \
../libavcodec/x86/dwt.o \
../libavcodec/x86/dwt_yasm.o \
../libavcodec/x86/fdct_mmx.o \
../libavcodec/x86/fft.o \
../libavcodec/x86/fft_3dn.o \
../libavcodec/x86/fft_3dn2.o \
../libavcodec/x86/fft_mmx.o \
../libavcodec/x86/fft_sse.o \
../libavcodec/x86/fmtconvert.o \
../libavcodec/x86/fmtconvert_mmx.o \
../libavcodec/x86/h264_chromamc.o \
../libavcodec/x86/h264_chromamc_10bit.o \
../libavcodec/x86/h264_deblock.o \
../libavcodec/x86/h264_deblock_10bit.o \
../libavcodec/x86/h264_idct.o \
../libavcodec/x86/h264_idct_10bit.o \
../libavcodec/x86/h264_intrapred.o \
../libavcodec/x86/h264_intrapred_10bit.o \
../libavcodec/x86/h264_intrapred_init.o \
../libavcodec/x86/h264_qpel_10bit.o \
../libavcodec/x86/h264_weight.o \
../libavcodec/x86/h264_weight_10bit.o \
../libavcodec/x86/h264dsp_mmx.o \
../libavcodec/x86/idct_mmx_xvid.o \
../libavcodec/x86/idct_sse2_xvid.o \
../libavcodec/x86/imdct36_sse.o \
../libavcodec/x86/lpc_mmx.o \
../libavcodec/x86/mlpdsp.o \
../libavcodec/x86/motion_est_mmx.o \
../libavcodec/x86/mpegaudiodec_mmx.o \
../libavcodec/x86/mpegvideo_mmx.o \
../libavcodec/x86/pngdsp-init.o \
../libavcodec/x86/pngdsp.o \
../libavcodec/x86/proresdsp-init.o \
../libavcodec/x86/proresdsp.o \
../libavcodec/x86/rv34dsp.o \
../libavcodec/x86/rv34dsp_init.o \
../libavcodec/x86/rv40dsp.o \
../libavcodec/x86/rv40dsp_init.o \
../libavcodec/x86/sbrdsp.o \
../libavcodec/x86/sbrdsp_init.o \
../libavcodec/x86/simple_idct_mmx.o \
../libavcodec/x86/snowdsp_mmx.o \
../libavcodec/x86/v210-init.o \
../libavcodec/x86/v210.o \
../libavcodec/x86/vc1dsp_mmx.o \
../libavcodec/x86/vc1dsp_yasm.o \
../libavcodec/x86/vp3dsp.o \
../libavcodec/x86/vp56dsp.o \
../libavcodec/x86/vp56dsp_init.o \
../libavcodec/x86/vp8dsp-init.o \
../libavcodec/x86/vp8dsp.o 

C_SRCS += \
../libavcodec/x86/ac3dsp_mmx.c \
../libavcodec/x86/cavsdsp_mmx.c \
../libavcodec/x86/diracdsp_mmx.c \
../libavcodec/x86/dnxhd_mmx.c \
../libavcodec/x86/dsputil_mmx.c \
../libavcodec/x86/dsputil_mmx_avg_template.c \
../libavcodec/x86/dsputil_mmx_qns_template.c \
../libavcodec/x86/dsputil_mmx_rnd_template.c \
../libavcodec/x86/dsputilenc_mmx.c \
../libavcodec/x86/dwt.c \
../libavcodec/x86/fdct_mmx.c \
../libavcodec/x86/fft.c \
../libavcodec/x86/fft_3dn.c \
../libavcodec/x86/fft_3dn2.c \
../libavcodec/x86/fft_sse.c \
../libavcodec/x86/fmtconvert_mmx.c \
../libavcodec/x86/h264_intrapred_init.c \
../libavcodec/x86/h264_qpel_mmx.c \
../libavcodec/x86/h264dsp_mmx.c \
../libavcodec/x86/idct_mmx.c \
../libavcodec/x86/idct_mmx_xvid.c \
../libavcodec/x86/idct_sse2_xvid.c \
../libavcodec/x86/lpc_mmx.c \
../libavcodec/x86/mlpdsp.c \
../libavcodec/x86/motion_est_mmx.c \
../libavcodec/x86/mpegaudiodec_mmx.c \
../libavcodec/x86/mpegvideo_mmx.c \
../libavcodec/x86/mpegvideo_mmx_template.c \
../libavcodec/x86/pngdsp-init.c \
../libavcodec/x86/proresdsp-init.c \
../libavcodec/x86/rv34dsp_init.c \
../libavcodec/x86/rv40dsp_init.c \
../libavcodec/x86/sbrdsp_init.c \
../libavcodec/x86/simple_idct_mmx.c \
../libavcodec/x86/snowdsp_mmx.c \
../libavcodec/x86/v210-init.c \
../libavcodec/x86/vc1dsp_mmx.c \
../libavcodec/x86/vp56dsp_init.c \
../libavcodec/x86/vp8dsp-init.c \
../libavcodec/x86/w64xmmtest.c 

ASM_SRCS += \
../libavcodec/x86/ac3dsp.asm \
../libavcodec/x86/dct32_sse.asm \
../libavcodec/x86/deinterlace.asm \
../libavcodec/x86/diracdsp_yasm.asm \
../libavcodec/x86/dsputil_yasm.asm \
../libavcodec/x86/dsputilenc_yasm.asm \
../libavcodec/x86/dwt_yasm.asm \
../libavcodec/x86/fft_mmx.asm \
../libavcodec/x86/fmtconvert.asm \
../libavcodec/x86/h264_chromamc.asm \
../libavcodec/x86/h264_chromamc_10bit.asm \
../libavcodec/x86/h264_deblock.asm \
../libavcodec/x86/h264_deblock_10bit.asm \
../libavcodec/x86/h264_idct.asm \
../libavcodec/x86/h264_idct_10bit.asm \
../libavcodec/x86/h264_intrapred.asm \
../libavcodec/x86/h264_intrapred_10bit.asm \
../libavcodec/x86/h264_qpel_10bit.asm \
../libavcodec/x86/h264_weight.asm \
../libavcodec/x86/h264_weight_10bit.asm \
../libavcodec/x86/imdct36_sse.asm \
../libavcodec/x86/pngdsp.asm \
../libavcodec/x86/proresdsp.asm \
../libavcodec/x86/rv34dsp.asm \
../libavcodec/x86/rv40dsp.asm \
../libavcodec/x86/sbrdsp.asm \
../libavcodec/x86/v210.asm \
../libavcodec/x86/vc1dsp_yasm.asm \
../libavcodec/x86/vp3dsp.asm \
../libavcodec/x86/vp56dsp.asm \
../libavcodec/x86/vp8dsp.asm 

OBJS += \
./libavcodec/x86/ac3dsp.o \
./libavcodec/x86/ac3dsp_mmx.o \
./libavcodec/x86/cavsdsp_mmx.o \
./libavcodec/x86/dct32_sse.o \
./libavcodec/x86/deinterlace.o \
./libavcodec/x86/diracdsp_mmx.o \
./libavcodec/x86/diracdsp_yasm.o \
./libavcodec/x86/dnxhd_mmx.o \
./libavcodec/x86/dsputil_mmx.o \
./libavcodec/x86/dsputil_mmx_avg_template.o \
./libavcodec/x86/dsputil_mmx_qns_template.o \
./libavcodec/x86/dsputil_mmx_rnd_template.o \
./libavcodec/x86/dsputil_yasm.o \
./libavcodec/x86/dsputilenc_mmx.o \
./libavcodec/x86/dsputilenc_yasm.o \
./libavcodec/x86/dwt.o \
./libavcodec/x86/dwt_yasm.o \
./libavcodec/x86/fdct_mmx.o \
./libavcodec/x86/fft.o \
./libavcodec/x86/fft_3dn.o \
./libavcodec/x86/fft_3dn2.o \
./libavcodec/x86/fft_mmx.o \
./libavcodec/x86/fft_sse.o \
./libavcodec/x86/fmtconvert.o \
./libavcodec/x86/fmtconvert_mmx.o \
./libavcodec/x86/h264_chromamc.o \
./libavcodec/x86/h264_chromamc_10bit.o \
./libavcodec/x86/h264_deblock.o \
./libavcodec/x86/h264_deblock_10bit.o \
./libavcodec/x86/h264_idct.o \
./libavcodec/x86/h264_idct_10bit.o \
./libavcodec/x86/h264_intrapred.o \
./libavcodec/x86/h264_intrapred_10bit.o \
./libavcodec/x86/h264_intrapred_init.o \
./libavcodec/x86/h264_qpel_10bit.o \
./libavcodec/x86/h264_qpel_mmx.o \
./libavcodec/x86/h264_weight.o \
./libavcodec/x86/h264_weight_10bit.o \
./libavcodec/x86/h264dsp_mmx.o \
./libavcodec/x86/idct_mmx.o \
./libavcodec/x86/idct_mmx_xvid.o \
./libavcodec/x86/idct_sse2_xvid.o \
./libavcodec/x86/imdct36_sse.o \
./libavcodec/x86/lpc_mmx.o \
./libavcodec/x86/mlpdsp.o \
./libavcodec/x86/motion_est_mmx.o \
./libavcodec/x86/mpegaudiodec_mmx.o \
./libavcodec/x86/mpegvideo_mmx.o \
./libavcodec/x86/mpegvideo_mmx_template.o \
./libavcodec/x86/pngdsp-init.o \
./libavcodec/x86/pngdsp.o \
./libavcodec/x86/proresdsp-init.o \
./libavcodec/x86/proresdsp.o \
./libavcodec/x86/rv34dsp.o \
./libavcodec/x86/rv34dsp_init.o \
./libavcodec/x86/rv40dsp.o \
./libavcodec/x86/rv40dsp_init.o \
./libavcodec/x86/sbrdsp.o \
./libavcodec/x86/sbrdsp_init.o \
./libavcodec/x86/simple_idct_mmx.o \
./libavcodec/x86/snowdsp_mmx.o \
./libavcodec/x86/v210-init.o \
./libavcodec/x86/v210.o \
./libavcodec/x86/vc1dsp_mmx.o \
./libavcodec/x86/vc1dsp_yasm.o \
./libavcodec/x86/vp3dsp.o \
./libavcodec/x86/vp56dsp.o \
./libavcodec/x86/vp56dsp_init.o \
./libavcodec/x86/vp8dsp-init.o \
./libavcodec/x86/vp8dsp.o \
./libavcodec/x86/w64xmmtest.o 

C_DEPS += \
./libavcodec/x86/ac3dsp_mmx.d \
./libavcodec/x86/cavsdsp_mmx.d \
./libavcodec/x86/diracdsp_mmx.d \
./libavcodec/x86/dnxhd_mmx.d \
./libavcodec/x86/dsputil_mmx.d \
./libavcodec/x86/dsputil_mmx_avg_template.d \
./libavcodec/x86/dsputil_mmx_qns_template.d \
./libavcodec/x86/dsputil_mmx_rnd_template.d \
./libavcodec/x86/dsputilenc_mmx.d \
./libavcodec/x86/dwt.d \
./libavcodec/x86/fdct_mmx.d \
./libavcodec/x86/fft.d \
./libavcodec/x86/fft_3dn.d \
./libavcodec/x86/fft_3dn2.d \
./libavcodec/x86/fft_sse.d \
./libavcodec/x86/fmtconvert_mmx.d \
./libavcodec/x86/h264_intrapred_init.d \
./libavcodec/x86/h264_qpel_mmx.d \
./libavcodec/x86/h264dsp_mmx.d \
./libavcodec/x86/idct_mmx.d \
./libavcodec/x86/idct_mmx_xvid.d \
./libavcodec/x86/idct_sse2_xvid.d \
./libavcodec/x86/lpc_mmx.d \
./libavcodec/x86/mlpdsp.d \
./libavcodec/x86/motion_est_mmx.d \
./libavcodec/x86/mpegaudiodec_mmx.d \
./libavcodec/x86/mpegvideo_mmx.d \
./libavcodec/x86/mpegvideo_mmx_template.d \
./libavcodec/x86/pngdsp-init.d \
./libavcodec/x86/proresdsp-init.d \
./libavcodec/x86/rv34dsp_init.d \
./libavcodec/x86/rv40dsp_init.d \
./libavcodec/x86/sbrdsp_init.d \
./libavcodec/x86/simple_idct_mmx.d \
./libavcodec/x86/snowdsp_mmx.d \
./libavcodec/x86/v210-init.d \
./libavcodec/x86/vc1dsp_mmx.d \
./libavcodec/x86/vp56dsp_init.d \
./libavcodec/x86/vp8dsp-init.d \
./libavcodec/x86/w64xmmtest.d 


# Each subdirectory must supply rules for building sources it contributes
libavcodec/x86/%.o: ../libavcodec/x86/%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: GCC Assembler'
	as  -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

libavcodec/x86/%.o: ../libavcodec/x86/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


