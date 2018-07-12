################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../externals/glm/test/gtc/gtc_bitfield.cpp \
../externals/glm/test/gtc/gtc_color_space.cpp \
../externals/glm/test/gtc/gtc_constants.cpp \
../externals/glm/test/gtc/gtc_epsilon.cpp \
../externals/glm/test/gtc/gtc_integer.cpp \
../externals/glm/test/gtc/gtc_matrix_access.cpp \
../externals/glm/test/gtc/gtc_matrix_integer.cpp \
../externals/glm/test/gtc/gtc_matrix_inverse.cpp \
../externals/glm/test/gtc/gtc_matrix_transform.cpp \
../externals/glm/test/gtc/gtc_noise.cpp \
../externals/glm/test/gtc/gtc_packing.cpp \
../externals/glm/test/gtc/gtc_quaternion.cpp \
../externals/glm/test/gtc/gtc_random.cpp \
../externals/glm/test/gtc/gtc_reciprocal.cpp \
../externals/glm/test/gtc/gtc_round.cpp \
../externals/glm/test/gtc/gtc_type_aligned.cpp \
../externals/glm/test/gtc/gtc_type_precision.cpp \
../externals/glm/test/gtc/gtc_type_ptr.cpp \
../externals/glm/test/gtc/gtc_ulp.cpp \
../externals/glm/test/gtc/gtc_user_defined_types.cpp \
../externals/glm/test/gtc/gtc_vec1.cpp 

OBJS += \
./externals/glm/test/gtc/gtc_bitfield.o \
./externals/glm/test/gtc/gtc_color_space.o \
./externals/glm/test/gtc/gtc_constants.o \
./externals/glm/test/gtc/gtc_epsilon.o \
./externals/glm/test/gtc/gtc_integer.o \
./externals/glm/test/gtc/gtc_matrix_access.o \
./externals/glm/test/gtc/gtc_matrix_integer.o \
./externals/glm/test/gtc/gtc_matrix_inverse.o \
./externals/glm/test/gtc/gtc_matrix_transform.o \
./externals/glm/test/gtc/gtc_noise.o \
./externals/glm/test/gtc/gtc_packing.o \
./externals/glm/test/gtc/gtc_quaternion.o \
./externals/glm/test/gtc/gtc_random.o \
./externals/glm/test/gtc/gtc_reciprocal.o \
./externals/glm/test/gtc/gtc_round.o \
./externals/glm/test/gtc/gtc_type_aligned.o \
./externals/glm/test/gtc/gtc_type_precision.o \
./externals/glm/test/gtc/gtc_type_ptr.o \
./externals/glm/test/gtc/gtc_ulp.o \
./externals/glm/test/gtc/gtc_user_defined_types.o \
./externals/glm/test/gtc/gtc_vec1.o 

CPP_DEPS += \
./externals/glm/test/gtc/gtc_bitfield.d \
./externals/glm/test/gtc/gtc_color_space.d \
./externals/glm/test/gtc/gtc_constants.d \
./externals/glm/test/gtc/gtc_epsilon.d \
./externals/glm/test/gtc/gtc_integer.d \
./externals/glm/test/gtc/gtc_matrix_access.d \
./externals/glm/test/gtc/gtc_matrix_integer.d \
./externals/glm/test/gtc/gtc_matrix_inverse.d \
./externals/glm/test/gtc/gtc_matrix_transform.d \
./externals/glm/test/gtc/gtc_noise.d \
./externals/glm/test/gtc/gtc_packing.d \
./externals/glm/test/gtc/gtc_quaternion.d \
./externals/glm/test/gtc/gtc_random.d \
./externals/glm/test/gtc/gtc_reciprocal.d \
./externals/glm/test/gtc/gtc_round.d \
./externals/glm/test/gtc/gtc_type_aligned.d \
./externals/glm/test/gtc/gtc_type_precision.d \
./externals/glm/test/gtc/gtc_type_ptr.d \
./externals/glm/test/gtc/gtc_ulp.d \
./externals/glm/test/gtc/gtc_user_defined_types.d \
./externals/glm/test/gtc/gtc_vec1.d 


# Each subdirectory must supply rules for building sources it contributes
externals/glm/test/gtc/%.o: ../externals/glm/test/gtc/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++0x -I/home/admin/workspace/vulkan-tutorial/externals/glm -I/home/admin/workspace/vulkan-tutorial/externals/glfw/include -I/home/admin/workspace/vulkan-tutorial/externals/stb -I/home/admin/Programming/VulkanSDK/1.1.73.0/x86_64/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


