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

#ifndef VULKAN_DEVICE_HPP_
#define VULKAN_DEVICE_HPP_

#include "vulkan_physical_device.hpp"

namespace tobi_engine
{
namespace vulkan_wrapper
{
///
///
class vulkan_device
{
 public:
  vulkan_device(std::shared_ptr<vulkan_physical_device> physical_device,
                std::shared_ptr<vulkan_instance> instance);
  ~vulkan_device();
  vulkan_device(vulkan_device &&) = delete;
  vulkan_device(const vulkan_device &) = delete;
  vulkan_device &operator=(const vulkan_device &) = delete;
  vulkan_device &operator=(vulkan_device &&) = delete;

  void wait_idle() const;

  const VkDevice get_device() const
  {
    return device;
  }
  const VkQueue get_graphics_queue() const
  {
    return graphics_queue;
  }
  const VkQueue get_present_queue() const
  {
    return present_queue;
  }

 private:

  mutable VkDevice device;
  mutable VkQueue graphics_queue;
  mutable VkQueue present_queue;

  std::shared_ptr<vulkan_physical_device> physical_device;
  std::shared_ptr<vulkan_instance> instance;

  void initialize() const;

  bool check_device_extension_support();
};

}  // namespace vulkan_wrapper
}  // namespace tobi_engine
#endif // VULKAN_DEVICE_HPP_
