################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip-2.0.2/src/core/def.c \
../lwip-2.0.2/src/core/dns.c \
../lwip-2.0.2/src/core/inet_chksum.c \
../lwip-2.0.2/src/core/init.c \
../lwip-2.0.2/src/core/ip.c \
../lwip-2.0.2/src/core/mem.c \
../lwip-2.0.2/src/core/memp.c \
../lwip-2.0.2/src/core/netif.c \
../lwip-2.0.2/src/core/pbuf.c \
../lwip-2.0.2/src/core/raw.c \
../lwip-2.0.2/src/core/stats.c \
../lwip-2.0.2/src/core/sys.c \
../lwip-2.0.2/src/core/tcp.c \
../lwip-2.0.2/src/core/tcp_in.c \
../lwip-2.0.2/src/core/tcp_out.c \
../lwip-2.0.2/src/core/timeouts.c \
../lwip-2.0.2/src/core/udp.c 

OBJS += \
./lwip-2.0.2/src/core/def.o \
./lwip-2.0.2/src/core/dns.o \
./lwip-2.0.2/src/core/inet_chksum.o \
./lwip-2.0.2/src/core/init.o \
./lwip-2.0.2/src/core/ip.o \
./lwip-2.0.2/src/core/mem.o \
./lwip-2.0.2/src/core/memp.o \
./lwip-2.0.2/src/core/netif.o \
./lwip-2.0.2/src/core/pbuf.o \
./lwip-2.0.2/src/core/raw.o \
./lwip-2.0.2/src/core/stats.o \
./lwip-2.0.2/src/core/sys.o \
./lwip-2.0.2/src/core/tcp.o \
./lwip-2.0.2/src/core/tcp_in.o \
./lwip-2.0.2/src/core/tcp_out.o \
./lwip-2.0.2/src/core/timeouts.o \
./lwip-2.0.2/src/core/udp.o 

C_DEPS += \
./lwip-2.0.2/src/core/def.d \
./lwip-2.0.2/src/core/dns.d \
./lwip-2.0.2/src/core/inet_chksum.d \
./lwip-2.0.2/src/core/init.d \
./lwip-2.0.2/src/core/ip.d \
./lwip-2.0.2/src/core/mem.d \
./lwip-2.0.2/src/core/memp.d \
./lwip-2.0.2/src/core/netif.d \
./lwip-2.0.2/src/core/pbuf.d \
./lwip-2.0.2/src/core/raw.d \
./lwip-2.0.2/src/core/stats.d \
./lwip-2.0.2/src/core/sys.d \
./lwip-2.0.2/src/core/tcp.d \
./lwip-2.0.2/src/core/tcp_in.d \
./lwip-2.0.2/src/core/tcp_out.d \
./lwip-2.0.2/src/core/timeouts.d \
./lwip-2.0.2/src/core/udp.d 


# Each subdirectory must supply rules for building sources it contributes
lwip-2.0.2/src/core/%.o: ../lwip-2.0.2/src/core/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I../lwip-2.0.2/src/include -I../lwip-2.0.2/src -I../lwip-2.0.2/test/linux -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


