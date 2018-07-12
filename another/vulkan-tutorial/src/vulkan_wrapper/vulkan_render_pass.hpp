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

#ifndef VULKAN_RENDER_PASS_HPP_
#define VULKAN_RENDER_PASS_HPP_

#include "vulkan_device.hpp"
#include "vulkan_swap_chain.hpp"
#include "vulkan_physical_device.hpp"

namespace tobi_engine
{
namespace vulkan_wrapper
{

///
///
class vulkan_render_pass
{
 public:
  vulkan_render_pass(std::shared_ptr<vulkan_device> device,
                     std::shared_ptr<vulkan_swap_chain> swap_chain,
                     std::shared_ptr<vulkan_physical_device> physical_device);
  ~vulkan_render_pass();
  vulkan_render_pass(vulkan_render_pass &&) = delete;
  vulkan_render_pass(const vulkan_render_pass &) = delete;
  vulkan_render_pass &operator=(const vulkan_render_pass &) = delete;
  vulkan_render_pass &operator=(vulkan_render_pass &&) = delete;

  const VkRenderPass get_render_pass() const
  {
    return render_pass;
  }

 private:

  mutable VkRenderPass render_pass;

  std::shared_ptr<vulkan_device> device;
  std::shared_ptr<vulkan_swap_chain> swap_chain;
  std::shared_ptr<vulkan_physical_device> physical_device;

  void initialize() const;

  const VkFormat find_depth_format() const;
  const VkFormat find_supported_format(const std::vector<VkFormat>& candidates,
                                       VkImageTiling tiling,
                                       VkFormatFeatureFlags features) const;

};

}  // namespace vulkan_wrapper
}  // namespace tobi_engine

#endif // VULKAN_RENDER_PASS_HPP_
