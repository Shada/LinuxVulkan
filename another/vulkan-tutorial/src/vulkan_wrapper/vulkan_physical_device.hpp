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

#ifndef VULKAN_PHYSICAL_DEVICE_HPP_
#define VULKAN_PHYSICAL_DEVICE_HPP_

#include "vulkan_instance.hpp"
#include "vulkan_surface.hpp"

namespace tobi_engine
{
namespace vulkan_wrapper
{

// TODO: move to own file
struct queue_family_indices
{
  int graphics_family = -1;
  int present_family = -1;

  bool is_complete()
  {
    return graphics_family >= 0 && present_family >= 0;
  }
};

struct swap_chain_support_details
{
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> present_modes;
};

///
///
class vulkan_physical_device
{
 public:
  vulkan_physical_device(std::shared_ptr<vulkan_instance> instance,
                         std::shared_ptr<vulkan_surface> surface);
  ~vulkan_physical_device() = default;
  vulkan_physical_device(vulkan_physical_device &&) = delete;
  vulkan_physical_device(const vulkan_physical_device &) = delete;
  vulkan_physical_device &operator=(const vulkan_physical_device &) = delete;
  vulkan_physical_device &operator=(vulkan_physical_device &&) = delete;

  const queue_family_indices find_queue_families() const;

  const swap_chain_support_details query_swap_chain_support() const;

  const std::vector<const char*> get_device_extensions() const
  {
    return device_extensions;
  }

  const VkPhysicalDevice get_physical_device() const
  {
    return physical_device;
  }

 private:

  mutable VkPhysicalDevice physical_device;

  std::shared_ptr<vulkan_instance> instance;
  std::shared_ptr<vulkan_surface> surface;

  const std::vector<const char*> device_extensions =
  {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME
  };

  void initialize() const;

  bool is_device_suitable(VkPhysicalDevice device) const;

  bool check_device_extension_support() const;

};

}  // namespace vulkan_wrapper
}  // namespace tobi_engine

#endif // VULKAN_PHYSICAL_DEVICE_HPP_
