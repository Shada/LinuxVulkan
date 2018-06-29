################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../VulkanWrapper/vulkan_device.cpp \
../VulkanWrapper/vulkan_init_util.cpp \
../VulkanWrapper/vulkan_shader_pipeline.cpp \
../VulkanWrapper/vulkan_swap_chain.cpp \
../VulkanWrapper/vulkan_validation.cpp \
../VulkanWrapper/xcb_window_handler.cpp 

OBJS += \
./VulkanWrapper/vulkan_device.o \
./VulkanWrapper/vulkan_init_util.o \
./VulkanWrapper/vulkan_shader_pipeline.o \
./VulkanWrapper/vulkan_swap_chain.o \
./VulkanWrapper/vulkan_validation.o \
./VulkanWrapper/xcb_window_handler.o 

CPP_DEPS += \
./VulkanWrapper/vulkan_device.d \
./VulkanWrapper/vulkan_init_util.d \
./VulkanWrapper/vulkan_shader_pipeline.d \
./VulkanWrapper/vulkan_swap_chain.d \
./VulkanWrapper/vulkan_validation.d \
./VulkanWrapper/xcb_window_handler.d 


# Each subdirectory must supply rules for building sources it contributes
VulkanWrapper/%.o: ../VulkanWrapper/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++1y -DVK_USE_PLATFORM_XCB_KHR=1 -DNDEBUG=1 -I/home/admin/Programming/VulkanSDK/1.1.73.0/x86_64/include -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


