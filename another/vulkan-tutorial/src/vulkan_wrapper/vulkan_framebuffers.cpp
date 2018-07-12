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

#include "vulkan_framebuffers.hpp"

#include <array>

namespace tobi_engine
{
namespace vulkan_wrapper
{

vulkan_framebuffers::vulkan_framebuffers(
    std::shared_ptr<vulkan_device> device,
    std::shared_ptr<vulkan_swap_chain> swap_chain,
    std::shared_ptr<vulkan_render_pass> render_pass,
    VkImageView depth_image_view)
    : framebuffers(std::vector<VkFramebuffer> {}),
      device(device),
      swap_chain(swap_chain),
      render_pass(render_pass),
      depth_image_view(depth_image_view)
{
  initialize();
}

vulkan_framebuffers::~vulkan_framebuffers()
{
  for (auto framebuffer : framebuffers)
  {
    vkDestroyFramebuffer(device->get_device(), framebuffer, nullptr);
  }
}

void vulkan_framebuffers::initialize() const
{
  framebuffers.resize(swap_chain->get_num_images());

  for (size_t i = 0; i < swap_chain->get_num_images(); i++)
  {
    std::array<VkImageView, 2> attachments =
    {
        swap_chain->get_image_view(i),
        depth_image_view
    };

    VkFramebufferCreateInfo framebuffer_info = {};
    framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebuffer_info.renderPass = render_pass->get_render_pass();
    framebuffer_info.attachmentCount = static_cast<uint32_t>(attachments.size());
    framebuffer_info.pAttachments = attachments.data();
    framebuffer_info.width = swap_chain->get_extent().width;
    framebuffer_info.height = swap_chain->get_extent().height;
    framebuffer_info.layers = 1;

    if (vkCreateFramebuffer(device->get_device(), &framebuffer_info, nullptr, &framebuffers[i]) 
        != VK_SUCCESS)
    {
      throw std::runtime_error("failed to create framebuffer!");
    }
  }
}

}  // namespace vulkan_wrapper
}  // namespace tobi_engine
