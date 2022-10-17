################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../lwip-2.0.2/src/arch/if.c \
../lwip-2.0.2/src/arch/netif.c \
../lwip-2.0.2/src/arch/pcap.c

OBJS += \
./lwip-2.0.2/src/arch/if.o \
./lwip-2.0.2/src/arch/netif.o \
./lwip-2.0.2/src/arch/pcap.o

C_DEPS += \
./lwip-2.0.2/src/arch/if.d \
./lwip-2.0.2/src/arch/netif.d


# Each subdirectory must supply rules for building sources it contributes
lwip-2.0.2/src/arch/%.o: ../lwip-2.0.2/src/arch/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I../lwip-2.0.2/src/include -I../lwip-2.0.2/src -I../lwip-2.0.2/test/linux -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


