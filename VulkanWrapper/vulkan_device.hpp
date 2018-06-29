/*
 * VulkanInstance.hpp
 *
 *  Created on: Jun 20, 2018
 *      Author: tobi
 */

#ifndef TOBIVULKAN_VULKANWRAPPER_VULKANINSTANCE_HPP_
#define TOBIVULKAN_VULKANWRAPPER_VULKANINSTANCE_HPP_

#include <vulkan/vulkan.hpp>
#include <cstdio>
#include <memory>
#include <vector>
#include "vulkan_init_util.hpp"
#include "vulkan_validation.hpp"

namespace tobivulkan
{

/** @brief Contains queue family indices */
struct queue_family_indices
{
  uint32_t graphics;
  uint32_t compute;
  uint32_t transfer;
};

// currently only support for one device at a time. that should be appropriate, but we'll see
class vulkan_device
{
 public:
  // TODO: might change it to take in a settings object?
  vulkan_device(
      const char* application_name,
      VkQueueFlags requested_queue_types = VK_QUEUE_GRAPHICS_BIT
          | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT);

  vulkan_device(const vulkan_device& other) = delete;
  vulkan_device(vulkan_device&& other);
  vulkan_device& operator=(const vulkan_device&) = delete;
  vulkan_device& operator=(vulkan_device&& other);
  ~vulkan_device();

  VkInstance get_instance()
  {
    return instance;
  }

  VkDevice get_device()
  {
    return device;
  }

  VkPhysicalDevice get_physical_device()
  {
    return physical_device;
  }

  queue_family_indices get_queue_family_indices()
  {
    return queue_family_ids;
  }

  VkQueue get_graphics_queue(){return graphics_queue;}

 private:

  void initialize();

  // create instance, enable validation and extensions
  void create_instance();

  void enumerate_devices();

  void create_device();

  void check_extension_support();

  std::vector<const char*> get_required_instance_extensions();

  const char* application_name;
  VkQueueFlags requested_queue_types;
  VkInstance instance;
  VkPhysicalDevice physical_device;
  VkDevice device;
  uint32_t queue_family_count;
  VkQueue graphics_queue;
  VkQueue compute_queue;
  VkQueue transfer_queue;
  queue_family_indices queue_family_ids;

  std::unique_ptr<vulkan_validation> validation;
};

}  // namespace tobivulkan

#endif // TOBIVULKAN_VULKANWRAPPER_VULKANINSTANCE_HPP_
