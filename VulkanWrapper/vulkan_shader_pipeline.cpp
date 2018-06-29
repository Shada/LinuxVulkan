/*
 * vulkan_shader_pipeline.cpp
 *
 *  Created on: Jun 26, 2018
 *      Author: admin
 */

#include "vulkan_shader_pipeline.hpp"

#include <iostream>

#include "vulkan_init_util.hpp"

namespace tobivulkan
{

vulkan_shader_pipeline::vulkan_shader_pipeline(
    std::shared_ptr<vulkan_device> device_instance,
    std::shared_ptr<vulkan_swap_chain> swap_chain,
    std::vector<shader> shader_files)
    : device_instance(device_instance),
      swap_chain(swap_chain)
{

  initialize(shader_files);

  std::cout << ">>> Constructed vulkan_shader_pipeline" << std::endl;
}

vulkan_shader_pipeline::~vulkan_shader_pipeline()
{
  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
  {
    vkDestroySemaphore(device_instance->get_device(),
                       render_finished_semaphores[i], nullptr);

    vkDestroyFence(device_instance->get_device(), in_flight_fences[i], nullptr);
  }
  vkFreeCommandBuffers(device_instance->get_device(), command_pool,
                       command_buffers.size(), command_buffers.data());
  vkDestroyCommandPool(device_instance->get_device(), command_pool, nullptr);

  for (auto frame_buffer : frame_buffers)
  {
    vkDestroyFramebuffer(device_instance->get_device(), frame_buffer, nullptr);
  }
  vkDestroyPipeline(device_instance->get_device(), graphics_pipeline, nullptr);
  vkDestroyPipelineLayout(device_instance->get_device(), pipeline_layout,
                          nullptr);
  vkDestroyRenderPass(device_instance->get_device(), render_pass, nullptr);
  std::cout << "<<< Deconstructed vulkan_shader_pipeline" << std::endl;
}

void vulkan_shader_pipeline::draw_frame()
{
  vkWaitForFences(device_instance->get_device(), 1,
                  &in_flight_fences[current_frame], VK_TRUE,
                  std::numeric_limits<uint64_t>::max());
  vkResetFences(device_instance->get_device(), 1,
                &in_flight_fences[current_frame]);
  uint32_t image_index = swap_chain->get_next_image_index(current_frame);

  VkSubmitInfo submit_info =
  { };
  submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore wait_semaphores[] =
  { swap_chain->get_image_available_semaphore(current_frame) };

  VkPipelineStageFlags wait_stages[] =
  { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

  submit_info.waitSemaphoreCount = 1;
  submit_info.pWaitSemaphores = wait_semaphores;
  submit_info.pWaitDstStageMask = wait_stages;
  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers = &command_buffers[image_index];

  VkSemaphore signal_semaphores[] =
  { render_finished_semaphores[current_frame] };
  submit_info.signalSemaphoreCount = 1;
  submit_info.pSignalSemaphores = signal_semaphores;

  if (vkQueueSubmit(device_instance->get_graphics_queue(), 1, &submit_info,
                    in_flight_fences[current_frame]) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to submit draw command buffer!");
  }

  swap_chain->present_frame(render_finished_semaphores[current_frame],
                            image_index);

  current_frame = (current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void vulkan_shader_pipeline::initialize(std::vector<shader> shader_files)
{
std::vector<VkPipelineShaderStageCreateInfo> pipeline_shader_stage_create_infos;
std::vector<VkShaderModule> shader_modules;

for (auto& shader : shader_files)
{
  auto shader_module = create_shader_module(shader);

  shader_modules.push_back(shader_module);

  auto pipeline_shader_stage_create_info = create_shader_stage_create_info(
      shader, shader_module);

  pipeline_shader_stage_create_infos.push_back(
      pipeline_shader_stage_create_info);

}

// render pass
create_render_pass();

// ---------------------------------------------------------
// TODO: should have some clever way to choose different options for different shaders.
auto vertex_input_info = create_vertex_input_info();

auto input_assembly = create_input_assembly();
auto viewport = create_viewport();
VkRect2D scissor =
{ };
scissor.offset =
{ 0, 0};
scissor.extent = swap_chain->get_extent();

VkPipelineViewportStateCreateInfo viewport_state =
{ };
viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
viewport_state.viewportCount = 1;
viewport_state.pViewports = &viewport;
viewport_state.scissorCount = 1;
viewport_state.pScissors = &scissor;

auto rasterizer = create_rasterizer();
auto multisampling = create_multisampling_info();
auto color_blend_attachment = create_color_blend_attachment();
VkPipelineColorBlendStateCreateInfo color_blending =
{ };
color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
color_blending.logicOpEnable = VK_FALSE;
color_blending.logicOp = VK_LOGIC_OP_COPY;  // Optional
color_blending.attachmentCount = 1;
color_blending.pAttachments = &color_blend_attachment;
color_blending.blendConstants[0] = 0.0f;  // Optional
color_blending.blendConstants[1] = 0.0f;  // Optional
color_blending.blendConstants[2] = 0.0f;  // Optional
color_blending.blendConstants[3] = 0.0f;  // Optional

/*VkDynamicState dynamic_states[] =
 { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_LINE_WIDTH };

 VkPipelineDynamicStateCreateInfo dynamic_state =
 { };
 dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
 dynamic_state.dynamicStateCount = 2;
 dynamic_state.pDynamicStates = dynamic_states;*/

create_pipeline_layout();

VkGraphicsPipelineCreateInfo pipeline_info =
{ };
pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
pipeline_info.stageCount = 2;
pipeline_info.pStages = pipeline_shader_stage_create_infos.data();
pipeline_info.pVertexInputState = &vertex_input_info;
pipeline_info.pInputAssemblyState = &input_assembly;
pipeline_info.pViewportState = &viewport_state;
pipeline_info.pRasterizationState = &rasterizer;
pipeline_info.pMultisampleState = &multisampling;
pipeline_info.pDepthStencilState = nullptr;  // Optional
pipeline_info.pColorBlendState = &color_blending;
pipeline_info.pDynamicState = nullptr;  // Optional
pipeline_info.layout = pipeline_layout;
pipeline_info.renderPass = render_pass;
pipeline_info.subpass = 0;
pipeline_info.basePipelineHandle = VK_NULL_HANDLE;  // Optional
pipeline_info.basePipelineIndex = -1;  // Optional

if (vkCreateGraphicsPipelines(device_instance->get_device(), VK_NULL_HANDLE, 1,
                              &pipeline_info, nullptr, &graphics_pipeline)
    != VK_SUCCESS)
{
  throw std::runtime_error("failed to create graphics pipeline!");
}

for (auto& module : shader_modules)
{
  vkDestroyShaderModule(device_instance->get_device(), module,
  VK_NULL_HANDLE);
}

create_frame_buffers();

create_command_pool();
create_command_buffers();

record_command_buffers();

auto semaphore_create_info = initialisers::init_semafore_create_info();
VkFenceCreateInfo fence_info =
{ };
fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

render_finished_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
in_flight_fences.resize(MAX_FRAMES_IN_FLIGHT);

for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
{
  if (vkCreateSemaphore(device_instance->get_device(), &semaphore_create_info,
                        nullptr, &render_finished_semaphores[i]) != VK_SUCCESS
      || vkCreateFence(device_instance->get_device(), &fence_info, nullptr,
                       &in_flight_fences[i]) != VK_SUCCESS)
  {

    throw std::runtime_error("failed to create render semaphore!");
  }
}

std::cout << "ooo initialized vulkan_shader_pipeline" << std::endl;
}

VkShaderModule vulkan_shader_pipeline::create_shader_module(shader shader)
{
auto buffer = util::read_file(shader.file_name);

auto shader_module_create_info = initialisers::init_shader_module_create_info();

shader_module_create_info.codeSize = buffer.size();
shader_module_create_info.pCode =
    reinterpret_cast<const uint32_t*>(buffer.data());

VkShaderModule shader_module;
vkCreateShaderModule(device_instance->get_device(), &shader_module_create_info,
VK_NULL_HANDLE,
                     &shader_module);

return shader_module;
}

VkPipelineShaderStageCreateInfo vulkan_shader_pipeline::create_shader_stage_create_info(
  shader shader, VkShaderModule shader_module)
{

auto pipeline_shader_stage_create_info =
    initialisers::init_pipeline_shader_stage_create_info();
pipeline_shader_stage_create_info.module = shader_module;
pipeline_shader_stage_create_info.stage = shader.shader_type;

return pipeline_shader_stage_create_info;
}

void vulkan_shader_pipeline::create_render_pass()
{
// render pass
VkAttachmentDescription color_attachment =
{ };
color_attachment.format = swap_chain->get_format();
color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

VkAttachmentReference color_attachment_ref =
{ };
color_attachment_ref.attachment = 0;
color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

VkSubpassDescription subpass =
{ };
subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
subpass.colorAttachmentCount = 1;
subpass.pColorAttachments = &color_attachment_ref;

VkSubpassDependency dependency =
{ };
dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
dependency.dstSubpass = 0;
dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
dependency.srcAccessMask = 0;
dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT
    | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

VkRenderPassCreateInfo render_pass_create_info =
{ };
render_pass_create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
render_pass_create_info.attachmentCount = 1;
render_pass_create_info.pAttachments = &color_attachment;
render_pass_create_info.subpassCount = 1;
render_pass_create_info.pSubpasses = &subpass;
render_pass_create_info.dependencyCount = 1;
render_pass_create_info.pDependencies = &dependency;

if (vkCreateRenderPass(device_instance->get_device(), &render_pass_create_info,
                       nullptr, &render_pass) != VK_SUCCESS)
{
  throw std::runtime_error("failed to create render pass!");
}
}

void vulkan_shader_pipeline::create_pipeline_layout()
{
/*VkDynamicState dynamic_states[] =
 { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_LINE_WIDTH };

 VkPipelineDynamicStateCreateInfo dynamic_state =
 { };
 dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
 dynamic_state.dynamicStateCount = 2;
 dynamic_state.pDynamicStates = dynamic_states;*/
VkPipelineLayoutCreateInfo pipeline_layout_info =
{ };
pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
pipeline_layout_info.setLayoutCount = 0;  // Optional
pipeline_layout_info.pSetLayouts = nullptr;  // Optional
pipeline_layout_info.pushConstantRangeCount = 0;  // Optional
pipeline_layout_info.pPushConstantRanges = nullptr;  // Optional
if (vkCreatePipelineLayout(device_instance->get_device(), &pipeline_layout_info,
                           nullptr, &pipeline_layout) != VK_SUCCESS)
{
  throw std::runtime_error("failed to create pipeline layout!");
}
}

auto vulkan_shader_pipeline::create_vertex_input_info() -> VkPipelineVertexInputStateCreateInfo
{
auto vertex_input_info = initialisers::init_vertex_input_create_info();

vertex_input_info.vertexBindingDescriptionCount = 0;
vertex_input_info.pVertexBindingDescriptions = nullptr;  // Optional
vertex_input_info.vertexAttributeDescriptionCount = 0;
vertex_input_info.pVertexAttributeDescriptions = nullptr;  // Optional

return vertex_input_info;
}

VkPipelineInputAssemblyStateCreateInfo vulkan_shader_pipeline::create_input_assembly()
{
VkPipelineInputAssemblyStateCreateInfo input_assembly =
{ };
input_assembly.sType =
    VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
input_assembly.primitiveRestartEnable = VK_FALSE;
return input_assembly;
}

VkViewport vulkan_shader_pipeline::create_viewport()
{
VkViewport viewport =
{ };
viewport.x = 0.0f;
viewport.y = 0.0f;
viewport.width = (float) (swap_chain->get_extent().width);
viewport.height = (float) (swap_chain->get_extent().height);
viewport.minDepth = 0.0f;
viewport.maxDepth = 1.0f;
return viewport;
}

VkPipelineRasterizationStateCreateInfo vulkan_shader_pipeline::create_rasterizer()
{
VkPipelineRasterizationStateCreateInfo rasterizer =
{ };
rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
rasterizer.depthClampEnable = VK_FALSE;
rasterizer.rasterizerDiscardEnable = VK_FALSE;
rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
rasterizer.lineWidth = 1.0f;
rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
rasterizer.depthBiasEnable = VK_FALSE;
rasterizer.depthBiasConstantFactor = 0.0f;  // Optional
rasterizer.depthBiasClamp = 0.0f;  // Optional
rasterizer.depthBiasSlopeFactor = 0.0f;  // Optional
return rasterizer;
}

VkPipelineMultisampleStateCreateInfo vulkan_shader_pipeline::create_multisampling_info()
{
VkPipelineMultisampleStateCreateInfo multisampling =
{ };
multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
multisampling.sampleShadingEnable = VK_FALSE;
multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
multisampling.minSampleShading = 1.0f;  // Optional
multisampling.pSampleMask = nullptr;  // Optional
multisampling.alphaToCoverageEnable = VK_FALSE;  // Optional
multisampling.alphaToOneEnable = VK_FALSE;  // Optional
return multisampling;
}

VkPipelineColorBlendAttachmentState vulkan_shader_pipeline::create_color_blend_attachment()
{
VkPipelineColorBlendAttachmentState color_blend_attachment =
{ };
color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT
    | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT
    | VK_COLOR_COMPONENT_A_BIT;
color_blend_attachment.blendEnable = VK_FALSE;
color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;  // Optional
color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;  // Optional
color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;  // Optional
color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;  // Optional
return color_blend_attachment;
}

void vulkan_shader_pipeline::create_frame_buffers()
{
// frame buffers
frame_buffers.resize(swap_chain->get_image_views().size());
for (size_t i = 0; i < frame_buffers.size(); i++)
{
  VkImageView attachments[] =
  { swap_chain->get_image_views()[i] };
  VkFramebufferCreateInfo framebufferInfo =
  { };
  framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
  framebufferInfo.renderPass = render_pass;
  framebufferInfo.attachmentCount = 1;
  framebufferInfo.pAttachments = attachments;
  framebufferInfo.width = swap_chain->get_extent().width;
  framebufferInfo.height = swap_chain->get_extent().height;
  framebufferInfo.layers = 1;
  if (vkCreateFramebuffer(device_instance->get_device(), &framebufferInfo,
                          nullptr, &frame_buffers[i]) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create framebuffer!");
  }
}
}

void vulkan_shader_pipeline::create_command_pool()
{
//command pool
VkCommandPoolCreateInfo pool_info =
{ };
pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
pool_info.queueFamilyIndex = device_instance->get_queue_family_indices()
    .graphics;
pool_info.flags = 0;  // Optional
if (vkCreateCommandPool(device_instance->get_device(), &pool_info, nullptr,
                        &command_pool) != VK_SUCCESS)
{
  throw std::runtime_error("failed to create command pool!");
}
}

void vulkan_shader_pipeline::create_command_buffers()
{
// command buffers
command_buffers.resize(frame_buffers.size());
VkCommandBufferAllocateInfo alloc_info =
{ };
alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
alloc_info.commandPool = command_pool;
alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
alloc_info.commandBufferCount = (uint32_t) (command_buffers.size());
if (vkAllocateCommandBuffers(device_instance->get_device(), &alloc_info,
                             command_buffers.data()) != VK_SUCCESS)
{
  throw std::runtime_error("failed to allocate command buffers!");
}
}

void vulkan_shader_pipeline::record_command_buffers()
{
for (size_t i = 0; i < command_buffers.size(); i++)
{
  VkCommandBufferBeginInfo begin_info =
  { };
  begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  begin_info.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
  begin_info.pInheritanceInfo = nullptr;  // Optional

  if (vkBeginCommandBuffer(command_buffers[i], &begin_info) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to begin recording command buffer!");
  }

  // render pass
  VkRenderPassBeginInfo render_pass_begin_info =
  { };
  render_pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  render_pass_begin_info.renderPass = render_pass;
  render_pass_begin_info.framebuffer = frame_buffers[i];
  render_pass_begin_info.renderArea.offset =
  { 0, 0};
  render_pass_begin_info.renderArea.extent = swap_chain->get_extent();
  VkClearValue clearColor =
  { 0.0f, 0.0f, 0.0f, 1.0f };
  render_pass_begin_info.clearValueCount = 1;
  render_pass_begin_info.pClearValues = &clearColor;
  vkCmdBeginRenderPass(command_buffers[i], &render_pass_begin_info,
                       VK_SUBPASS_CONTENTS_INLINE);

  vkCmdBindPipeline(command_buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
                    graphics_pipeline);

  vkCmdDraw(command_buffers[i], 3, 1, 0, 0);

  vkCmdEndRenderPass(command_buffers[i]);

  if (vkEndCommandBuffer(command_buffers[i]) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to record command buffer!");
  }
}
}

}

