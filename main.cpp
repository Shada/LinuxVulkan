/*
 * main.cpp
 *
 *  Created on: Jun 20, 2018
 *      Author: admin
 */

#include <iostream>
#include <memory>
#include <chrono>

#include "VulkanWrapper/vulkan_device.hpp"
#include "VulkanWrapper/vulkan_swap_chain.hpp"
#include "VulkanWrapper/vulkan_shader_pipeline.hpp"

#ifdef VK_USE_PLATFORM_XCB_KHR
#include "VulkanWrapper/xcb_window_handler.hpp"
#endif /* VK_USE_PLATFORM_XCB_KHR */

namespace tobivulkan
{
void test_launch()
{
  auto instance = std::shared_ptr<vulkan_device>(
      new vulkan_device("Tobi Vulkan Application"));

  auto window = std::shared_ptr<xcb_window_handler>(
      new xcb_window_handler(800, 600));

  auto swap_chain = std::shared_ptr<vulkan_swap_chain>(
      new vulkan_swap_chain(window, instance));

  std::vector<shader> shaders =
  {
  { "./shaders/vert.spv", VK_SHADER_STAGE_VERTEX_BIT },
  { "./shaders/frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT } };

  auto triangle_pipeline = std::unique_ptr<vulkan_shader_pipeline>(
      new vulkan_shader_pipeline(instance, swap_chain, shaders));

  while (!window->is_quit())
  {
    triangle_pipeline->draw_frame();
    window->update();
  }
  vkDeviceWaitIdle(instance->get_device());

}
}

int main()
{

  std::cout << "STARTING" << std::endl;
  tobivulkan::test_launch();
  std::cout << "QUITTING" << std::endl;

  return 0;
}
