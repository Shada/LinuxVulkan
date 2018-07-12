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

#include "vulkan_validation_layers.hpp"

#include <iostream>
#include <sstream>

namespace tobi_engine
{
namespace vulkan_wrapper
{
namespace validation
{

vulkan_validation_layers::vulkan_validation_layers(VkInstance &instance)
    : callback(VK_NULL_HANDLE),
      instance(instance)
{
}

vulkan_validation_layers::~vulkan_validation_layers()
{
  destroy_debug_report_callback_ext(nullptr);
}

bool vulkan_validation_layers::check_validation_layer_support()
{
  uint32_t layer_count;
  vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

  std::vector<VkLayerProperties> available_layers(layer_count);
  vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

  for (const char* layer_name : get_validation_layers())
  {
    bool layer_found = false;

    for (const auto& layer_properties : available_layers)
    {
      if (strcmp(layer_name, layer_properties.layerName) == 0)
      {
        layer_found = true;
        break;
      }
    }

    if (!layer_found)
    {
      return false;
    }
  }

  return true;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
    VkDebugReportFlagsEXT msgFlags, VkDebugReportObjectTypeEXT objType,
    uint64_t obj, size_t location, int32_t msgCode, const char* pLayerPrefix,
    const char* pMsg, void* userData)
{

  std::ostringstream message;

  if (msgFlags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
  {
    message << "ERROR: ";
  } else if (msgFlags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
  {
    message << "WARNING: ";
  } else if (msgFlags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
  {
    message << "PERFORMANCE WARNING: ";
  } else if (msgFlags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT)
  {
    message << "INFO: ";
  } else if (msgFlags & VK_DEBUG_REPORT_DEBUG_BIT_EXT)
  {
    message << "DEBUG: ";
  }
  message << "[" << pLayerPrefix << "] Code " << msgCode << " Object "
      << objType << " " << obj << " Location " << location << " User Data "
      << userData << " : " << pMsg;

#ifdef _WIN32
  MessageBox(NULL, message.str().c_str(), "Alert", MB_OK);
#else
  std::cout << message << std::endl;
#endif

  /*
   * false indicates that layer should not bail-out of an
   * API call that had validation failures. This may mean that the
   * app dies inside the driver due to invalid parameter(s).
   * That's what would happen without validation layers, so we'll
   * keep that behavior here.
   */

  return VK_FALSE;
}

void vulkan_validation_layers::setup_debug_callback()
{
  if (!is_enable_validation_layers())
    return;

  VkDebugReportCallbackCreateInfoEXT create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
  create_info.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT
      | VK_DEBUG_REPORT_WARNING_BIT_EXT;
  create_info.pfnCallback = debug_callback;

  if (create_debug_report_callback_ext(&create_info, nullptr) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to set up debug callback!");
  }
}

VkResult vulkan_validation_layers::create_debug_report_callback_ext(
    const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator)
{
  auto func = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(
      instance, "vkCreateDebugReportCallbackEXT");
  if (func != nullptr)
  {
    return func(instance, pCreateInfo, pAllocator, &callback);
  } else
  {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

void vulkan_validation_layers::destroy_debug_report_callback_ext(
    const VkAllocationCallbacks* pAllocator)
{
  auto func = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(
      instance, "vkDestroyDebugReportCallbackEXT");

  if (func != nullptr)
  {
    func(instance, callback, pAllocator);
  }
}

}  // namespace validation
}  // namespace vulkan_wrapper
}  // namespace tobi_engine
