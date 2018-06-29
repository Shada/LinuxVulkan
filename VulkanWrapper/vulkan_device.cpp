/*
 * VulkanInstance.cpp
 *
 *  Created on: Jun 20, 2018
 *      Author: admin
 */

#include "vulkan_device.hpp"

#include <iostream>
#include <cassert>

namespace tobivulkan
{

vulkan_device::vulkan_device(const char* application_name,
                             VkQueueFlags requested_queue_types)
    : application_name(application_name),
      requested_queue_types(requested_queue_types),
      queue_family_count(0),
      graphics_queue(0),
      compute_queue(0),
      transfer_queue(0)
{

  validation = std::unique_ptr<vulkan_validation>(
      new vulkan_validation(instance));

  initialize();

  std::cout << ">>> Constructed vulkan_device" << std::endl;
}

vulkan_device::vulkan_device(vulkan_device&& other)
    : application_name(other.application_name),
      requested_queue_types(other.requested_queue_types),
      instance(other.instance),
      physical_device(other.physical_device),
      device(other.device),
      queue_family_count(other.queue_family_count),
      graphics_queue(other.graphics_queue),
      compute_queue(other.compute_queue),
      transfer_queue(other.transfer_queue),
      queue_family_ids(other.queue_family_ids)
{
  other.application_name = nullptr;
  other.requested_queue_types = 0;
  other.instance = VK_NULL_HANDLE;
  other.physical_device = VK_NULL_HANDLE;
  other.device = VK_NULL_HANDLE;
  other.queue_family_count = 0;
  other.graphics_queue = 0;
  other.compute_queue = 0;
  other.transfer_queue = 0;
  other.queue_family_ids =
  { 0,0,0};
  std::cout << "/// Moved vulkan_device" << std::endl;
}

vulkan_device& vulkan_device::operator=(vulkan_device&& other)
{
  if (this != &other)
  {
    validation->destroy_debug_report_callbacks();
    vkDestroyDevice(device, VK_NULL_HANDLE);
    vkDestroyInstance(instance, VK_NULL_HANDLE);

    application_name = other.application_name;
    requested_queue_types = other.requested_queue_types;
    instance = other.instance;
    physical_device = other.physical_device;
    device = other.device;
    queue_family_count = other.queue_family_count;
    graphics_queue = other.graphics_queue;
    compute_queue = other.compute_queue;
    transfer_queue = other.transfer_queue;
    queue_family_ids = other.queue_family_ids;

    other.application_name = nullptr;
    other.requested_queue_types = 0;
    other.instance = VK_NULL_HANDLE;
    other.physical_device = VK_NULL_HANDLE;
    other.device = VK_NULL_HANDLE;
    other.queue_family_count = 0;
    other.graphics_queue = 0;
    other.compute_queue = 0;
    other.transfer_queue = 0;
    other.queue_family_ids =
    {};
  }
  std::cout << "///= Moved vulkan_device END" << std::endl;
  return *this;
}

vulkan_device::~vulkan_device()
{

  if (instance != VK_NULL_HANDLE)
  {
    validation->destroy_debug_report_callbacks();
    vkDestroyDevice(device, VK_NULL_HANDLE);
    vkDestroyInstance(instance, VK_NULL_HANDLE);
  }
  std::cout << "<<< Deconstructed vulkan_device" << std::endl;
}

void vulkan_device::initialize()
{
  create_instance();

  enumerate_devices();

  create_device();

  std::cout << "ooo initialed vulkan_device END" << std::endl;
}

std::vector<const char*> vulkan_device::get_required_instance_extensions()
{

  std::vector<const char*> instance_extension_names =
  { };

  if (validation->is_validation_enabled())
  {
    instance_extension_names.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
  }

  instance_extension_names.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
#ifdef __ANDROID__
  instance_extension_names.push_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);
#elif defined(_WIN32)
  instance_extension_names.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_IOS_MVK)
  instance_extension_names.push_back(VK_MVK_IOS_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_MACOS_MVK)
  instance_extension_names.push_back(VK_MVK_MACOS_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
  instance_extension_names.push_back(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);
#else
  instance_extension_names.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#endif

  return instance_extension_names;
}

void vulkan_device::create_instance()
{
  // creating the instance
  auto application_info = initialisers::init_application_info();
  application_info.pApplicationName = application_name;

  auto instance_create_info = initialisers::init_instance_create_info();
  instance_create_info.pApplicationInfo = &application_info;

  if (validation->is_validation_enabled())
  {
    instance_create_info.enabledLayerCount = static_cast<uint32_t>(validation
        ->validation_layers.size());
    instance_create_info.ppEnabledLayerNames = validation->validation_layers
        .data();
  }

  auto extension_names = get_required_instance_extensions();
  instance_create_info.enabledExtensionCount =
      static_cast<uint32_t>(extension_names.size());
  instance_create_info.ppEnabledExtensionNames = extension_names.data();

  auto result = vkCreateInstance(&instance_create_info, VK_NULL_HANDLE,
                                 &instance);

  validation->init_debug_report_callback();

  // maybe move to some verification function?
  if (result == VK_ERROR_INCOMPATIBLE_DRIVER)
  {
    std::cout << "cannot find a compatible Vulkan ICD\n";
    exit(-1);
  } else if (result)
  {
    std::cout << "unknown error\n";
    exit(-1);
  }
}

void vulkan_device::enumerate_devices()
{
  // enumerating devices
  uint32_t gpu_count = 1;
  vkEnumeratePhysicalDevices(instance, &gpu_count, VK_NULL_HANDLE);
  assert(gpu_count);

  std::vector<VkPhysicalDevice> physical_devices(gpu_count);
  VkResult result = vkEnumeratePhysicalDevices(instance, &gpu_count,
                                               physical_devices.data());

  if (result != VK_SUCCESS && gpu_count <= 0)
    throw std::runtime_error("could not find any GPU devices");

  for (const auto& device : physical_devices)
  {
    // change to "is_device_suitable - method
    if (util::check_device_extension_support(device))
    {
      physical_device = device;
    }
  }
}

void vulkan_device::create_device()
{
  std::vector<VkDeviceQueueCreateInfo> queue_create_infos
  { };

  // Get queue family indices for the requested queue family types
  // Note that the indices may overlap depending on the implementation
  std::vector<VkQueueFamilyProperties> queue_family_properties;
  uint32_t queue_family_count;
  vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count,
                                           nullptr);
  assert(queue_family_count > 0);

  queue_family_properties.resize(queue_family_count);
  vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count,
                                           queue_family_properties.data());

  // TODO: refactor into methods
  const float default_queue_priority(0.0f);

  // Graphics queue
  if (requested_queue_types & VK_QUEUE_GRAPHICS_BIT)
  {
    queue_family_ids.graphics = util::get_queue_family_index(
        queue_family_properties, VK_QUEUE_GRAPHICS_BIT);
    VkDeviceQueueCreateInfo queue_info
    { };
    queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_info.queueFamilyIndex = queue_family_ids.graphics;
    queue_info.queueCount = 1;
    queue_info.pQueuePriorities = &default_queue_priority;
    queue_create_infos.push_back(queue_info);
  } else
  {
    queue_family_ids.graphics = VK_NULL_HANDLE;
  }

  // Dedicated compute queue
  if (requested_queue_types & VK_QUEUE_COMPUTE_BIT)
  {
    queue_family_ids.compute = util::get_queue_family_index(
        queue_family_properties, VK_QUEUE_COMPUTE_BIT);
    if (queue_family_ids.compute != queue_family_ids.graphics)
    {
      // If compute family index differs, we need an additional queue create info for the compute queue
      VkDeviceQueueCreateInfo queueInfo
      { };
      queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      queueInfo.queueFamilyIndex = queue_family_ids.compute;
      queueInfo.queueCount = 1;
      queueInfo.pQueuePriorities = &default_queue_priority;
      queue_create_infos.push_back(queueInfo);
    }
  } else
  {
    // Else we use the same queue
    queue_family_ids.compute = queue_family_ids.graphics;
  }

  // Dedicated transfer queue
  if (requested_queue_types & VK_QUEUE_TRANSFER_BIT)
  {
    queue_family_ids.transfer = util::get_queue_family_index(
        queue_family_properties, VK_QUEUE_TRANSFER_BIT);
    if ((queue_family_ids.transfer != queue_family_ids.graphics)
        && (queue_family_ids.transfer != queue_family_ids.compute))
    {
      // If compute family index differs, we need an additional queue create info for the compute queue
      VkDeviceQueueCreateInfo queue_info
      { };
      queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      queue_info.queueFamilyIndex = queue_family_ids.transfer;
      queue_info.queueCount = 1;
      queue_info.pQueuePriorities = &default_queue_priority;
      queue_create_infos.push_back(queue_info);
    }
  } else
  {
    // Else we use the same queue
    queue_family_ids.transfer = queue_family_ids.graphics;
  }

  VkDeviceCreateInfo device_info = initialisers::init_device_create_info();
  device_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos
      .size());
  ;
  device_info.pQueueCreateInfos = queue_create_infos.data();

  auto device_extensions = util::get_required_device_extensions();
  device_info.enabledExtensionCount = static_cast<uint32_t>(device_extensions
      .size());
  device_info.ppEnabledExtensionNames = device_extensions.data();

  if (validation->is_validation_enabled())
  {
    device_info.enabledLayerCount = static_cast<uint32_t>(validation
        ->validation_layers.size());
    device_info.ppEnabledLayerNames = validation->validation_layers.data();
  }
  VkResult result = vkCreateDevice(physical_device, &device_info, NULL,
                                   &device);

  if (result != VK_SUCCESS)
  {
    std::cout << "error creating device" << std::endl;
  }
  assert(result == VK_SUCCESS);

  vkGetDeviceQueue(device, queue_family_ids.graphics, 0, &graphics_queue);
  vkGetDeviceQueue(device, queue_family_ids.compute, 0, &compute_queue);
  vkGetDeviceQueue(device, queue_family_ids.transfer, 0, &transfer_queue);
}

}  // namespace tobivulkan
