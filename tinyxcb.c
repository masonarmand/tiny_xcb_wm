/*
 * File: tinyxcb.c
 * Author: Mason Armand
 * Date Created: March 9th, 2025
 *
 * port of TinyWM from Xlib -> xcb
 *
 * Like TinyWM, tinyxcb is in the public domain and is provided AS IS, with
 * NO WARRANTY.
 * ---------------------------------------------------------------------------
 *
 * TinyWM is written by Nick Welch <nick@incise.org> in 2005 & 2011.
 *
 * This software is in the public domain
 * and is provided AS IS, with NO WARRANTY.
 * https://github.com/mackstann/tinywm/
 */
#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>
#include <X11/keysym.h>
#include <stdlib.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MODKEY XCB_MOD_MASK_1

typedef struct {
        int x, y, w, h;
        int start_x, start_y;
        unsigned int btn;
        xcb_window_t win;
} MoveInfo;

typedef struct {
        xcb_generic_event_t* ev;
        xcb_connection_t* conn;
        MoveInfo mv;
} EventInfo;


void handle_key_press(EventInfo* event)
{
        xcb_key_press_event_t* ev = (xcb_key_press_event_t*) event->ev;
        if (ev->child != XCB_NONE) {
                unsigned int values[] = { XCB_STACK_MODE_ABOVE };
                xcb_configure_window(
                        event->conn, ev->child,
                        XCB_CONFIG_WINDOW_STACK_MODE, values
                );
                xcb_flush(event->conn);
        }
}


void handle_button_press(EventInfo* event)
{
        xcb_button_press_event_t* ev = (xcb_button_press_event_t*) event->ev;
        if (ev->child != XCB_NONE) {
                xcb_get_geometry_cookie_t geom_cookie;
                xcb_get_geometry_reply_t* geom;

                event->mv.win = ev->child;
                event->mv.start_x = ev->root_x;
                event->mv.start_y = ev->root_y;
                event->mv.btn = ev->detail;

                geom_cookie = xcb_get_geometry(event->conn, event->mv.win);
                geom = xcb_get_geometry_reply(event->conn, geom_cookie, NULL);
                if (geom) {
                        event->mv.x = geom->x;
                        event->mv.y = geom->y;

                        event->mv.w = geom->width;
                        event->mv.h = geom->height;
                        free(geom);
                }
        }
}


void handle_motion_notify(EventInfo* event)
{
        xcb_motion_notify_event_t* ev = (xcb_motion_notify_event_t*) event->ev;
        MoveInfo mv = event->mv;

        if (event->mv.win != XCB_NONE) {
                int dx = ev->root_x - mv.start_x;
                int dy = ev->root_y - mv.start_y;
                /* move / resize window */
                unsigned int values[4] = {
                        mv.x + (mv.btn == 1 ? dx : 0),
                        mv.y + (mv.btn == 1 ? dy : 0),
                        MAX(1, mv.w + (mv.btn == 3 ? dx : 0)),
                        MAX(1, mv.h + (mv.btn == 3 ? dy : 0))
                };
                unsigned int mask = XCB_CONFIG_WINDOW_X
                        | XCB_CONFIG_WINDOW_Y
                        | XCB_CONFIG_WINDOW_WIDTH
                        | XCB_CONFIG_WINDOW_HEIGHT;
                xcb_configure_window(event->conn, mv.win, mask, values);
                xcb_flush(event->conn);
        }
}


void handle_button_release(EventInfo* event)
{
        event->mv.win = XCB_NONE;
        xcb_flush(event->conn);
}


void handle_events(xcb_connection_t* conn)
{
        xcb_generic_event_t* ev;
        EventInfo ev_info = { 0 };
        ev_info.mv.win = XCB_NONE;

        while ((ev = xcb_wait_for_event(conn))) {
                unsigned int ev_type = ev->response_type & ~0x80;
                ev_info.ev = ev;
                ev_info.conn = conn;

                switch (ev_type) {
                case XCB_KEY_PRESS:
                        handle_key_press(&ev_info);
                        break;
                case XCB_BUTTON_PRESS:
                        handle_button_press(&ev_info);
                        break;
                case XCB_MOTION_NOTIFY:
                        handle_motion_notify(&ev_info);
                        break;
                case XCB_BUTTON_RELEASE:
                        handle_button_release(&ev_info);
                        break;
                }
                free(ev);
        }
}


int main(void)
{
        xcb_connection_t* conn = xcb_connect(NULL, NULL);
        xcb_screen_t* screen;
        xcb_window_t root;

        xcb_key_symbols_t* keysyms;
        xcb_keycode_t* f1_key;

        unsigned int event_mask = XCB_EVENT_MASK_BUTTON_PRESS
                | XCB_EVENT_MASK_BUTTON_RELEASE
                | XCB_EVENT_MASK_POINTER_MOTION;

        if (xcb_connection_has_error(conn))
                return 1;

        /* get root window */
        screen = xcb_setup_roots_iterator(xcb_get_setup(conn)).data;
        root = screen->root;

        /* alloc key syms */
        keysyms = xcb_key_symbols_alloc(conn);
        f1_key = xcb_key_symbols_get_keycode(keysyms, XK_F1);

        /* grab mod+f1 key*/
        xcb_grab_key(
                conn, 1, root, MODKEY, *f1_key,
                XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC
        );
        /* grab left mouse button */
        xcb_grab_button(
                conn, 0, root, event_mask,
                XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC,
                XCB_NONE, XCB_NONE, 1, XCB_MOD_MASK_1
        );
        /* grab right mouse button */
        xcb_grab_button(
                conn, 0, root, event_mask,
                XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC,
                XCB_NONE, XCB_NONE, 3, XCB_MOD_MASK_1
        );

        xcb_flush(conn);

        handle_events(conn);

        free(f1_key);
        xcb_key_symbols_free(keysyms);
        xcb_disconnect(conn);

        return 0;
}
