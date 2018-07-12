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

#ifndef TOBI_ENGINE_VULKAN_WRAPPER_VULKAN_INSTANCE_HPP_
#define TOBI_ENGINE_VULKAN_WRAPPER_VULKAN_INSTANCE_HPP_

#include "validation/vulkan_validation_layers.hpp"

namespace tobi_engine
{
namespace vulkan_wrapper
{

///
///
class vulkan_instance
{
 public:
  vulkan_instance();
  ~vulkan_instance();
  vulkan_instance(vulkan_instance &&) = delete;
  vulkan_instance(const vulkan_instance &) = delete;
  vulkan_instance &operator=(const vulkan_instance &) = delete;
  vulkan_instance &operator=(vulkan_instance &&) = delete;

  const VkInstance get_instance() const
  {
    return instance;
  }

  const std::vector<const char*> get_validation_layers() const
  {
    return validation_layers->is_enable_validation_layers()
         ? validation_layers->get_validation_layers()
         : std::vector<const char*>();
  }

 private:

  mutable VkInstance instance;
  mutable std::unique_ptr<validation::vulkan_validation_layers> validation_layers;

  void initialize() const;

  const std::vector<const char*> get_required_extensions() const;

};

}  // namespace vulkan_wrapper
}  // namespace tobi_engine

#endif // TOBI_ENGINE_VULKAN_WRAPPER_VULKAN_INSTANCE_HPP_
