#ifndef SHIELD_H
#define SHIELD_H

#include <wayland-server-core.h>
#include <wlr/backend.h>
#include <wlr/render/allocator.h>
#include <wlr/render/wlr_renderer.h>
#include <wlr/types/wlr_scene.h>
#include <wlr/types/wlr_xdg_shell.h>
#include <wlr/types/wlr_seat.h>
#include <wlr/types/wlr_keyboard.h>
#include <wlr/types/wlr_input_device.h>
#include <wlr/types/wlr_output.h>
#include <wlr/types/wlr_subcompositor.h>

// Global compositor state
struct shield_server {
    struct wl_display *wl_display;
    struct wlr_backend *backend;
    
    struct wlr_renderer *renderer;
    struct wlr_allocator *allocator;
    struct wlr_scene *scene;

    // Window Management
    struct wlr_xdg_shell *xdg_shell;
    struct wl_listener new_xdg_surface;

    // Keyboard & Mouse Inputs
    struct wlr_seat *seat;
    struct wl_listener new_input;
    struct wl_list keyboards;

    // Outputs (Monitors / Nested Windows)
    struct wl_listener new_output;
    struct wl_list outputs;
};

// Represents a physical monitor or a nested Wayland window
struct shield_output {
    struct wl_list link;
    struct shield_server *server;
    struct wlr_output *wlr_output;
    struct wl_listener frame;
    struct wl_listener destroy;
};

// Represents a connected keyboard
struct shield_keyboard {
    struct wl_list link;
    struct shield_server *server;
    struct wlr_keyboard *wlr_keyboard;
    struct wl_listener key;
    struct wl_listener destroy;
};

#endif