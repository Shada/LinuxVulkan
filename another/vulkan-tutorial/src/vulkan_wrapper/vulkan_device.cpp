/// Copyright (c) 2018 Tobias Andersson (shada).
///
/// SPDX-License-Identifier: MIT
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to
/// deal in the Software without restriction, including without limitation the
/// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
/// sell copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in all
/// copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.

#include "vulkan_device.hpp"
#include <set>

namespace tobi_engine
{
namespace vulkan_wrapper
{

vulkan_device::vulkan_device(
    std::shared_ptr<vulkan_physical_device> physical_device,
    std::shared_ptr<vulkan_instance> instance)
    : device(VK_NULL_HANDLE),
      graphics_queue(VK_NULL_HANDLE),
      present_queue(VK_NULL_HANDLE),
      physical_device(physical_device),
      instance(instance)
{
  initialize();

}

vulkan_device::~vulkan_device()
{
  vkDestroyDevice(device, nullptr);
}

void vulkan_device::wait_idle() const
{
  vkDeviceWaitIdle(device);
}

void vulkan_device::initialize() const
{
  queue_family_indices indices = physical_device->find_queue_families();

  std::vector<VkDeviceQueueCreateInfo> queue_create_infos;

  std::set<int> unique_queue_families =
  {
      indices.graphics_family,
      indices.present_family
  };

  auto queue_priority = 1.0f;

  for (const auto &queue_family : unique_queue_families)
  {
    VkDeviceQueueCreateInfo queue_create_info = {};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = queue_family;
    queue_create_info.queueCount = 1;
    queue_create_info.pQueuePriorities = &queue_priority;
    queue_create_infos.push_back(queue_create_info);
  }

  VkPhysicalDeviceFeatures device_features = {};
  device_features.samplerAnisotropy = VK_TRUE;

  VkDeviceCreateInfo device_create_info = {};
  device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  device_create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
  device_create_info.pQueueCreateInfos = queue_create_infos.data();
  device_create_info.pEnabledFeatures = &device_features;

  auto extensions = physical_device->get_device_extensions();
  device_create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  device_create_info.ppEnabledExtensionNames = extensions.data();

  auto validation_layers = instance->get_validation_layers();
  device_create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
  device_create_info.ppEnabledLayerNames = validation_layers.data();

  if (vkCreateDevice(physical_device->get_physical_device(), &device_create_info, nullptr, &device)
      != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create logical device!");
  }

  vkGetDeviceQueue(device, indices.graphics_family, 0, &graphics_queue);
  vkGetDeviceQueue(device, indices.present_family, 0, &present_queue);
}

}  // namespace vulkan_wrapper
}  // namespace tobi_engine
