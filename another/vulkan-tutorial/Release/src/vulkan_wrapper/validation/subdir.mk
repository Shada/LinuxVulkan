################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/vulkan_wrapper/validation/vulkan_validation_layers.cpp 

OBJS += \
./src/vulkan_wrapper/validation/vulkan_validation_layers.o 

CPP_DEPS += \
./src/vulkan_wrapper/validation/vulkan_validation_layers.d 


# Each subdirectory must supply rules for building sources it contributes
src/vulkan_wrapper/validation/%.o: ../src/vulkan_wrapper/validation/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++1y -DNDEBUG=1 -I/home/admin/workspace/libs/glfw/include -I/home/admin/workspace/libs/stb -I/home/admin/workspace/libs/glm -I/home/admin/Programming/VulkanSDK/1.1.77.0/x86_64/include -O3 -pedantic -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


