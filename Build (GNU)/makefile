################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm

# All of the sources participating in the build are defined here
-include sources.mk
-include tools/subdir.mk
-include tests/subdir.mk
-include mt-work/subdir.mk
-include libswscale/x86/subdir.mk
-include libswscale/sparc/subdir.mk
-include libswscale/ppc/subdir.mk
-include libswscale/bfin/subdir.mk
-include libswscale/subdir.mk
-include libswresample/x86/subdir.mk
-include libswresample/subdir.mk
-include libpostproc/subdir.mk
-include libavutil/x86/subdir.mk
-include libavutil/ppc/subdir.mk
-include libavutil/arm/subdir.mk
-include libavutil/subdir.mk
-include libavresample/x86/subdir.mk
-include libavresample/subdir.mk
-include libavformat/subdir.mk
-include libavfilter/x86/subdir.mk
-include libavfilter/libmpcodecs/subdir.mk
-include libavfilter/subdir.mk
-include libavdevice/subdir.mk
-include libavcodec/x86/subdir.mk
-include libavcodec/sparc/subdir.mk
-include libavcodec/sh4/subdir.mk
-include libavcodec/ppc/subdir.mk
-include libavcodec/mips/subdir.mk
-include libavcodec/bfin/subdir.mk
-include libavcodec/arm/subdir.mk
-include libavcodec/alpha/subdir.mk
-include libavcodec/subdir.mk
-include doc/examples/subdir.mk
-include doc/subdir.mk
-include .metadata/.plugins/org.eclipse.cdt.make.core/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: libffmpeg

# Tool invocations
libffmpeg: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -shared -o "libffmpeg" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(LIBRARIES)$(CPP_DEPS)$(CXX_DEPS)$(C_UPPER_DEPS) libffmpeg
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
