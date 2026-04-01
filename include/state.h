#ifndef STATE_H
#define STATE_H

#include <stdbool.h>

typedef enum {
    SCREEN_MAIN_MENU,
    SCREEN_PLAYING,
    SCREEN_PAUSED,
    SCREEN_GAME_OVER
} GameScreen;

typedef enum {
    SUB_SCREEN_NONE,
    SUB_SCREEN_HOW_TO_PLAY,
    SUB_SCREEN_OPTIONS,
    SUB_SCREEN_CHARACTER_SELECT
} MenuSubScreen;

typedef struct {
    unsigned char master_volume;
    unsigned char music_volume;
    unsigned char effects_volume;
    unsigned char game_speed;
    int focused_row;
} GameSettings;

GameSettings DefaultGameSettings(void);
float GameSettingsMasterF32(GameSettings* settings);
float GameSettingsMusicF32(GameSettings* settings);
float GameSettingsEffectsF32(GameSettings* settings);
float GameSettingsSpeedF32(GameSettings* settings);

typedef enum {
    CHAR_OODI,
    CHAR_OOLI,
    CHAR_OOZI,
    CHAR_OOPI,
    CHAR_OOBI
} CharacterChoice;

CharacterChoice DefaultCharacterChoice(void);
const char* CharacterChoiceMeshKey(CharacterChoice choice);
const char* CharacterChoiceDisplayName(CharacterChoice choice);
CharacterChoice CharacterChoiceNext(CharacterChoice choice);
CharacterChoice CharacterChoicePrev(CharacterChoice choice);

typedef struct {
    GameScreen screen;
    float score;
    float high_score;
    float distance;
    unsigned int coins;
    unsigned int combo;
    float combo_anim_timer;
} GameState;

GameState NewGameState(void);
void GameStateStartGame(GameState* state);
void GameStatePause(GameState* state);
void GameStateResume(GameState* state);
void GameStateGameOver(GameState* state);
void GameStateReturnToMenu(GameState* state);
void GameStateUpdateScore(GameState* state, float dt, float distance, unsigned int coins);
void GameStateAddCollectiblePoints(GameState* state, bool is_jewel);

typedef enum {
    MENU_OPT_PLAY,
    MENU_OPT_HOW_TO_PLAY,
    MENU_OPT_OPTIONS,
    MENU_OPT_QUIT,
    MENU_OPT_COUNT
} MenuOption;

typedef enum {
    PAUSE_OPT_RESUME,
    PAUSE_OPT_RESTART,
    PAUSE_OPT_OPTIONS,
    PAUSE_OPT_QUIT,
    PAUSE_OPT_COUNT
} PauseOption;

typedef enum {
    GAMEOVER_OPT_RESTART,
    GAMEOVER_OPT_QUIT,
    GAMEOVER_OPT_COUNT
} GameOverOption;

typedef struct {
    int selected;
    int option_count;
} MenuNavigator;

void MenuNavigatorUp(MenuNavigator* nav);
void MenuNavigatorDown(MenuNavigator* nav);
MenuNavigator DefaultMenuNavigator(int count);

unsigned int LoadHighscore(void);
void SaveHighscore(unsigned int score);

#endif // STATE_H
