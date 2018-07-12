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

#ifndef VULKAN_SURFACE_HPP_
#define VULKAN_SURFACE_HPP_

#include "window_handler.hpp"
#include "vulkan_instance.hpp"

namespace tobi_engine
{
namespace vulkan_wrapper
{
///
///
class vulkan_surface
{
 public:
  vulkan_surface(std::shared_ptr<window_handler> window,
                 std::shared_ptr<vulkan_instance> instance);
  ~vulkan_surface();
  vulkan_surface(vulkan_surface &&) = delete;
  vulkan_surface(const vulkan_surface &) = delete;
  vulkan_surface &operator=(const vulkan_surface &) = delete;
  vulkan_surface &operator=(vulkan_surface &&) = delete;

  VkSurfaceKHR get_surface()
  {
    return surface;
  }

 private:

  VkSurfaceKHR surface;

  std::shared_ptr<window_handler> window;
  std::shared_ptr<vulkan_instance> instance;

  void initialize();
};

}  // namespace vulkan_wrapper
}  // namespace tobi_engine

#endif // VULKAN_SURFACE_HPP_
