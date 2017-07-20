################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include lwip-2.0.2/test/linux/subdir.mk
-include lwip-2.0.2/src/netif/subdir.mk
-include lwip-2.0.2/src/core/ipv6/subdir.mk
-include lwip-2.0.2/src/core/ipv4/subdir.mk
-include lwip-2.0.2/src/core/subdir.mk
-include lwip-2.0.2/src/arch/subdir.mk
-include lwip-2.0.2/src/apps/tftp/subdir.mk
-include lwip-2.0.2/src/apps/sntp/subdir.mk
-include lwip-2.0.2/src/apps/snmp/subdir.mk
-include lwip-2.0.2/src/apps/netbiosns/subdir.mk
-include lwip-2.0.2/src/apps/mqtt/subdir.mk
-include lwip-2.0.2/src/apps/mdns/subdir.mk
-include lwip-2.0.2/src/apps/lwiperf/subdir.mk
-include lwip-2.0.2/src/apps/httpd/subdir.mk
-include lwip-2.0.2/src/api/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: lwip-linux

# Tool invocations
lwip-linux: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross GCC Linker'
	gcc  -o "lwip-linux" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) lwip-linux
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
