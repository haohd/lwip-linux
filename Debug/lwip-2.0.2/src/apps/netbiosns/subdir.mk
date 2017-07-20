################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip-2.0.2/src/apps/netbiosns/netbiosns.c 

OBJS += \
./lwip-2.0.2/src/apps/netbiosns/netbiosns.o 

C_DEPS += \
./lwip-2.0.2/src/apps/netbiosns/netbiosns.d 


# Each subdirectory must supply rules for building sources it contributes
lwip-2.0.2/src/apps/netbiosns/%.o: ../lwip-2.0.2/src/apps/netbiosns/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I../lwip-2.0.2/src/include -I../lwip-2.0.2/src -I../lwip-2.0.2/test/linux -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


