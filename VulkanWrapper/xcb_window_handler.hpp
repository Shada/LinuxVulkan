/*
 * xcb_window_handler.hpp
 *
 *  Created on: Jun 22, 2018
 *      Author: admin
 */

#ifndef TOBIVULKAN_VULKANWRAPPER_XCB_WINDOW_HANDLER_HPP_
#define TOBIVULKAN_VULKANWRAPPER_XCB_WINDOW_HANDLER_HPP_

#ifdef VK_USE_PLATFORM_XCB_KHR
#include "vulkan/vk_sdk_platform.h"

#include <vulkan/vulkan.hpp>

namespace tobivulkan
{

class xcb_window_handler
{
 public:
  xcb_window_handler(uint32_t width, uint32_t height);
  xcb_window_handler(const xcb_window_handler& other) = delete;
  xcb_window_handler(xcb_window_handler&& other);
  xcb_window_handler& operator=(const xcb_window_handler&) = delete;
  xcb_window_handler& operator=(xcb_window_handler&& other);
  ~xcb_window_handler();

  void change_window_size(uint32_t width, uint32_t height)
  {
    this->width = width;
    this->height = height;
  }

  void update();
  void handle_xcb_event(const xcb_generic_event_t *event);

  xcb_connection_t* get_connection() const
  {
    return connection;
  }
  const xcb_window_t get_window() const
  {
    return window;
  }
  const uint32_t get_height() const
  {
    return height;
  }
  const uint32_t get_width() const
  {
    return width;
  }
  bool is_quit(){return quit;}
 private:

  void initialize_window();

  void init_connection();


  xcb_connection_t *connection;
  xcb_screen_t *screen;
  xcb_window_t window;
  xcb_intern_atom_reply_t *atom_wm_delete_window;

  uint32_t width;
  uint32_t height;

  // TODO: these should be sent as events instead.
  bool pause;
  bool quit;

};

}

#endif /* VK_USE_PLATFORM_XCB_KHR */
#endif /* TOBIVULKAN_VULKANWRAPPER_XCB_WINDOW_HANDLER_HPP_ */
