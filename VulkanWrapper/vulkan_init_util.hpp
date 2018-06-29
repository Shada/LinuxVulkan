/*
 * vulkan_init_util.hpp
 *
 *  Created on: Jun 21, 2018
 *      Author: admin
 */

#ifndef TOBIVULKAN_VULKANWRAPPER_VULKAN_INIT_UTIL_HPP_
#define TOBIVULKAN_VULKANWRAPPER_VULKAN_INIT_UTIL_HPP_

#include <vulkan/vulkan.hpp>
#include <set>

namespace tobivulkan
{
const int MAX_FRAMES_IN_FLIGHT = 2;
namespace initialisers
{

VkApplicationInfo init_application_info();

VkInstanceCreateInfo init_instance_create_info();

VkDeviceCreateInfo init_device_create_info();

VkSwapchainCreateInfoKHR init_swap_chain_create_info();

VkImageViewCreateInfo init_image_view_2d_create_info();

VkXcbSurfaceCreateInfoKHR init_xcb_surface_create_info();

VkShaderModuleCreateInfo init_shader_module_create_info();

VkPipelineShaderStageCreateInfo init_pipeline_shader_stage_create_info();

VkPipelineVertexInputStateCreateInfo init_vertex_input_create_info();

VkSemaphoreCreateInfo init_semafore_create_info();

}  // namespace initializers

namespace util
{

std::vector<const char*> get_required_instance_extensions();

std::vector<const char*> get_required_device_extensions();

bool check_device_extension_support(VkPhysicalDevice device);

uint32_t get_queue_family_index(
    std::vector<VkQueueFamilyProperties> queue_family_properties,
    VkQueueFlagBits queue_flags);

std::vector<char> read_file(std::string file_name);

}  // namespace util
}  // namespace tobivulkan

#endif /* TOBIVULKAN_VULKANWRAPPER_VULKAN_INIT_UTIL_HPP_ */
