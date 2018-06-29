/*
 * vulkan_swapchain.hpp
 *
 *  Created on: Jun 22, 2018
 *      Author: admin
 */

#ifndef TOBIVULKAN_VULKANWRAPPER_VULKAN_SWAPCHAIN_HPP_
#define TOBIVULKAN_VULKANWRAPPER_VULKAN_SWAPCHAIN_HPP_

#include <vulkan/vulkan.hpp>

#ifdef VK_USE_PLATFORM_XCB_KHR
#include "xcb_window_handler.hpp"
#endif

#include "vulkan_device.hpp"

namespace tobivulkan
{

class vulkan_swap_chain
{
 public:

#ifdef VK_USE_PLATFORM_XCB_KHR
  vulkan_swap_chain(std::shared_ptr<xcb_window_handler> window,
                    std::shared_ptr<vulkan_device> device_instance);
#endif

  vulkan_swap_chain(const vulkan_swap_chain& other) = delete;
  vulkan_swap_chain(vulkan_swap_chain&& other) = delete;
  vulkan_swap_chain& operator=(const vulkan_swap_chain&) = delete;
  vulkan_swap_chain& operator=(vulkan_swap_chain&& other) = delete;
  ~vulkan_swap_chain();

  // save the image_index in variable.
  void present_frame(VkSemaphore semaphore, uint32_t image_index);

  uint32_t get_next_image_index(uint32_t current_frame);

  VkExtent2D get_extent()
  {
    return swap_chain_extent;
  }
  VkFormat get_format()
  {
    return swap_chain_image_format;
  }
  const std::vector<VkImageView>& get_image_views()
  {
    return swap_chain_views;
  }
  const VkSemaphore get_image_available_semaphore(uint32_t current_frame)
  {
    return image_available_semaphores[current_frame];
  }



 private:

  void initialize();

  void initialize_surface();
  void initialize_swap_chain();

  VkSurfaceFormatKHR choose_swap_surface_format();
  VkPresentModeKHR choose_swap_present_mode();
  VkExtent2D choose_swap_extent();

  uint32_t get_max_image_count(const VkSurfaceCapabilitiesKHR& capabilities);
  std::vector<uint32_t> get_queue_family_indices();
  void initialize_swap_chain_images();
  void initialize_swap_chain_views();

#ifdef VK_USE_PLATFORM_XCB_KHR
  std::shared_ptr<xcb_window_handler> window;
#endif
  // TODO: i feel like there are too much stuff here.
  VkSurfaceKHR surface;
  VkSwapchainKHR swap_chain;
  VkFormat swap_chain_image_format;
  VkExtent2D swap_chain_extent;
  VkQueue present_queue;
  uint32_t present_queue_index;
  std::vector<VkImage> swap_chain_images;
  std::vector<VkImageView> swap_chain_views;
  std::vector<VkSemaphore> image_available_semaphores;

  std::shared_ptr<vulkan_device> device_instance;

};

}

#endif /* TOBIVULKAN_VULKANWRAPPER_VULKAN_SWAPCHAIN_HPP_ */
