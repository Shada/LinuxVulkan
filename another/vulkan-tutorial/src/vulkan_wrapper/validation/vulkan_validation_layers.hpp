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

#ifndef TOBI_ENGINE_VULKAN_WRAPPER_VULKAN_VALIDATION_LAYERS_HPP_
#define TOBI_ENGINE_VULKAN_WRAPPER_VULKAN_VALIDATION_LAYERS_HPP_

#include <vulkan/vulkan.hpp>

namespace tobi_engine
{
namespace vulkan_wrapper
{
namespace validation
{

///  Validation Layers
///  Used to debug vulkan operations
class vulkan_validation_layers
{
 public:
  vulkan_validation_layers(VkInstance &instance);
  ~vulkan_validation_layers();
  vulkan_validation_layers(vulkan_validation_layers &&) = delete;
  vulkan_validation_layers(const vulkan_validation_layers &) = delete;
  vulkan_validation_layers &operator=(const vulkan_validation_layers &) = delete;
  vulkan_validation_layers &operator=(vulkan_validation_layers &&) = delete;

  bool check_validation_layer_support();

  void setup_debug_callback();

  VkResult create_debug_report_callback_ext(
      const VkDebugReportCallbackCreateInfoEXT* create_info,
      const VkAllocationCallbacks* allocator);

  void destroy_debug_report_callback_ext(
      const VkAllocationCallbacks* allocator);

  bool is_enable_validation_layers()
  {
#ifdef NDEBUG
    return false;
#else
    return true;
#endif
  }

  const std::vector<const char*> &get_validation_layers()
  {
    return layers;
  }

 private:

  const std::vector<const char*> layers = {
      "VK_LAYER_LUNARG_standard_validation" };

  VkDebugReportCallbackEXT callback;

  VkInstance &instance;
};

}  // namespace validation
}  // namespace vulkan_wrapper
}  // namespace tobi_engine

#endif // TOBI_ENGINE_VULKAN_WRAPPER_VULKAN_VALIDATION_LAYERS_HPP_
