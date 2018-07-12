################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../externals/glfw/CMakeFiles/3.11.2/CompilerIdC/CMakeCCompilerId.c 

OBJS += \
./externals/glfw/CMakeFiles/3.11.2/CompilerIdC/CMakeCCompilerId.o 

C_DEPS += \
./externals/glfw/CMakeFiles/3.11.2/CompilerIdC/CMakeCCompilerId.d 


# Each subdirectory must supply rules for building sources it contributes
externals/glfw/CMakeFiles/3.11.2/CompilerIdC/%.o: ../externals/glfw/CMakeFiles/3.11.2/CompilerIdC/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -std=c11 -I/home/admin/Programming/VulkanSDK/1.1.73.0/x86_64/include -I/home/admin/workspace/libs/glm -I/home/admin/workspace/libs/stb -I"/home/admin/workspace/vulkan-tutorial/externals/glfw/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


