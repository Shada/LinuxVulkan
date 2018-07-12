################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../externals/glm/glm/detail/dummy.cpp \
../externals/glm/glm/detail/glm.cpp 

OBJS += \
./externals/glm/glm/detail/dummy.o \
./externals/glm/glm/detail/glm.o 

CPP_DEPS += \
./externals/glm/glm/detail/dummy.d \
./externals/glm/glm/detail/glm.d 


# Each subdirectory must supply rules for building sources it contributes
externals/glm/glm/detail/%.o: ../externals/glm/glm/detail/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++0x -I/home/admin/workspace/vulkan-tutorial/externals/glm -I/home/admin/workspace/vulkan-tutorial/externals/glfw/include -I/home/admin/workspace/vulkan-tutorial/externals/stb -I/home/admin/Programming/VulkanSDK/1.1.73.0/x86_64/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


