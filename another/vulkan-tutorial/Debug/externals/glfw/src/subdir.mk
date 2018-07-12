################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../externals/glfw/src/cocoa_time.c \
../externals/glfw/src/context.c \
../externals/glfw/src/egl_context.c \
../externals/glfw/src/glx_context.c \
../externals/glfw/src/init.c \
../externals/glfw/src/input.c \
../externals/glfw/src/linux_joystick.c \
../externals/glfw/src/mir_init.c \
../externals/glfw/src/mir_monitor.c \
../externals/glfw/src/mir_window.c \
../externals/glfw/src/monitor.c \
../externals/glfw/src/null_init.c \
../externals/glfw/src/null_joystick.c \
../externals/glfw/src/null_monitor.c \
../externals/glfw/src/null_window.c \
../externals/glfw/src/osmesa_context.c \
../externals/glfw/src/posix_thread.c \
../externals/glfw/src/posix_time.c \
../externals/glfw/src/vulkan.c \
../externals/glfw/src/wgl_context.c \
../externals/glfw/src/win32_init.c \
../externals/glfw/src/win32_joystick.c \
../externals/glfw/src/win32_monitor.c \
../externals/glfw/src/win32_thread.c \
../externals/glfw/src/win32_time.c \
../externals/glfw/src/win32_window.c \
../externals/glfw/src/window.c \
../externals/glfw/src/wl_init.c \
../externals/glfw/src/wl_monitor.c \
../externals/glfw/src/wl_window.c \
../externals/glfw/src/x11_init.c \
../externals/glfw/src/x11_monitor.c \
../externals/glfw/src/x11_window.c \
../externals/glfw/src/xkb_unicode.c 

OBJS += \
./externals/glfw/src/cocoa_time.o \
./externals/glfw/src/context.o \
./externals/glfw/src/egl_context.o \
./externals/glfw/src/glx_context.o \
./externals/glfw/src/init.o \
./externals/glfw/src/input.o \
./externals/glfw/src/linux_joystick.o \
./externals/glfw/src/mir_init.o \
./externals/glfw/src/mir_monitor.o \
./externals/glfw/src/mir_window.o \
./externals/glfw/src/monitor.o \
./externals/glfw/src/null_init.o \
./externals/glfw/src/null_joystick.o \
./externals/glfw/src/null_monitor.o \
./externals/glfw/src/null_window.o \
./externals/glfw/src/osmesa_context.o \
./externals/glfw/src/posix_thread.o \
./externals/glfw/src/posix_time.o \
./externals/glfw/src/vulkan.o \
./externals/glfw/src/wgl_context.o \
./externals/glfw/src/win32_init.o \
./externals/glfw/src/win32_joystick.o \
./externals/glfw/src/win32_monitor.o \
./externals/glfw/src/win32_thread.o \
./externals/glfw/src/win32_time.o \
./externals/glfw/src/win32_window.o \
./externals/glfw/src/window.o \
./externals/glfw/src/wl_init.o \
./externals/glfw/src/wl_monitor.o \
./externals/glfw/src/wl_window.o \
./externals/glfw/src/x11_init.o \
./externals/glfw/src/x11_monitor.o \
./externals/glfw/src/x11_window.o \
./externals/glfw/src/xkb_unicode.o 

C_DEPS += \
./externals/glfw/src/cocoa_time.d \
./externals/glfw/src/context.d \
./externals/glfw/src/egl_context.d \
./externals/glfw/src/glx_context.d \
./externals/glfw/src/init.d \
./externals/glfw/src/input.d \
./externals/glfw/src/linux_joystick.d \
./externals/glfw/src/mir_init.d \
./externals/glfw/src/mir_monitor.d \
./externals/glfw/src/mir_window.d \
./externals/glfw/src/monitor.d \
./externals/glfw/src/null_init.d \
./externals/glfw/src/null_joystick.d \
./externals/glfw/src/null_monitor.d \
./externals/glfw/src/null_window.d \
./externals/glfw/src/osmesa_context.d \
./externals/glfw/src/posix_thread.d \
./externals/glfw/src/posix_time.d \
./externals/glfw/src/vulkan.d \
./externals/glfw/src/wgl_context.d \
./externals/glfw/src/win32_init.d \
./externals/glfw/src/win32_joystick.d \
./externals/glfw/src/win32_monitor.d \
./externals/glfw/src/win32_thread.d \
./externals/glfw/src/win32_time.d \
./externals/glfw/src/win32_window.d \
./externals/glfw/src/window.d \
./externals/glfw/src/wl_init.d \
./externals/glfw/src/wl_monitor.d \
./externals/glfw/src/wl_window.d \
./externals/glfw/src/x11_init.d \
./externals/glfw/src/x11_monitor.d \
./externals/glfw/src/x11_window.d \
./externals/glfw/src/xkb_unicode.d 


# Each subdirectory must supply rules for building sources it contributes
externals/glfw/src/%.o: ../externals/glfw/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -std=c11 -I/home/admin/Programming/VulkanSDK/1.1.73.0/x86_64/include -I/home/admin/workspace/libs/glm -I/home/admin/workspace/libs/stb -I"/home/admin/workspace/vulkan-tutorial/externals/glfw/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


