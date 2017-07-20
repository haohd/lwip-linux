################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip-2.0.2/src/core/ipv6/dhcp6.c \
../lwip-2.0.2/src/core/ipv6/ethip6.c \
../lwip-2.0.2/src/core/ipv6/icmp6.c \
../lwip-2.0.2/src/core/ipv6/inet6.c \
../lwip-2.0.2/src/core/ipv6/ip6.c \
../lwip-2.0.2/src/core/ipv6/ip6_addr.c \
../lwip-2.0.2/src/core/ipv6/ip6_frag.c \
../lwip-2.0.2/src/core/ipv6/mld6.c \
../lwip-2.0.2/src/core/ipv6/nd6.c 

OBJS += \
./lwip-2.0.2/src/core/ipv6/dhcp6.o \
./lwip-2.0.2/src/core/ipv6/ethip6.o \
./lwip-2.0.2/src/core/ipv6/icmp6.o \
./lwip-2.0.2/src/core/ipv6/inet6.o \
./lwip-2.0.2/src/core/ipv6/ip6.o \
./lwip-2.0.2/src/core/ipv6/ip6_addr.o \
./lwip-2.0.2/src/core/ipv6/ip6_frag.o \
./lwip-2.0.2/src/core/ipv6/mld6.o \
./lwip-2.0.2/src/core/ipv6/nd6.o 

C_DEPS += \
./lwip-2.0.2/src/core/ipv6/dhcp6.d \
./lwip-2.0.2/src/core/ipv6/ethip6.d \
./lwip-2.0.2/src/core/ipv6/icmp6.d \
./lwip-2.0.2/src/core/ipv6/inet6.d \
./lwip-2.0.2/src/core/ipv6/ip6.d \
./lwip-2.0.2/src/core/ipv6/ip6_addr.d \
./lwip-2.0.2/src/core/ipv6/ip6_frag.d \
./lwip-2.0.2/src/core/ipv6/mld6.d \
./lwip-2.0.2/src/core/ipv6/nd6.d 


# Each subdirectory must supply rules for building sources it contributes
lwip-2.0.2/src/core/ipv6/%.o: ../lwip-2.0.2/src/core/ipv6/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I../lwip-2.0.2/src/include -I../lwip-2.0.2/src -I../lwip-2.0.2/test/linux -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


