################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/util/file_handler.cpp 

OBJS += \
./src/util/file_handler.o 

CPP_DEPS += \
./src/util/file_handler.d 


# Each subdirectory must supply rules for building sources it contributes
src/util/%.o: ../src/util/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++0x -I/home/admin/workspace/libs/glm -I/home/admin/workspace/libs/stb -I/home/admin/workspace/libs/glfw/include -I/home/admin/Programming/VulkanSDK/1.1.77.0/x86_64/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


