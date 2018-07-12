################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../externals/glfw/tests/clipboard.c \
../externals/glfw/tests/cursor.c \
../externals/glfw/tests/empty.c \
../externals/glfw/tests/events.c \
../externals/glfw/tests/gamma.c \
../externals/glfw/tests/glfwinfo.c \
../externals/glfw/tests/icon.c \
../externals/glfw/tests/iconify.c \
../externals/glfw/tests/inputlag.c \
../externals/glfw/tests/joysticks.c \
../externals/glfw/tests/monitors.c \
../externals/glfw/tests/msaa.c \
../externals/glfw/tests/opacity.c \
../externals/glfw/tests/reopen.c \
../externals/glfw/tests/tearing.c \
../externals/glfw/tests/threads.c \
../externals/glfw/tests/timeout.c \
../externals/glfw/tests/title.c \
../externals/glfw/tests/vulkan.c \
../externals/glfw/tests/windows.c 

OBJS += \
./externals/glfw/tests/clipboard.o \
./externals/glfw/tests/cursor.o \
./externals/glfw/tests/empty.o \
./externals/glfw/tests/events.o \
./externals/glfw/tests/gamma.o \
./externals/glfw/tests/glfwinfo.o \
./externals/glfw/tests/icon.o \
./externals/glfw/tests/iconify.o \
./externals/glfw/tests/inputlag.o \
./externals/glfw/tests/joysticks.o \
./externals/glfw/tests/monitors.o \
./externals/glfw/tests/msaa.o \
./externals/glfw/tests/opacity.o \
./externals/glfw/tests/reopen.o \
./externals/glfw/tests/tearing.o \
./externals/glfw/tests/threads.o \
./externals/glfw/tests/timeout.o \
./externals/glfw/tests/title.o \
./externals/glfw/tests/vulkan.o \
./externals/glfw/tests/windows.o 

C_DEPS += \
./externals/glfw/tests/clipboard.d \
./externals/glfw/tests/cursor.d \
./externals/glfw/tests/empty.d \
./externals/glfw/tests/events.d \
./externals/glfw/tests/gamma.d \
./externals/glfw/tests/glfwinfo.d \
./externals/glfw/tests/icon.d \
./externals/glfw/tests/iconify.d \
./externals/glfw/tests/inputlag.d \
./externals/glfw/tests/joysticks.d \
./externals/glfw/tests/monitors.d \
./externals/glfw/tests/msaa.d \
./externals/glfw/tests/opacity.d \
./externals/glfw/tests/reopen.d \
./externals/glfw/tests/tearing.d \
./externals/glfw/tests/threads.d \
./externals/glfw/tests/timeout.d \
./externals/glfw/tests/title.d \
./externals/glfw/tests/vulkan.d \
./externals/glfw/tests/windows.d 


# Each subdirectory must supply rules for building sources it contributes
externals/glfw/tests/%.o: ../externals/glfw/tests/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -std=c11 -I/home/admin/Programming/VulkanSDK/1.1.73.0/x86_64/include -I/home/admin/workspace/libs/glm -I/home/admin/workspace/libs/stb -I"/home/admin/workspace/vulkan-tutorial/externals/glfw/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


