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

#ifndef VULKAN_SWAP_CHAIN_HPP_
#define VULKAN_SWAP_CHAIN_HPP_

#include "window_handler.hpp"
#include "vulkan_device.hpp"
#include "vulkan_physical_device.hpp"
#include "vulkan_surface.hpp"

namespace tobi_engine
{
namespace vulkan_wrapper
{

///
///
class vulkan_swap_chain
{
 public:
  vulkan_swap_chain(std::shared_ptr<window_handler> window,
                    std::shared_ptr<vulkan_device> device,
                    std::shared_ptr<vulkan_physical_device> physical_device,
                    std::shared_ptr<vulkan_surface> surface);
  ~vulkan_swap_chain();
  vulkan_swap_chain(vulkan_swap_chain &&) = delete;
  vulkan_swap_chain(const vulkan_swap_chain &) = delete;
  vulkan_swap_chain &operator=(const vulkan_swap_chain &) = delete;
  vulkan_swap_chain &operator=(vulkan_swap_chain &&) = delete;

  void initialize();

  VkSwapchainKHR get_swap_chain()
  {
    return swap_chain;
  }
  VkFormat get_image_format()
  {
    return image_format;
  }
  VkExtent2D get_extent()
  {
    return extent;
  }

  uint32_t get_num_images()
  {
    return static_cast<uint32_t>(images.size());
  }
  VkImageView get_image_view(uint32_t i)
  {
    return image_views[i];
  }

 private:

  VkSwapchainKHR swap_chain;
  VkExtent2D extent;
  VkFormat image_format;
  std::vector<VkImage> images;
  std::vector<VkImageView> image_views;

  std::shared_ptr<window_handler> window;
  std::shared_ptr<vulkan_device> device;
  std::shared_ptr<vulkan_physical_device> physical_device;
  std::shared_ptr<vulkan_surface> surface;

  void create_image_views();

  VkExtent2D choose_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities);
  VkPresentModeKHR choose_swap_present_mode(
      const std::vector<VkPresentModeKHR> available_present_modes);
  VkSurfaceFormatKHR choose_swap_surface_format(
      const std::vector<VkSurfaceFormatKHR>& available_formats);
};

}  // namespace vulkan_wrapper
}  // namespace tobi_engine

#endif // VULKAN_SWAP_CHAIN_HPP_
