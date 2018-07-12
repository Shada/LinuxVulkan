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

#include "vulkan_physical_device.hpp"

#include <set>

namespace tobi_engine
{
namespace vulkan_wrapper
{

vulkan_physical_device::vulkan_physical_device(
    std::shared_ptr<vulkan_instance> instance,
    std::shared_ptr<vulkan_surface> surface)
    : physical_device(VK_NULL_HANDLE),
      instance(instance),
      surface(surface)
{
  initialize();
}

void vulkan_physical_device::initialize() const
{
  uint32_t device_count = 0;

  vkEnumeratePhysicalDevices(instance->get_instance(), &device_count, nullptr);

  if (device_count == 0)
  {
    throw std::runtime_error("failed to find GPUs with Vulkan support!");
  }

  std::vector<VkPhysicalDevice> devices(device_count);
  vkEnumeratePhysicalDevices(instance->get_instance(), &device_count, devices.data());

  for (const auto& device : devices)
  {
    if (is_device_suitable(device))
    {
      physical_device = device;
      break;
    }
  }

  if (physical_device == VK_NULL_HANDLE)
  {
    throw std::runtime_error("failed to find a suitable GPU!");
  }
}

bool vulkan_physical_device::is_device_suitable(VkPhysicalDevice device) const
{
  physical_device = device;
  auto indices = find_queue_families();

  auto extensions_supported = check_device_extension_support();

  auto swap_chain_adequate = false;
  if (extensions_supported)
  {
    auto swap_chain_support = query_swap_chain_support();
    swap_chain_adequate = !swap_chain_support.formats.empty()
        && !swap_chain_support.present_modes.empty();
  }

  VkPhysicalDeviceFeatures supported_features;
  vkGetPhysicalDeviceFeatures(device, &supported_features);

  physical_device = VK_NULL_HANDLE;

  return indices.is_complete() && extensions_supported && swap_chain_adequate
      && supported_features.samplerAnisotropy;
}

const queue_family_indices vulkan_physical_device::find_queue_families() const
{
  queue_family_indices indices;

  uint32_t queue_family_count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, nullptr);

  std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
  vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_families.data());

  int i = 0;
  for (const auto& queue_family : queue_families)
  {
    if (queue_family.queueCount > 0
        && (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT))
    {
      indices.graphics_family = i;
    }

    VkBool32 present_support = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i,
                                         surface->get_surface(),
                                         &present_support);

    if (queue_family.queueCount > 0 && present_support)
    {
      indices.present_family = i;
    }

    if (indices.is_complete())
    {
      break;
    }

    i++;
  }

  return indices;
}

bool vulkan_physical_device::check_device_extension_support() const
{
  uint32_t extension_count;
  vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extension_count, nullptr);

  std::vector<VkExtensionProperties> available_extensions(extension_count);
  vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extension_count, available_extensions.data());

  std::set<std::string> required_extensions(device_extensions.begin(),
                                           device_extensions.end());

  for (const auto& extension : available_extensions)
  {
    required_extensions.erase(extension.extensionName);
  }

  return required_extensions.empty();
}

const swap_chain_support_details vulkan_physical_device::query_swap_chain_support() const
{
  swap_chain_support_details details;

  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device,
                                            surface->get_surface(),
                                            &details.capabilities);

  uint32_t format_count;
  vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface->get_surface(),
                                       &format_count, nullptr);

  if (format_count != 0)
  {
    details.formats.resize(format_count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device,
                                         surface->get_surface(), &format_count,
                                         details.formats.data());
  }

  uint32_t present_mode_count;
  vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device,
                                            surface->get_surface(),
                                            &present_mode_count, nullptr);

  if (present_mode_count != 0)
  {
    details.present_modes.resize(present_mode_count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device,
                                              surface->get_surface(),
                                              &present_mode_count,
                                              details.present_modes.data());
  }

  return details;
}

}  // namespace vulkan_wrapper
}  // namespace tobi_engine

