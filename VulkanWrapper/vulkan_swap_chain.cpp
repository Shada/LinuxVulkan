/*
 * vulkan_swapchain.cpp
 *
 *  Created on: Jun 22, 2018
 *      Author: admin
 */

#include "vulkan_swap_chain.hpp"

#include <iostream>

#include "vulkan_init_util.hpp"

namespace tobivulkan
{

vulkan_swap_chain::vulkan_swap_chain(
    std::shared_ptr<xcb_window_handler> window,
    std::shared_ptr<vulkan_device> device_instance)
    : window(window),
      device_instance(device_instance)
{

  initialize();

  std::cout << ">>> Constructedvulkan_swap_chain" << std::endl;
}

vulkan_swap_chain::~vulkan_swap_chain()
{
  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
  {
    vkDestroySemaphore(device_instance->get_device(),
                       image_available_semaphores[i], nullptr);
  }
  for (auto image_view : swap_chain_views)
  {
    vkDestroyImageView(device_instance->get_device(), image_view, nullptr);
  }
  vkDestroySwapchainKHR(device_instance->get_device(), swap_chain, nullptr);
  vkDestroySurfaceKHR(device_instance->get_instance(), surface, nullptr);
  std::cout << "<<< Deconstructed vulkan_swap_chain" << std::endl;
}

void vulkan_swap_chain::present_frame(VkSemaphore semaphore,
                                      uint32_t image_index)
{
  VkSemaphore signal_semaphores[] =
  { semaphore };

  VkPresentInfoKHR present_info =
  { };
  present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

  present_info.waitSemaphoreCount = 1;
  present_info.pWaitSemaphores = signal_semaphores;

  VkSwapchainKHR swap_chains[] =
  { swap_chain };
  present_info.swapchainCount = 1;
  present_info.pSwapchains = swap_chains;
  present_info.pImageIndices = &image_index;
  present_info.pResults = nullptr;  // Optional

  vkQueuePresentKHR(present_queue, &present_info);
}

uint32_t vulkan_swap_chain::get_next_image_index(uint32_t current_frame)
{
  uint32_t image_index;
  vkAcquireNextImageKHR(device_instance->get_device(), swap_chain,
                        std::numeric_limits<uint64_t>::max(),
                        image_available_semaphores[current_frame], VK_NULL_HANDLE,
                        &image_index);

  return image_index;
}

void vulkan_swap_chain::initialize()
{
  initialize_surface();

  initialize_swap_chain();

  initialize_swap_chain_images();

  initialize_swap_chain_views();

  image_available_semaphores.resize(MAX_FRAMES_IN_FLIGHT);

  auto semaphore_create_info = initialisers::init_semafore_create_info();

  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
  {
    if (vkCreateSemaphore(device_instance->get_device(), &semaphore_create_info,
                          nullptr, &image_available_semaphores[i])
        != VK_SUCCESS)
    {

      throw std::runtime_error("failed to create render semaphore!");
    }
  }

  std::cout << "ooo Initialized vulkan_swap_chain" << std::endl;
}

void vulkan_swap_chain::initialize_surface()
{
  VkXcbSurfaceCreateInfoKHR surface_create_info =
      initialisers::init_xcb_surface_create_info();
  surface_create_info.connection = window->get_connection();
  surface_create_info.window = window->get_window();

  VkResult result = vkCreateXcbSurfaceKHR(device_instance->get_instance(),
                                          &surface_create_info, NULL, &surface);
  if (result != VK_SUCCESS)
    throw std::runtime_error("could not create instance");
}

void vulkan_swap_chain::initialize_swap_chain_images()
{
  // CREATE SWAP CHAIN IMAGES
  uint32_t real_image_count;
  vkGetSwapchainImagesKHR(device_instance->get_device(), swap_chain,
                          &real_image_count, nullptr);
  swap_chain_images.resize(real_image_count);
  vkGetSwapchainImagesKHR(device_instance->get_device(), swap_chain,
                          &real_image_count, swap_chain_images.data());
}

void vulkan_swap_chain::initialize_swap_chain_views()
{
  // create swap chain image views
  swap_chain_views.resize(swap_chain_images.size());
  for (size_t i = 0; i < swap_chain_views.size(); i++)
  {
    auto image_view_create_info =
        initialisers::init_image_view_2d_create_info();
    image_view_create_info.image = swap_chain_images[i];
    image_view_create_info.format = swap_chain_image_format;
    image_view_create_info.subresourceRange.aspectMask =
        VK_IMAGE_ASPECT_COLOR_BIT;
    if (vkCreateImageView(device_instance->get_device(),
                          &image_view_create_info, nullptr,
                          &swap_chain_views[i]) != VK_SUCCESS)
    {
      throw std::runtime_error("failed to create image views!");
    }
  }
}

void vulkan_swap_chain::initialize_swap_chain()
{
  // GET MAX IMAGE COUNT

  // SELECT FORMAT, PRESENT MODE, AND EXTENT
  auto selected_format = choose_swap_surface_format();

  auto selected_present_mode = choose_swap_present_mode();

  auto selected_extent = choose_swap_extent();

  swap_chain_image_format = selected_format.format;
  swap_chain_extent = selected_extent;

  VkSurfaceCapabilitiesKHR capabilities;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
      device_instance->get_physical_device(), surface, &capabilities);

  auto queue_family_indices = get_queue_family_indices();

  auto swap_chain_create_info = initialisers::init_swap_chain_create_info();
  swap_chain_create_info.surface = surface;
  swap_chain_create_info.minImageCount = get_max_image_count(capabilities);
  swap_chain_create_info.imageFormat = selected_format.format;
  swap_chain_create_info.imageColorSpace = selected_format.colorSpace;
  swap_chain_create_info.imageExtent = selected_extent;

  if (queue_family_indices[0] != queue_family_indices[1])
  {
    swap_chain_create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    swap_chain_create_info.queueFamilyIndexCount = queue_family_indices.size();
    swap_chain_create_info.pQueueFamilyIndices = queue_family_indices.data();
  } else
  {
    swap_chain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swap_chain_create_info.queueFamilyIndexCount = 0;  // Optional
    swap_chain_create_info.pQueueFamilyIndices = nullptr;  // Optional
  }

  swap_chain_create_info.preTransform = capabilities.currentTransform;
  swap_chain_create_info.presentMode = selected_present_mode;
  swap_chain_create_info.clipped = VK_TRUE;

  if (vkCreateSwapchainKHR(device_instance->get_device(),
                           &swap_chain_create_info, nullptr, &swap_chain)
      != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create swap chain!");
  }
}

uint32_t vulkan_swap_chain::get_max_image_count(
    const VkSurfaceCapabilitiesKHR& capabilities)
{
  uint32_t image_count = capabilities.minImageCount + 1;
  if (capabilities.maxImageCount > 0
      && image_count > capabilities.maxImageCount)
  {
    image_count = capabilities.maxImageCount;
  }
  return image_count;
}

std::vector<uint32_t> vulkan_swap_chain::get_queue_family_indices()
{
  std::vector<VkQueueFamilyProperties> queue_family_properties;
  uint32_t queue_family_count;

  vkGetPhysicalDeviceQueueFamilyProperties(
      device_instance->get_physical_device(), &queue_family_count, nullptr);
  assert(queue_family_count > 0);

  queue_family_properties.resize(queue_family_count);
  vkGetPhysicalDeviceQueueFamilyProperties(
      device_instance->get_physical_device(), &queue_family_count,
      queue_family_properties.data());

  present_queue_index = 0;
  for (uint32_t i = 0; i < queue_family_properties.size(); i++)
  {
    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device_instance->get_physical_device(),
                                         i, surface, &presentSupport);

    if (presentSupport)
    {
      present_queue_index = i;
      break;
    }
    i++;
  }

  vkGetDeviceQueue(device_instance->get_device(), present_queue_index, 0,
                   &present_queue);

  auto graphics_family_index = util::get_queue_family_index(
      queue_family_properties, VK_QUEUE_GRAPHICS_BIT);

  std::vector<uint32_t> queue_family_indices =
  { graphics_family_index, present_queue_index };
  return queue_family_indices;
}

VkSurfaceFormatKHR vulkan_swap_chain::choose_swap_surface_format()
{
  uint32_t format_count = 0;
  std::vector<VkSurfaceFormatKHR> available_formats;

  vkGetPhysicalDeviceSurfaceFormatsKHR(device_instance->get_physical_device(),
                                       surface, &format_count, nullptr);

  if (format_count != 0)
  {
    available_formats.resize(format_count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device_instance->get_physical_device(),
                                         surface, &format_count,
                                         available_formats.data());
  }

  if (available_formats.size() == 1
      && available_formats[0].format == VK_FORMAT_UNDEFINED)
  {
    return
    { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
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

VkPresentModeKHR vulkan_swap_chain::choose_swap_present_mode()
{
  std::vector<VkPresentModeKHR> available_present_modes;
  uint32_t present_mode_count;
  vkGetPhysicalDeviceSurfacePresentModesKHR(
      device_instance->get_physical_device(), surface, &present_mode_count,
      nullptr);

  if (present_mode_count != 0)
  {
    available_present_modes.resize(present_mode_count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(
        device_instance->get_physical_device(), surface, &present_mode_count,
        available_present_modes.data());
  }

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

VkExtent2D vulkan_swap_chain::choose_swap_extent()
{
  VkSurfaceCapabilitiesKHR capabilities;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
      device_instance->get_physical_device(), surface, &capabilities);

  if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
  {
    return capabilities.currentExtent;
  } else
  {
    VkExtent2D actual_extent =
    { window->get_width(), window->get_height() };

    actual_extent.width = std::max(
        capabilities.minImageExtent.width,
        std::min(capabilities.maxImageExtent.width, actual_extent.width));
    actual_extent.height = std::max(
        capabilities.minImageExtent.height,
        std::min(capabilities.maxImageExtent.height, actual_extent.height));

    return actual_extent;
  }
}

}

