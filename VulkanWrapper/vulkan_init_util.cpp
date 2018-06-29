/*
 * vulkan_init_util.cpp
 *
 *  Created on: Jun 25, 2018
 *      Author: admin
 */

#include "vulkan_init_util.hpp"

#include <fstream>

namespace tobivulkan
{
namespace initialisers
{

VkApplicationInfo init_application_info()
{
  VkApplicationInfo application_info =
  { };
  application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  application_info.pApplicationName = "Tobi Vulkan Engine";
  application_info.applicationVersion = 1;
  application_info.pEngineName = "Tobi Vulkan Engine";
  application_info.engineVersion = 1;
  application_info.apiVersion = VK_API_VERSION_1_1;

  return application_info;
}

VkInstanceCreateInfo init_instance_create_info()
{

  VkInstanceCreateInfo instance_create_info =
  { };
  instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

  return instance_create_info;
}

VkDeviceCreateInfo init_device_create_info()
{
  VkDeviceCreateInfo device_info =
  { };
  device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

  return device_info;
}

VkSwapchainCreateInfoKHR init_swap_chain_create_info()
{
  VkSwapchainCreateInfoKHR swap_chain_info =
  { };
  swap_chain_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  swap_chain_info.imageArrayLayers = 1;
  swap_chain_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  swap_chain_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

  return swap_chain_info;
}

VkImageViewCreateInfo init_image_view_2d_create_info()
{
  VkImageViewCreateInfo image_view_create_info =
  { };
  image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
  image_view_create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
  image_view_create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
  image_view_create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
  image_view_create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
  image_view_create_info.subresourceRange.baseMipLevel = 0;
  image_view_create_info.subresourceRange.levelCount = 1;
  image_view_create_info.subresourceRange.baseArrayLayer = 0;
  image_view_create_info.subresourceRange.layerCount = 1;

  return image_view_create_info;
}

VkXcbSurfaceCreateInfoKHR init_xcb_surface_create_info()
{
  VkXcbSurfaceCreateInfoKHR surface_create_info =
  { };
  surface_create_info.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
  surface_create_info.pNext = VK_NULL_HANDLE;

  return surface_create_info;
}

VkShaderModuleCreateInfo init_shader_module_create_info()
{
  VkShaderModuleCreateInfo create_info =
  { };
  create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

  return create_info;
}

VkPipelineShaderStageCreateInfo init_pipeline_shader_stage_create_info()
{
  VkPipelineShaderStageCreateInfo create_info =
  { };
  create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  create_info.pName = "main";

  return create_info;
}

VkPipelineVertexInputStateCreateInfo init_vertex_input_create_info()
{
  VkPipelineVertexInputStateCreateInfo vertex_input_info =
  { };
  vertex_input_info.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

  return vertex_input_info;
}

VkSemaphoreCreateInfo init_semafore_create_info()
{
  VkSemaphoreCreateInfo semaphore_info =
  { };
  semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  return semaphore_info;
}

}  // namespace initializers

namespace util
{

std::vector<const char*> get_required_device_extensions()
{
  std::vector<const char*> device_extension_names =
  { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

  return device_extension_names;
}

bool check_device_extension_support(VkPhysicalDevice device)
{
  uint32_t extension_count;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count,
                                       nullptr);

  std::vector<VkExtensionProperties> available_extensions(extension_count);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count,
                                       available_extensions.data());

  auto required_device_extensions = get_required_device_extensions();
  std::set<std::string> required_extensions_set(
      required_device_extensions.begin(), required_device_extensions.end());

  for (const auto& extension : available_extensions)
  {
    required_extensions_set.erase(extension.extensionName);
  }

  return required_extensions_set.empty();
}

/**
 * Get the index of a queue family that supports the requested queue flags
 *
 * @param queueFlags Queue flags to find a queue family index for
 *
 * @return Index of the queue family index that matches the flags
 *
 * @throw Throws an exception if no queue family index could be found that
 * supports the requested flags
 */
uint32_t get_queue_family_index(
    std::vector<VkQueueFamilyProperties> queue_family_properties,
    VkQueueFlagBits queue_flags)
{
  // Dedicated queue for compute
  // Try to find a queue family index that supports compute but not graphics
  if (queue_flags & VK_QUEUE_COMPUTE_BIT)
  {
    for (uint32_t i = 0;
        i < static_cast<uint32_t>(queue_family_properties.size()); i++)
    {
      if ((queue_family_properties[i].queueFlags & queue_flags)
          && ((queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
              == 0))
      {
        return i;
        break;
      }
    }
  }

  // Dedicated queue for transfer
  // Try to find a queue family index that supports transfer but not graphics and compute
  if (queue_flags & VK_QUEUE_TRANSFER_BIT)
  {
    for (uint32_t i = 0;
        i < static_cast<uint32_t>(queue_family_properties.size()); i++)
    {
      if ((queue_family_properties[i].queueFlags & queue_flags)
          && ((queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
              == 0)
          && ((queue_family_properties[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
              == 0))
      {
        return i;
        break;
      }
    }
  }

  // For other queue types or if no separate compute queue is present, return the
  // first one to support the requested flags
  for (uint32_t i = 0;
      i < static_cast<uint32_t>(queue_family_properties.size()); i++)
  {
    if (queue_family_properties[i].queueFlags & queue_flags)
    {
      return i;
      break;
    }
  }

  throw std::runtime_error("Could not find a matching queue family index");
}

std::vector<char> read_file(std::string file_name)
{
  std::ifstream file(file_name, std::ios::ate | std::ios::binary);

  if (!file.is_open())
  {
    throw std::runtime_error("failed to open file!");
  }

  size_t fileSize = (size_t) file.tellg();
  std::vector<char> buffer(fileSize);

  file.seekg(0);
  file.read(buffer.data(), fileSize);

  file.close();

  return buffer;
}

}  // namespace util
}  // namespace tobivulkan
