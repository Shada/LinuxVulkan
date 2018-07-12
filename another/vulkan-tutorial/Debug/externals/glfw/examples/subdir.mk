################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../externals/glfw/examples/boing.c \
../externals/glfw/examples/gears.c \
../externals/glfw/examples/heightmap.c \
../externals/glfw/examples/offscreen.c \
../externals/glfw/examples/particles.c \
../externals/glfw/examples/sharing.c \
../externals/glfw/examples/simple.c \
../externals/glfw/examples/splitview.c \
../externals/glfw/examples/wave.c 

OBJS += \
./externals/glfw/examples/boing.o \
./externals/glfw/examples/gears.o \
./externals/glfw/examples/heightmap.o \
./externals/glfw/examples/offscreen.o \
./externals/glfw/examples/particles.o \
./externals/glfw/examples/sharing.o \
./externals/glfw/examples/simple.o \
./externals/glfw/examples/splitview.o \
./externals/glfw/examples/wave.o 

C_DEPS += \
./externals/glfw/examples/boing.d \
./externals/glfw/examples/gears.d \
./externals/glfw/examples/heightmap.d \
./externals/glfw/examples/offscreen.d \
./externals/glfw/examples/particles.d \
./externals/glfw/examples/sharing.d \
./externals/glfw/examples/simple.d \
./externals/glfw/examples/splitview.d \
./externals/glfw/examples/wave.d 


# Each subdirectory must supply rules for building sources it contributes
externals/glfw/examples/%.o: ../externals/glfw/examples/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -std=c11 -I/home/admin/Programming/VulkanSDK/1.1.73.0/x86_64/include -I/home/admin/workspace/libs/glm -I/home/admin/workspace/libs/stb -I"/home/admin/workspace/vulkan-tutorial/externals/glfw/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


