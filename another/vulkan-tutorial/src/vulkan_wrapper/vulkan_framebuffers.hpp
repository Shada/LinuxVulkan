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

#ifndef VULKAN_FRAMEBUFFERS_HPP_
#define VULKAN_FRAMEBUFFERS_HPP_

#include "vulkan_device.hpp"
#include "vulkan_swap_chain.hpp"
#include "vulkan_render_pass.hpp"

namespace tobi_engine
{
namespace vulkan_wrapper
{

///
///
class vulkan_framebuffers
{
 public:
  vulkan_framebuffers(std::shared_ptr<vulkan_device> device,
                       std::shared_ptr<vulkan_swap_chain> swap_chain,
                       std::shared_ptr<vulkan_render_pass> render_pass,
                       VkImageView depth_image_view);
  ~vulkan_framebuffers();
  vulkan_framebuffers(vulkan_framebuffers &&) = delete;
  vulkan_framebuffers(const vulkan_framebuffers &) = delete;
  vulkan_framebuffers &operator=(const vulkan_framebuffers &) = delete;
  vulkan_framebuffers &operator=(vulkan_framebuffers &&) = delete;

  const std::vector<VkFramebuffer> get_framebuffers() const
  {
    return framebuffers;
  }

  const VkFramebuffer get_frame_buffer(uint32_t index) const
  {
    if (index >= framebuffers.size())
      throw "out of bounds";
    return framebuffers[index];
  }
  const uint32_t get_num_frame_buffers() const
  {
    return static_cast<uint32_t>(framebuffers.size());
  }

 private:

  mutable std::vector<VkFramebuffer> framebuffers;
  std::shared_ptr<vulkan_device> device;
  std::shared_ptr<vulkan_swap_chain> swap_chain;
  std::shared_ptr<vulkan_render_pass> render_pass;
  VkImageView depth_image_view;

  void initialize() const;
};

}  // namespace vulkan_wrapper
}  // namespace tobi_engine

#endif // VULKAN_FRAMEBUFFERS_HPP_
