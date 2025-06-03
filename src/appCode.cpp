#include "appCode.hpp"
#include <GLFW/glfw3.h>
#ifdef DEBUG
#include <iostream>
#endif

void process_keyboard_input(GameState* game_state, int32_t key_pressed) {
    if (!game_state) {
        std::cerr << "No game state found\n";
        return;
    }

    switch (key_pressed) {
        case GLFW_KEY_LEFT: {
            game_state->transforms[2].transforms[0].translate_acc({ -PADDLE_MOVE_STEP, 0.0f, 0.0f });
        } break;
        case GLFW_KEY_RIGHT: {
            game_state->transforms[2].transforms[0].translate_acc({ +PADDLE_MOVE_STEP, 0.0f, 0.0f });
        } break;
        case GLFW_KEY_UP: {
            game_state->transforms[2].transforms[0].translate_acc({ 0.0f, +PADDLE_MOVE_STEP, 0.0f });
        } break;
        case GLFW_KEY_DOWN: {
            game_state->transforms[2].transforms[0].translate_acc({ 0.0f, -PADDLE_MOVE_STEP, 0.0f });
        } break;
    }
}
