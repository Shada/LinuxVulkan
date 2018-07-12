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

#include "vulkan_swap_chain.hpp"
#include "helper.hpp"

namespace tobi_engine
{
namespace vulkan_wrapper
{
vulkan_swap_chain::vulkan_swap_chain(
    std::shared_ptr<window_handler> window,
    std::shared_ptr<vulkan_device> device,
    std::shared_ptr<vulkan_physical_device> physical_device,
    std::shared_ptr<vulkan_surface> surface)
    : window(window),
      device(device),
      physical_device(physical_device),
      surface(surface)
{
  initialize();
}

vulkan_swap_chain::~vulkan_swap_chain()
{
  for (const auto &image_view : image_views)
  {
    vkDestroyImageView(device->get_device(), image_view, nullptr);
  }

  vkDestroySwapchainKHR(device->get_device(), swap_chain, nullptr);
}

void vulkan_swap_chain::initialize()
{
  auto swap_chain_support = physical_device->query_swap_chain_support();

  auto surface_format = choose_swap_surface_format(swap_chain_support.formats);
  auto present_mode = choose_swap_present_mode(swap_chain_support.present_modes);
  auto extent = choose_swap_extent(swap_chain_support.capabilities);

  auto image_count = swap_chain_support.capabilities.minImageCount + 1;
  if (swap_chain_support.capabilities.maxImageCount > 0
      && image_count > swap_chain_support.capabilities.maxImageCount)
  {
    image_count = swap_chain_support.capabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  create_info.surface = surface->get_surface();

  create_info.minImageCount = image_count;
  create_info.imageFormat = surface_format.format;
  create_info.imageColorSpace = surface_format.colorSpace;
  create_info.imageExtent = extent;
  create_info.imageArrayLayers = 1;
  create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  queue_family_indices indices = physical_device->find_queue_families();
  uint32_t queue_family_indices[] =
  {
      (uint32_t) indices.graphics_family,
      (uint32_t) indices.present_family
  };

  if (indices.graphics_family != indices.present_family)
  {
    create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    create_info.queueFamilyIndexCount = 2;
    create_info.pQueueFamilyIndices = queue_family_indices;
  } else
  {
    create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  }

  create_info.preTransform = swap_chain_support.capabilities.currentTransform;
  create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  create_info.presentMode = present_mode;
  create_info.clipped = VK_TRUE;

  if (vkCreateSwapchainKHR(device->get_device(), &create_info, nullptr, &swap_chain)
      != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create swap chain!");
  }

  vkGetSwapchainImagesKHR(device->get_device(), swap_chain, &image_count,
                          nullptr);

  images.resize(image_count);
  vkGetSwapchainImagesKHR(device->get_device(), swap_chain, &image_count,
                          images.data());

  image_format = surface_format.format;
  this->extent = extent;

  create_image_views();
}

void vulkan_swap_chain::create_image_views()
{
  image_views.resize(images.size());

  for (uint32_t i = 0; i < images.size(); i++)
  {
    image_views[i] = helper::create_image_view(images[i], image_format,
                                             VK_IMAGE_ASPECT_COLOR_BIT,
                                             device->get_device());
  }
}

VkSurfaceFormatKHR vulkan_swap_chain::choose_swap_surface_format(
    const std::vector<VkSurfaceFormatKHR>& available_formats)
{
  if (available_formats.size() == 1
      && available_formats[0].format == VK_FORMAT_UNDEFINED)
  {
    return
    {
      VK_FORMAT_B8G8R8A8_UNORM,
      VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
    };
  }

  for (const auto& available_format : available_formats)
  {
    if (available_format.format == VK_FORMAT_B8G8R8A8_UNORM
        && available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
    {
      return available_format;
    }
  }

  return available_formats[0];
}

VkPresentModeKHR vulkan_swap_chain::choose_swap_present_mode(
    const std::vector<VkPresentModeKHR> available_present_modes)
{
  auto best_mode = VK_PRESENT_MODE_FIFO_KHR;

  for (const auto& available_present_mode : available_present_modes)
  {
    if (available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR)
    {
      return available_present_mode;
    } else if (available_present_mode == VK_PRESENT_MODE_IMMEDIATE_KHR)
    {
      best_mode = available_present_mode;
    }
  }

  return best_mode;
}

VkExtent2D vulkan_swap_chain::choose_swap_extent(
    const VkSurfaceCapabilitiesKHR& capabilities)
{
  if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
  {
    return capabilities.currentExtent;
  } else
  {
    // TODO: change to getSize method that returns height and width
    int width, height;
    glfwGetFramebufferSize(window->get_window(), &width, &height);

    VkExtent2D actual_extent =
    {
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height)
    };

    actual_extent.width = std::max(
        capabilities.minImageExtent.width,
        std::min(capabilities.maxImageExtent.width, actual_extent.width));
    actual_extent.height = std::max(
        capabilities.minImageExtent.height,
        std::min(capabilities.maxImageExtent.height, actual_extent.height));

    return actual_extent;
  }
}

}  // namespace vulkan_wrapper
}  // namespace tobi_engine

