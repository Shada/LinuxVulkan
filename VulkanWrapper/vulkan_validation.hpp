/*
 * vulkan_validation_layers.hpp
 *
 *  Created on: Jun 22, 2018
 *      Author: admin
 */

#ifndef TOBIVULKAN_VULKANWRAPPER_VULKAN_VALIDATION_LAYERS_HPP_
#define TOBIVULKAN_VULKANWRAPPER_VULKAN_VALIDATION_LAYERS_HPP_

#include <vector>

#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <sstream>
#include <vulkan/vulkan.hpp>

namespace tobivulkan
{
class vulkan_validation
{
 public:
  vulkan_validation(VkInstance &instance);
  vulkan_validation(const vulkan_validation& other) = delete;
  vulkan_validation(vulkan_validation&& other) = delete;
  vulkan_validation& operator=(const vulkan_validation&) = delete;
  vulkan_validation& operator=(vulkan_validation&& other) = delete;
  ~vulkan_validation();

//TODO: should be internal. don't need it to be used outside.
  bool check_validation_layer_support();

  void destroy_debug_report_callbacks();

  bool is_validation_enabled(){return enable_validation_layers;}

  void init_debug_report_callback();

  const std::vector<const char*> validation_layers{
    "VK_LAYER_LUNARG_standard_validation" };

 private:

  VkInstance &instance;

  bool enable_validation_layers;


  PFN_vkCreateDebugReportCallbackEXT dbgCreateDebugReportCallback;
  PFN_vkDestroyDebugReportCallbackEXT dbgDestroyDebugReportCallback;
  PFN_vkDebugReportMessageEXT dbgBreakCallback;
  std::vector<VkDebugReportCallbackEXT> debug_report_callbacks;

};
}  // namespace tobivulkan

#endif /* TOBIVULKAN_VULKANWRAPPER_VULKAN_VALIDATION_LAYERS_HPP_ */
