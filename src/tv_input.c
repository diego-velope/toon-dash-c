#include "tv_input.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

// Global state for web TV interop
static TvInputManager g_tv_input;

void InitTvInputManager(void) {
    for (int i = 0; i < TV_ACTION_COUNT; i++) {
        g_tv_input.current_state[i] = false;
        g_tv_input.previous_state[i] = false;
    }
}

TvInputManager* GetTvInputManager(void) {
    return &g_tv_input;
}

void UpdateTvInputManager(void) {
    for (int i = 0; i < TV_ACTION_COUNT; i++) {
        g_tv_input.previous_state[i] = g_tv_input.current_state[i];
    }
}

bool TvInputIsActionPressed(TvAction action) {
    return g_tv_input.current_state[action];
}

bool TvInputIsActionJustPressed(TvAction action) {
    return g_tv_input.current_state[action] && !g_tv_input.previous_state[action];
}

void TvInputClear(void) {
    for (int i = 0; i < TV_ACTION_COUNT; i++) {
        g_tv_input.current_state[i] = false;
        g_tv_input.previous_state[i] = false;
    }
}

// ---------------------------------------------------------
// WASM interface for JavaScript tv-pal.js
// By using EMSCRIPTEN_KEEPALIVE, Emscripten automatically adds these 
// to its exported functions list, so `window._mq_handle_up` works safely.
// ---------------------------------------------------------

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE void mq_handle_up(int pressed) { g_tv_input.current_state[TV_ACTION_UP] = (pressed != 0); }
EMSCRIPTEN_KEEPALIVE void mq_handle_down(int pressed) { g_tv_input.current_state[TV_ACTION_DOWN] = (pressed != 0); }
EMSCRIPTEN_KEEPALIVE void mq_handle_left(int pressed) { g_tv_input.current_state[TV_ACTION_LEFT] = (pressed != 0); }
EMSCRIPTEN_KEEPALIVE void mq_handle_right(int pressed) { g_tv_input.current_state[TV_ACTION_RIGHT] = (pressed != 0); }
EMSCRIPTEN_KEEPALIVE void mq_handle_action(int pressed) { g_tv_input.current_state[TV_ACTION_ACTION] = (pressed != 0); }
EMSCRIPTEN_KEEPALIVE void mq_handle_back(int pressed) { g_tv_input.current_state[TV_ACTION_BACK] = (pressed != 0); }
#else
// Mock for native Desktop compilation if needed
void mq_handle_up(int pressed) {}
void mq_handle_down(int pressed) {}
void mq_handle_left(int pressed) {}
void mq_handle_right(int pressed) {}
void mq_handle_action(int pressed) {}
void mq_handle_back(int pressed) {}
#endif
