################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip-2.0.2/src/core/ipv4/autoip.c \
../lwip-2.0.2/src/core/ipv4/dhcp.c \
../lwip-2.0.2/src/core/ipv4/etharp.c \
../lwip-2.0.2/src/core/ipv4/icmp.c \
../lwip-2.0.2/src/core/ipv4/igmp.c \
../lwip-2.0.2/src/core/ipv4/ip4.c \
../lwip-2.0.2/src/core/ipv4/ip4_addr.c \
../lwip-2.0.2/src/core/ipv4/ip4_frag.c 

OBJS += \
./lwip-2.0.2/src/core/ipv4/autoip.o \
./lwip-2.0.2/src/core/ipv4/dhcp.o \
./lwip-2.0.2/src/core/ipv4/etharp.o \
./lwip-2.0.2/src/core/ipv4/icmp.o \
./lwip-2.0.2/src/core/ipv4/igmp.o \
./lwip-2.0.2/src/core/ipv4/ip4.o \
./lwip-2.0.2/src/core/ipv4/ip4_addr.o \
./lwip-2.0.2/src/core/ipv4/ip4_frag.o 

C_DEPS += \
./lwip-2.0.2/src/core/ipv4/autoip.d \
./lwip-2.0.2/src/core/ipv4/dhcp.d \
./lwip-2.0.2/src/core/ipv4/etharp.d \
./lwip-2.0.2/src/core/ipv4/icmp.d \
./lwip-2.0.2/src/core/ipv4/igmp.d \
./lwip-2.0.2/src/core/ipv4/ip4.d \
./lwip-2.0.2/src/core/ipv4/ip4_addr.d \
./lwip-2.0.2/src/core/ipv4/ip4_frag.d 


# Each subdirectory must supply rules for building sources it contributes
lwip-2.0.2/src/core/ipv4/%.o: ../lwip-2.0.2/src/core/ipv4/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I../lwip-2.0.2/src/include -I../lwip-2.0.2/src -I../lwip-2.0.2/test/linux -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


