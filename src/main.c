#include "raylib.h"
#include "types.h"
#include "state.h"
#include "player.h"
#include "track.h"
#include "obstacles.h"
#include "collectibles.h"
#include "camera.h"
#include "assets.h"
#include "renderer.h"
#include "tv_input.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

// Global game state structures required by Emscripten
GameAssets assets;
GameState state;
GameSettings settings;
GameConfig config;
Player player;
Track track;
ObstacleManager obs_manager;
CollectibleManager col_manager;
GameCamera g_camera;
CharacterChoice char_choice;

MenuNavigator main_menu_nav;
MenuNavigator pause_menu_nav;
MenuNavigator game_over_nav;
MenuSubScreen active_subscreen;

void ResetRun(void) {
    GameStateStartGame(&state);
    ResetPlayer(&player);
    ResetTrack(&track);
    ResetObstacleManager(&obs_manager);
    ResetCollectibleManager(&col_manager);
    g_camera = NewGameCamera();
}

void UpdateFrame(void) {
    float dt = GetFrameTime();
    
    // Fallback Desktop input processing if not TV WASM bridging
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) mq_handle_up(1);
    if (IsKeyReleased(KEY_UP) || IsKeyReleased(KEY_W)) mq_handle_up(0);
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) mq_handle_down(1);
    if (IsKeyReleased(KEY_DOWN) || IsKeyReleased(KEY_S)) mq_handle_down(0);
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) mq_handle_left(1);
    if (IsKeyReleased(KEY_LEFT) || IsKeyReleased(KEY_A)) mq_handle_left(0);
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) mq_handle_right(1);
    if (IsKeyReleased(KEY_RIGHT) || IsKeyReleased(KEY_D)) mq_handle_right(0);
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) mq_handle_action(1);
    if (IsKeyReleased(KEY_ENTER) || IsKeyReleased(KEY_SPACE)) mq_handle_action(0);
    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_BACKSPACE)) mq_handle_back(1);
    if (IsKeyReleased(KEY_ESCAPE) || IsKeyReleased(KEY_BACKSPACE)) mq_handle_back(0);

    if (state.screen == SCREEN_MAIN_MENU) {
        if (TvInputIsActionJustPressed(TV_ACTION_UP)) MenuNavigatorUp(&main_menu_nav);
        if (TvInputIsActionJustPressed(TV_ACTION_DOWN)) MenuNavigatorDown(&main_menu_nav);
        if (TvInputIsActionJustPressed(TV_ACTION_ACTION)) {
            if (main_menu_nav.selected == MENU_OPT_PLAY) ResetRun();
            // other options stubbed
        }
    } else if (state.screen == SCREEN_PLAYING) {
        if (TvInputIsActionJustPressed(TV_ACTION_BACK)) {
            GameStatePause(&state);
        } else {
            if (TvInputIsActionJustPressed(TV_ACTION_LEFT)) PlayerChangeLane(&player, -1);
            if (TvInputIsActionJustPressed(TV_ACTION_RIGHT)) PlayerChangeLane(&player, 1);
            if (TvInputIsActionJustPressed(TV_ACTION_UP) || TvInputIsActionJustPressed(TV_ACTION_ACTION)) PlayerJump(&player, &config);
            if (TvInputIsActionJustPressed(TV_ACTION_DOWN)) PlayerSlide(&player, &config);
            
            UpdatePlayer(&player, dt, dt, &config);
            UpdateTrack(&track, player.position.z, &config);
            
            TrackSegment* segments[MAX_TRACK_SEGMENTS];
            int count = TrackGetVisible(&track, player.position.z, config.spawn_distance, segments);
            ObstacleManagerSpawnFromSegments(&obs_manager, segments, count, &config);
            CollectibleManagerSpawnFromSegments(&col_manager, segments, count, &config);
            
            UpdateObstacleManager(&obs_manager, player.position.z, &config);
            UpdateCollectibleManager(&col_manager, player.position.z, &config);
            
            UpdateGameCamera(&g_camera, player.position, dt);
            
            unsigned int c_coins, c_jewels;
            CheckCollectibleCollection(&col_manager, player.lane, player.position.y, player.position.z, &c_coins, &c_jewels);
            if (c_coins > 0 || c_jewels > 0) {
                GameStateAddCollectiblePoints(&state, c_jewels > 0);
            }
            
            GameStateUpdateScore(&state, dt, player.position.z, state.coins + c_coins);
            
            BoundingBox3D pbox = PlayerGetBoundingBox(&player);
            Obstacle* hit = CheckObstacleCollision(&obs_manager, pbox, player.lane, PlayerIsAirborne(&player), PlayerIsSliding(&player));
            if (hit) {
                PlayerDie(&player);
                GameStateGameOver(&state);
            }
        }
    } else if (state.screen == SCREEN_PAUSED) {
        if (TvInputIsActionJustPressed(TV_ACTION_UP)) MenuNavigatorUp(&pause_menu_nav);
        if (TvInputIsActionJustPressed(TV_ACTION_DOWN)) MenuNavigatorDown(&pause_menu_nav);
        if (TvInputIsActionJustPressed(TV_ACTION_ACTION)) {
            if (pause_menu_nav.selected == PAUSE_OPT_RESUME) GameStateResume(&state);
            if (pause_menu_nav.selected == PAUSE_OPT_RESTART) ResetRun();
            if (pause_menu_nav.selected == PAUSE_OPT_QUIT) GameStateReturnToMenu(&state);
        }
        if (TvInputIsActionJustPressed(TV_ACTION_BACK)) {
            GameStateResume(&state);
        }
    } else if (state.screen == SCREEN_GAME_OVER) {
        if (TvInputIsActionJustPressed(TV_ACTION_UP)) MenuNavigatorUp(&game_over_nav);
        if (TvInputIsActionJustPressed(TV_ACTION_DOWN)) MenuNavigatorDown(&game_over_nav);
        if (TvInputIsActionJustPressed(TV_ACTION_ACTION)) {
            if (game_over_nav.selected == GAMEOVER_OPT_RESTART) ResetRun();
            if (game_over_nav.selected == GAMEOVER_OPT_QUIT) GameStateReturnToMenu(&state);
        }
    }

    UpdateTvInputManager();
    
    DrawGame(&assets, &state, &settings, &player, &track, &obs_manager, 
             &col_manager, &g_camera, char_choice, &main_menu_nav, 
             &pause_menu_nav, &game_over_nav, active_subscreen);
}

int main(void) {
    InitWindow(1280, 720, "Toon Dash");
    
    InitTvInputManager();
    state = NewGameState();
    settings = DefaultGameSettings();
    config = DefaultGameConfig();
    char_choice = DefaultCharacterChoice();
    player = NewPlayer();
    track = NewTrack();
    obs_manager = NewObstacleManager();
    col_manager = NewCollectibleManager();
    g_camera = NewGameCamera();
    
    main_menu_nav = DefaultMenuNavigator(MENU_OPT_COUNT);
    pause_menu_nav = DefaultMenuNavigator(PAUSE_OPT_COUNT);
    game_over_nav = DefaultMenuNavigator(GAMEOVER_OPT_COUNT);
    active_subscreen = SUB_SCREEN_NONE;

    LoadGameAssets(&assets);

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(UpdateFrame, 0, 1);
#else
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        UpdateFrame();
    }
#endif

    UnloadGameAssets(&assets);
    CloseWindow();
    return 0;
}
