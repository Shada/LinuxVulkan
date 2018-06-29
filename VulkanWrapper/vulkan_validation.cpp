/*
 * vulkan_validation_layers.cpp
 *
 *  Created on: Jun 25, 2018
 *      Author: admin
 */

#include <vulkan/vulkan.hpp>
#include "vulkan_validation.hpp"

namespace tobivulkan
{
vulkan_validation::vulkan_validation(VkInstance &instance)
    : instance(instance)
{

#if defined(NDEBUG)
  enable_validation_layers = false;
#else
  enable_validation_layers = true;
#endif

  if (enable_validation_layers && !check_validation_layer_support())
  {
    throw std::runtime_error("Validation layers requested, but not available!");
  }
  std::cout << ">>> Constructed vulkan_validation" << std::endl;
}
vulkan_validation::~vulkan_validation()
{
  destroy_debug_report_callbacks();
  std::cout << "<<< Deconstructed vulkan_validation" << std::endl;
}

void vulkan_validation::destroy_debug_report_callbacks()
{
  if (!enable_validation_layers)
  {
    return;
  }
  while (debug_report_callbacks.size() > 0)
  {
    dbgDestroyDebugReportCallback(instance, debug_report_callbacks.back(),
    NULL);
    debug_report_callbacks.pop_back();
  }
}

VKAPI_ATTR VkBool32 VKAPI_CALL dbgFunc(VkDebugReportFlagsEXT msgFlags,
                                       VkDebugReportObjectTypeEXT objType,
                                       uint64_t srcObject, size_t location,
                                       int32_t msgCode,
                                       const char *pLayerPrefix,
                                       const char *pMsg, void *pUserData)
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
  message << "[" << pLayerPrefix << "] Code " << msgCode << " : " << pMsg;

#ifdef _WIN32
  MessageBox(NULL, message.str().c_str(), "Alert", MB_OK);
#else
  std::cout << message.str() << std::endl;
#endif

  /*
   * false indicates that layer should not bail-out of an
   * API call that had validation failures. This may mean that the
   * app dies inside the driver due to invalid parameter(s).
   * That's what would happen without validation layers, so we'll
   * keep that behavior here.
   */
  return false;
}

bool vulkan_validation::check_validation_layer_support()
{
  if (!enable_validation_layers)
  {
    return false;
  }

  uint32_t layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

  std::vector<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  for (const char* layerName : validation_layers)
  {
    bool layerFound = false;

    for (const auto& layerProperties : availableLayers)
    {
      if (strcmp(layerName, layerProperties.layerName) == 0)
      {
        layerFound = true;
        break;
      }
    }

    if (!layerFound)
    {
      return false;
    }
  }

  return true;
}

void vulkan_validation::init_debug_report_callback()
{
  if (!enable_validation_layers)
    return;

  VkResult res;
  VkDebugReportCallbackEXT debug_report_callback;

  dbgCreateDebugReportCallback =
      (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(
          instance, "vkCreateDebugReportCallbackEXT");

  if (!dbgCreateDebugReportCallback)
  {
    std::cout << "GetInstanceProcAddr: Unable to find "
              "vkCreateDebugReportCallbackEXT function."
              << std::endl;
  }
  std::cout << "Got dbgCreateDebugReportCallback function\n";

  dbgDestroyDebugReportCallback =
      (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(
          instance, "vkDestroyDebugReportCallbackEXT");
  if (!dbgDestroyDebugReportCallback)
  {
    std::cout << "GetInstanceProcAddr: Unable to find "
              "vkDestroyDebugReportCallbackEXT function."
              << std::endl;
  }
  std::cout << "Got dbgDestroyDebugReportCallback function\n";

  VkDebugReportCallbackCreateInfoEXT create_info =
  { };
  create_info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
  create_info.pNext = NULL;
  create_info.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT
      | VK_DEBUG_REPORT_WARNING_BIT_EXT;
  create_info.pfnCallback = dbgFunc;
  create_info.pUserData = NULL;

  res = dbgCreateDebugReportCallback(instance, &create_info, NULL,
                                     &debug_report_callback);
  switch (res)
  {
    case VK_SUCCESS:
      std::cout << "Successfully created debug report callback object\n";
      debug_report_callbacks.push_back(debug_report_callback);
      break;
    case VK_ERROR_OUT_OF_HOST_MEMORY:
      std::cout << "dbgCreateDebugReportCallback: out of host memory pointer\n"
                << std::endl;
      break;
    default:
      std::cout << "dbgCreateDebugReportCallback: unknown failure\n"
                << std::endl;
      break;
  }

  if (res != VK_SUCCESS)
    std::cout << "failed to create validation layers!\n";

  std::cout << "ooo initialized vulkan_validation" << std::endl;
}

}  // namespace tobivulkan
