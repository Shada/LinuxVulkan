################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/vulkan_wrapper/vulkan_device.cpp \
../src/vulkan_wrapper/vulkan_framebuffers.cpp \
../src/vulkan_wrapper/vulkan_instance.cpp \
../src/vulkan_wrapper/vulkan_physical_device.cpp \
../src/vulkan_wrapper/vulkan_render_pass.cpp \
../src/vulkan_wrapper/vulkan_surface.cpp \
../src/vulkan_wrapper/vulkan_swap_chain.cpp \
../src/vulkan_wrapper/window_handler.cpp 

OBJS += \
./src/vulkan_wrapper/vulkan_device.o \
./src/vulkan_wrapper/vulkan_framebuffers.o \
./src/vulkan_wrapper/vulkan_instance.o \
./src/vulkan_wrapper/vulkan_physical_device.o \
./src/vulkan_wrapper/vulkan_render_pass.o \
./src/vulkan_wrapper/vulkan_surface.o \
./src/vulkan_wrapper/vulkan_swap_chain.o \
./src/vulkan_wrapper/window_handler.o 

CPP_DEPS += \
./src/vulkan_wrapper/vulkan_device.d \
./src/vulkan_wrapper/vulkan_framebuffers.d \
./src/vulkan_wrapper/vulkan_instance.d \
./src/vulkan_wrapper/vulkan_physical_device.d \
./src/vulkan_wrapper/vulkan_render_pass.d \
./src/vulkan_wrapper/vulkan_surface.d \
./src/vulkan_wrapper/vulkan_swap_chain.d \
./src/vulkan_wrapper/window_handler.d 


# Each subdirectory must supply rules for building sources it contributes
src/vulkan_wrapper/%.o: ../src/vulkan_wrapper/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++0x -I/home/admin/workspace/libs/glm -I/home/admin/workspace/libs/stb -I/home/admin/workspace/libs/glfw/include -I/home/admin/Programming/VulkanSDK/1.1.77.0/x86_64/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


