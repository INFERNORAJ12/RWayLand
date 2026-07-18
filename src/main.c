#define _POSIX_C_SOURCE 200809L

#include "shield.h"
#include <stdlib.h>
#include <time.h>
#include <wlr/util/log.h>
#include <xkbcommon/xkbcommon.h> // Translates keycodes to symbols (like "Esc")

// --- 1. Keyboard Input Logic ---
static void keyboard_handle_key(struct wl_listener *listener, void *data) {
    struct shield_keyboard *keyboard = wl_container_of(listener, keyboard, key);
    struct shield_server *server = keyboard->server;
    struct wlr_keyboard_key_event *event = data;

    // Translate the raw keycode into a usable symbol
    uint32_t keycode = event->keycode + 8;
    const xkb_keysym_t *syms;
    int nsyms = xkb_state_key_get_syms(keyboard->wlr_keyboard->xkb_state, keycode, &syms);

    // If Alt + Esc is pressed, terminate the compositor safely
    uint32_t modifiers = wlr_keyboard_get_modifiers(keyboard->wlr_keyboard);
    if (event->state == WL_KEYBOARD_KEY_STATE_PRESSED) {
        for (int i = 0; i < nsyms; i++) {
            if (syms[i] == XKB_KEY_Escape && (modifiers & WLR_MODIFIER_ALT)) {
                wl_display_terminate(server->wl_display);
            }
        }
    }
}

static void server_new_input(struct wl_listener *listener, void *data) {
    struct shield_server *server = wl_container_of(listener, server, new_input);
    struct wlr_input_device *device = data;

    if (device->type == WLR_INPUT_DEVICE_KEYBOARD) {
        struct shield_keyboard *keyboard = calloc(1, sizeof(struct shield_keyboard));
        keyboard->server = server;
        keyboard->wlr_keyboard = wlr_keyboard_from_input_device(device);

        // Load the default keyboard layout map
        struct xkb_context *context = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
        struct xkb_keymap *keymap = xkb_keymap_new_from_names(context, NULL, XKB_KEYMAP_COMPILE_NO_FLAGS);
        wlr_keyboard_set_keymap(keyboard->wlr_keyboard, keymap);
        xkb_keymap_unref(keymap);
        xkb_context_unref(context);

        // Listen for key presses
        keyboard->key.notify = keyboard_handle_key;
        wl_signal_add(&keyboard->wlr_keyboard->events.key, &keyboard->key);

        wlr_seat_set_keyboard(server->seat, keyboard->wlr_keyboard);
        wl_list_insert(&server->keyboards, &keyboard->link);
    }
}

// --- 2. Window Management Logic ---
static void server_new_xdg_surface(struct wl_listener *listener, void *data) {
    struct shield_server *server = wl_container_of(listener, server, new_xdg_surface);
    struct wlr_xdg_surface *xdg_surface = data;

    // If a new window opens, attach it to our Scene Graph so it gets drawn
    if (xdg_surface->role == WLR_XDG_SURFACE_ROLE_TOPLEVEL) {
        wlr_scene_xdg_surface_create(&server->scene->tree, xdg_surface);
    }
}

// --- 3. Screen Rendering Logic ---
static void output_frame(struct wl_listener *listener, void *data) {
    struct shield_output *output = wl_container_of(listener, output, frame);
    struct wlr_scene_output *scene_output = wlr_scene_get_scene_output(output->server->scene, output->wlr_output);

    // Render the scene graph to the monitor
    wlr_scene_output_commit(scene_output, NULL);

    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    wlr_scene_output_send_frame_done(scene_output, &now);
}

static void server_new_output(struct wl_listener *listener, void *data) {
    struct shield_server *server = wl_container_of(listener, server, new_output);
    struct wlr_output *wlr_output = data;

    wlr_output_init_render(wlr_output, server->allocator, server->renderer);

    struct shield_output *output = calloc(1, sizeof(struct shield_output));
    output->wlr_output = wlr_output;
    output->server = server;

    // Listen for the frame event (when the screen is ready for a new picture)
    output->frame.notify = output_frame;
    wl_signal_add(&wlr_output->events.frame, &output->frame);

    // Set output to enabled
    struct wlr_output_state state;
    wlr_output_state_init(&state);
    wlr_output_state_set_enabled(&state, true);
    wlr_output_commit_state(wlr_output, &state);
    wlr_output_state_finish(&state);

    // Attach output to the scene graph
    wlr_scene_output_create(server->scene, wlr_output);
    wl_list_insert(&server->outputs, &output->link);
}

// --- Main Initialization ---
int main(void) {
    wlr_log_init(WLR_DEBUG, NULL);
    struct shield_server server = {0};
    wl_list_init(&server.outputs);
    wl_list_init(&server.keyboards);

    server.wl_display = wl_display_create();
    server.backend = wlr_backend_autocreate(server.wl_display, NULL);
    
    server.renderer = wlr_renderer_autocreate(server.backend);
    wlr_renderer_init_wl_display(server.renderer, server.wl_display);
    server.allocator = wlr_allocator_autocreate(server.backend, server.renderer);
    server.scene = wlr_scene_create();

    // Many Wayland apps crash if a subcompositor doesn't exist
    wlr_subcompositor_create(server.wl_display);

    // Setup Outputs (Monitors)
    server.new_output.notify = server_new_output;
    wl_signal_add(&server.backend->events.new_output, &server.new_output);

    // Setup Window Management (XDG Shell)
    server.xdg_shell = wlr_xdg_shell_create(server.wl_display, 3);
    server.new_xdg_surface.notify = server_new_xdg_surface;
    wl_signal_add(&server.xdg_shell->events.new_surface, &server.new_xdg_surface);

    // Setup Inputs (Seat)
    server.seat = wlr_seat_create(server.wl_display, "seat0");
    server.new_input.notify = server_new_input;
    wl_signal_add(&server.backend->events.new_input, &server.new_input);

    const char *socket = wl_display_add_socket_auto(server.wl_display);
    if (!wlr_backend_start(server.backend)) return 1;
    wlr_log(WLR_INFO, "Running compositor on WAYLAND_DISPLAY=%s", socket);

    wl_display_run(server.wl_display);

    wl_display_destroy_clients(server.wl_display);
    wl_display_destroy(server.wl_display);
    return 0;
}