################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip-2.0.2/src/api/api_lib.c \
../lwip-2.0.2/src/api/api_msg.c \
../lwip-2.0.2/src/api/err.c \
../lwip-2.0.2/src/api/netbuf.c \
../lwip-2.0.2/src/api/netdb.c \
../lwip-2.0.2/src/api/netifapi.c \
../lwip-2.0.2/src/api/sockets.c \
../lwip-2.0.2/src/api/tcpip.c 

OBJS += \
./lwip-2.0.2/src/api/api_lib.o \
./lwip-2.0.2/src/api/api_msg.o \
./lwip-2.0.2/src/api/err.o \
./lwip-2.0.2/src/api/netbuf.o \
./lwip-2.0.2/src/api/netdb.o \
./lwip-2.0.2/src/api/netifapi.o \
./lwip-2.0.2/src/api/sockets.o \
./lwip-2.0.2/src/api/tcpip.o 

C_DEPS += \
./lwip-2.0.2/src/api/api_lib.d \
./lwip-2.0.2/src/api/api_msg.d \
./lwip-2.0.2/src/api/err.d \
./lwip-2.0.2/src/api/netbuf.d \
./lwip-2.0.2/src/api/netdb.d \
./lwip-2.0.2/src/api/netifapi.d \
./lwip-2.0.2/src/api/sockets.d \
./lwip-2.0.2/src/api/tcpip.d 


# Each subdirectory must supply rules for building sources it contributes
lwip-2.0.2/src/api/%.o: ../lwip-2.0.2/src/api/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I../lwip-2.0.2/src/include -I../lwip-2.0.2/src -I../lwip-2.0.2/test/linux -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


