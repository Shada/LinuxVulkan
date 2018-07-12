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

#include "vulkan_surface.hpp"

namespace tobi_engine
{
namespace vulkan_wrapper
{

vulkan_surface::vulkan_surface(std::shared_ptr<window_handler> window,
                               std::shared_ptr<vulkan_instance> instance)
    : surface(VK_NULL_HANDLE),
      window(window),
      instance(instance)
{
  initialize();
}

vulkan_surface::~vulkan_surface()
{
  vkDestroySurfaceKHR(instance->get_instance(), surface, nullptr);
}

void vulkan_surface::initialize()
{
  if (glfwCreateWindowSurface(instance->get_instance(), window->get_window(),
                              nullptr, &surface) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create window surface!");
  }
}

}  // namespace vulkan_wrapper
}  // namespace tobi_engine

