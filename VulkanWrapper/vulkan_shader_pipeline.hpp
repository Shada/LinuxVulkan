/*
 * vulkan_shader_pipeline.hpp
 *
 *  Created on: Jun 26, 2018
 *      Author: admin
 */

#ifndef TOBIVULKAN_VULKANWRAPPER_VULKAN_SHADER_PIPELINE_HPP_
#define TOBIVULKAN_VULKANWRAPPER_VULKAN_SHADER_PIPELINE_HPP_

#include <vector>

#include <vulkan/vulkan.hpp>

#include "vulkan_device.hpp"
#include "vulkan_swap_chain.hpp"

namespace tobivulkan
{

typedef struct _shader
{
  std::string file_name;
  VkShaderStageFlagBits shader_type;
} shader;

class vulkan_shader_pipeline
{
 public:
  vulkan_shader_pipeline(std::shared_ptr<vulkan_device> device_instance,
                         std::shared_ptr<vulkan_swap_chain> swap_chain,
                         std::vector<shader> shader_files);

  vulkan_shader_pipeline(const vulkan_shader_pipeline& other) = delete;

  vulkan_shader_pipeline(vulkan_shader_pipeline&& other) = delete;

  vulkan_shader_pipeline& operator=(const vulkan_shader_pipeline&) = delete;

  vulkan_shader_pipeline& operator=(vulkan_shader_pipeline&& other) = delete;

  ~vulkan_shader_pipeline();

  void draw_frame();
 private:

  std::shared_ptr<vulkan_device> device_instance;
  std::shared_ptr<vulkan_swap_chain> swap_chain;

  VkPipelineLayout pipeline_layout;
  VkPipeline graphics_pipeline;
  std::vector<VkFramebuffer> frame_buffers;
  VkRenderPass render_pass;
  VkCommandPool command_pool;
  std::vector<VkCommandBuffer> command_buffers;
  std::vector<VkFence> in_flight_fences;
  std::vector<VkSemaphore> render_finished_semaphores;

  size_t current_frame = 0;

  auto initialize(std::vector<shader> shader_files) -> void;

  auto record_command_buffers() -> void;
  auto create_shader_module(shader shader) -> VkShaderModule;
  auto create_render_pass() -> void;
  auto create_pipeline_layout() -> void;
  auto create_vertex_input_info() -> VkPipelineVertexInputStateCreateInfo;
  auto create_input_assembly() -> VkPipelineInputAssemblyStateCreateInfo;
  auto create_viewport() -> VkViewport;
  auto create_rasterizer() -> VkPipelineRasterizationStateCreateInfo;
  auto create_multisampling_info() -> VkPipelineMultisampleStateCreateInfo;
  auto create_color_blend_attachment() -> VkPipelineColorBlendAttachmentState;
  auto create_shader_stage_create_info(
      shader shader,
      VkShaderModule shader_module) -> VkPipelineShaderStageCreateInfo;
  auto create_frame_buffers() -> void;
  auto create_command_pool() -> void;
  auto create_command_buffers() -> void;

};

}

#endif /* TOBIVULKAN_VULKANWRAPPER_VULKAN_SHADER_PIPELINE_HPP_ */
