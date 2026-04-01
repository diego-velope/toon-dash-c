#include "state.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

GameSettings DefaultGameSettings(void) {
    GameSettings settings;
    settings.master_volume = 5;
    settings.music_volume = 10;
    settings.effects_volume = 10;
    settings.game_speed = 5;
    settings.focused_row = 0;
    return settings;
}

float GameSettingsMasterF32(GameSettings* settings) { return (float)settings->master_volume / 10.0f; }
float GameSettingsMusicF32(GameSettings* settings) { return (float)settings->music_volume / 10.0f; }
float GameSettingsEffectsF32(GameSettings* settings) { return (float)settings->effects_volume / 10.0f; }
float GameSettingsSpeedF32(GameSettings* settings) { return (float)settings->game_speed / 5.0f; }

CharacterChoice DefaultCharacterChoice(void) { return CHAR_OODI; }

const char* CharacterChoiceMeshKey(CharacterChoice choice) {
    switch (choice) {
        case CHAR_OODI: return "char_oodi";
        case CHAR_OOLI: return "char_ooli";
        case CHAR_OOZI: return "char_oozi";
        case CHAR_OOPI: return "char_oopi";
        case CHAR_OOBI: return "char_oobi";
    }
    return "char_oodi";
}

const char* CharacterChoiceDisplayName(CharacterChoice choice) {
    switch (choice) {
        case CHAR_OODI: return "OODI";
        case CHAR_OOLI: return "OOLI";
        case CHAR_OOZI: return "OOZI";
        case CHAR_OOPI: return "OOPI";
        case CHAR_OOBI: return "OOBI";
    }
    return "OODI";
}

CharacterChoice CharacterChoiceNext(CharacterChoice choice) {
    return (CharacterChoice)(((int)choice + 1) % 5);
}

CharacterChoice CharacterChoicePrev(CharacterChoice choice) {
    if (choice == CHAR_OODI) return CHAR_OOBI;
    return (CharacterChoice)((int)choice - 1);
}

GameState NewGameState(void) {
    GameState state;
    state.screen = SCREEN_MAIN_MENU;
    state.score = 0.0f;
    state.high_score = (float)LoadHighscore();
    state.distance = 0.0f;
    state.coins = 0;
    state.combo = 1;
    state.combo_anim_timer = 0.0f;
    return state;
}

void GameStateStartGame(GameState* state) {
    state->screen = SCREEN_PLAYING;
    state->score = 0.0f;
    state->distance = 0.0f;
    state->coins = 0;
    state->combo = 1;
    state->combo_anim_timer = 0.0f;
}

void GameStatePause(GameState* state) {
    if (state->screen == SCREEN_PLAYING) state->screen = SCREEN_PAUSED;
}

void GameStateResume(GameState* state) {
    if (state->screen == SCREEN_PAUSED) state->screen = SCREEN_PLAYING;
}

void GameStateGameOver(GameState* state) {
    state->screen = SCREEN_GAME_OVER;
    if (state->score > state->high_score) {
        state->high_score = state->score;
        SaveHighscore((unsigned int)state->high_score);
    }
}

void GameStateReturnToMenu(GameState* state) {
    state->screen = SCREEN_MAIN_MENU;
}

void GameStateUpdateScore(GameState* state, float dt, float distance, unsigned int coins) {
    float dist_diff = distance - state->distance;
    state->distance = distance;
    state->coins = coins;

    if (dist_diff > 0.0f) {
        state->score += dist_diff * (float)state->combo;
    }

    if (state->combo_anim_timer > 0.0f) {
        state->combo_anim_timer -= dt;
    }
}

void GameStateAddCollectiblePoints(GameState* state, bool is_jewel) {
    if (is_jewel) {
        state->score += 200.0f;
        state->combo += 1;
        state->combo_anim_timer = 0.8f;
    } else {
        state->score += 100.0f;
    }
}

MenuNavigator DefaultMenuNavigator(int count) {
    MenuNavigator nav;
    nav.selected = 0;
    nav.option_count = count;
    return nav;
}

void MenuNavigatorUp(MenuNavigator* nav) {
    if (nav->selected > 0) nav->selected -= 1;
}

void MenuNavigatorDown(MenuNavigator* nav) {
    if (nav->selected < nav->option_count - 1) nav->selected += 1;
}

unsigned int LoadHighscore(void) {
#ifdef __EMSCRIPTEN__
    return (unsigned int)EM_ASM_INT({
        var score = localStorage.getItem("toon_dash_highscore");
        return score ? parseInt(score, 10) : 0;
    });
#else
    unsigned int score = 0;
    FILE* f = fopen("highscore.dat", "r");
    if (f) {
        fscanf(f, "%u", &score);
        fclose(f);
    }
    return score;
#endif
}

void SaveHighscore(unsigned int score) {
#ifdef __EMSCRIPTEN__
    EM_ASM_({
        localStorage.setItem("toon_dash_highscore", $0.toString());
    }, score);
#else
    FILE* f = fopen("highscore.dat", "w");
    if (f) {
        fprintf(f, "%u", score);
        fclose(f);
    }
#endif
}
