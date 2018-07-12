/*
 * xcb_window_handler.cpp
 *
 *  Created on: Jun 22, 2018
 *      Author: admin
 */

#ifdef VK_USE_PLATFORM_XCB_KHR
#include <cassert>
#include <iostream>
#include "xcb_window_handler.hpp"

namespace tobivulkan
{

xcb_window_handler::xcb_window_handler(uint32_t width, uint32_t height)
    : connection(nullptr),
      screen(nullptr),
      window(0),
      atom_wm_delete_window(nullptr),
      width(width),
      height(height)
{

  initialize_window();

  std::cout << ">>> Constructed xcb_window_handler " << std::endl;
}

xcb_window_handler::xcb_window_handler(xcb_window_handler&& other)
    : connection(other.connection),
      screen(other.screen),
      window(other.window),
      atom_wm_delete_window(other.atom_wm_delete_window),
      width(other.width),
      height(other.height),
      pause(false),
      quit(false)
{
  other.connection = nullptr;
  other.screen = nullptr;
  other.window = 0;
  other.atom_wm_delete_window = nullptr;
  other.width = 0;
  other.height = 0;
  std::cout << "/// Moved xcb_window_handler " << std::endl;
}

xcb_window_handler& xcb_window_handler::operator=(xcb_window_handler&& other)
{
  if (this != &other)
  {
    xcb_destroy_window(connection, window);
    xcb_disconnect(connection);
    connection = other.connection;
    screen = other.screen;
    window = other.window;
    atom_wm_delete_window = other.atom_wm_delete_window;
    width = other.width;
    height = other.height;

    other.screen = nullptr;
    other.connection = nullptr;
    other.atom_wm_delete_window = nullptr;
    other.window = 0;
    other.width = 0;
    other.height = 0;
  }
  std::cout << "///= Moved xcb_window_handler" << std::endl;

  return *this;
}

xcb_window_handler::~xcb_window_handler()
{
  xcb_destroy_window(connection, window);
  xcb_disconnect(connection);

  free(atom_wm_delete_window);

  std::cout << "<<< Deconstructed xcb_window_handler" << std::endl;
}

void xcb_window_handler::handle_xcb_event(const xcb_generic_event_t *event) {
    uint8_t event_code = event->response_type & 0x7f;
    switch (event_code) {
        case XCB_EXPOSE:
            // TODO: Resize window
            break;
        case XCB_CLIENT_MESSAGE:
            if ((*(xcb_client_message_event_t *)event).data.data32[0] == (*atom_wm_delete_window).atom) {
                quit = true;
            }
            break;
        case XCB_KEY_RELEASE: {
            const xcb_key_release_event_t *key = (const xcb_key_release_event_t *)event;

            switch (key->detail) {
                case 0x9:  // Escape
                    quit = true;
                    break;
                /*case 0x71:  // left arrow key
                    spin_angle -= spin_increment;
                    break;
                case 0x72:  // right arrow key
                    spin_angle += spin_increment;
                    break;*/
                case 0x41:  // space bar
                    pause = !pause;
                    break;
            }
        } break;
        case XCB_CONFIGURE_NOTIFY: {
            const xcb_configure_notify_event_t *cfg = (const xcb_configure_notify_event_t *)event;
            if ((width != cfg->width) || (height != cfg->height)) {
                width = cfg->width;
                height = cfg->height;
                this->change_window_size(width, height);
            }
        } break;
        default:
            break;
    }
}
void xcb_window_handler::update()
{
  xcb_generic_event_t *event;
  if(pause)
    event = xcb_wait_for_event(connection);
  else
    event = xcb_poll_for_event(connection);

  while (event)
  {
    handle_xcb_event (event);
    free(event);
    event = xcb_poll_for_event(connection);
  }

}

void xcb_window_handler::initialize_window()
{

  // TODO: not assert.. should throw exception instead.
  assert(width > 0);
  assert(height > 0);

  // start by establishing a connection
  init_connection();

  window = xcb_generate_id(connection);

  uint32_t value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
  uint32_t value_list[32];
  value_list[0] = screen->black_pixel;
  value_list[1] = XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_EXPOSURE;

  xcb_create_window(connection, XCB_COPY_FROM_PARENT, window, screen->root, 0,
                    0, width, height, 0, XCB_WINDOW_CLASS_INPUT_OUTPUT,
                    screen->root_visual, value_mask, value_list);

  // Magic code that will send notification when window is destroyed
  xcb_intern_atom_cookie_t cookie = xcb_intern_atom(connection, 1, 12,
                                                    "WM_PROTOCOLS");
  xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(connection, cookie, 0);

  xcb_intern_atom_cookie_t cookie2 = xcb_intern_atom(connection, 0, 16,
                                                     "WM_DELETE_WINDOW");
  atom_wm_delete_window = xcb_intern_atom_reply(connection, cookie2, 0);

  xcb_change_property(connection, XCB_PROP_MODE_REPLACE, window, (*reply).atom,
                      4, 32, 1, &(*atom_wm_delete_window).atom);
  free(reply);

  xcb_map_window(connection, window);

  // Force the x/y coordinates to 100,100 results are identical in consecutive
  // runs
  const uint32_t coords[] =
  { 100, 100 };
  xcb_configure_window(connection, window,
                       XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y, coords);
  xcb_flush(connection);

  xcb_generic_event_t *e;
  while ((e = xcb_wait_for_event(connection)))
  {
    if ((e->response_type & ~0x80) == XCB_EXPOSE)
      break;
  }

  free(e);
  std::cout << "ooo initialized xcb_window_handler" << std::endl;
}

void xcb_window_handler::init_connection()
{
  int scr;
  connection = xcb_connect(nullptr, &scr);

  if (connection == nullptr || xcb_connection_has_error(connection))
  {
    std::cout << "Unable to make an XCB connection" << std::endl;
    exit(-1);
  }

  const xcb_setup_t *setup = xcb_get_setup(connection);
  xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
  while (scr-- > 0)
    xcb_screen_next(&iter);

  screen = iter.data;
}

}  // namspace tobivulkan

#endif /* VK_USE_PLATFORM_XCB_KHR */
