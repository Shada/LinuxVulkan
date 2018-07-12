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

#include "vulkan_instance.hpp"

#include <GLFW/glfw3.h>

#include "../util/std_functions.hpp"

namespace tobi_engine
{
namespace vulkan_wrapper
{
vulkan_instance::vulkan_instance()
: instance(VK_NULL_HANDLE),
  validation_layers(nullptr)
{
  initialize();
}

vulkan_instance::~vulkan_instance()
{
  if (validation_layers->is_enable_validation_layers())
  {
    validation_layers->destroy_debug_report_callback_ext(nullptr);
  }
  vkDestroyInstance(instance, nullptr);
}

void vulkan_instance::initialize() const
{
  if (validation_layers->is_enable_validation_layers())
    validation_layers = util::cpp14::make_unique<validation::vulkan_validation_layers>(instance);

  if (validation_layers->is_enable_validation_layers()
      && !validation_layers->check_validation_layer_support())
  {
    throw std::runtime_error("validation layers requested, but not available!");
  }

  VkApplicationInfo application_info = {};
  application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  application_info.pApplicationName = "Hello Triangle";
  application_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  application_info.pEngineName = "No Engine";
  application_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  application_info.apiVersion = VK_API_VERSION_1_1;

  auto extensions = get_required_extensions();

  VkInstanceCreateInfo instance_create_info = {};
  instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instance_create_info.pApplicationInfo = &application_info;
  instance_create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  instance_create_info.ppEnabledExtensionNames = extensions.data();

  if (validation_layers->is_enable_validation_layers())
  {
    instance_create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers
        ->get_validation_layers().size());
    instance_create_info.ppEnabledLayerNames = validation_layers->get_validation_layers()
        .data();
  } else
  {
    instance_create_info.enabledLayerCount = 0;
  }
  if (vkCreateInstance(&instance_create_info, nullptr, &instance) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create instance!");
  }

  if (validation_layers->is_enable_validation_layers())
    validation_layers->setup_debug_callback();
}


const std::vector<const char*> vulkan_instance::get_required_extensions() const
{
  uint32_t glfwExtensionCount = 0;
  const char** glfwExtensions;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  std::vector<const char*> extensions(glfwExtensions,
                                      glfwExtensions + glfwExtensionCount);

  if (validation_layers->is_enable_validation_layers())
  {
    extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
  }

  return extensions;
}

}  // namespace vulkan_wrapper
}  // namespace tobi_engine
