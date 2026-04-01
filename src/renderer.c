#include "renderer.h"
#include <stdio.h>
#include <math.h>

static void DrawTextCentered(Font font, const char* text, float y, float size, Color color, int screen_width) {
    Vector2 tsize = MeasureTextEx(font, text, size, 0);
    DrawTextEx(font, text, (Vector2){(float)screen_width/2.0f - tsize.x/2.0f, y}, size, 0, color);
}

void DrawGame(
    GameAssets* assets, GameState* state, GameSettings* settings, 
    Player* player, Track* track, ObstacleManager* obs, 
    CollectibleManager* cols, GameCamera* cam, CharacterChoice choice,
    MenuNavigator* main_menu_nav, MenuNavigator* pause_menu_nav, MenuNavigator* game_over_nav,
    MenuSubScreen active_subscreen
) {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    BeginDrawing();
    ClearBackground(SKYBLUE);

    if (state->screen == SCREEN_PLAYING || state->screen == SCREEN_PAUSED || state->screen == SCREEN_GAME_OVER) {
        
        BeginMode3D(cam->camera);
        
        TrackSegment* segments[MAX_TRACK_SEGMENTS];
        int vis_seg = TrackGetVisible(track, player->position.z, 200.0f, segments);
        for (int i = 0; i < vis_seg; i++) {
            Vector3 pos = { 0.0f, 0.0f, segments[i]->z_position };
            DrawModelEx(assets->road_straight, pos, (Vector3){0,1,0}, 0.0f, (Vector3){1.5f, 1.0f, 1.0f}, WHITE);
        }

        if (state->screen != SCREEN_GAME_OVER || player->state != PLAYER_STATE_DEAD) {
            Model* pmodel = GetCharacterModel(assets, choice);
            Vector3 ppos = (Vector3){player->position.x, player->position.y, player->position.z};
            
            float wiggle = 0.0f;
            if (player->state == PLAYER_STATE_RUNNING) {
                wiggle = sinf(player->animation_tick * 15.0f) * 15.0f;
            }
            DrawModelEx(*pmodel, ppos, (Vector3){0, 1, 0}, 180.0f + wiggle, (Vector3){0.8f, 0.8f, 0.8f}, WHITE);
        }

        Obstacle* visible_obs[MAX_OBSTACLES];
        int obs_count = ObstacleManagerGetVisible(obs, player->position.z, 150.0f, visible_obs);
        for (int i = 0; i < obs_count; i++) {
            Obstacle* o = visible_obs[i];
            Vector3 opos = {o->position.x, 0.0f, o->position.z};
            Model* m = &assets->spike_block;
            if (o->obstacle_type == OBSTACLE_TYPE_HIGH_BARRIER) m = &assets->fence_low;
            else if (o->obstacle_type == OBSTACLE_TYPE_FULL_BARRIER) m = &assets->trap_spikes_large;
            
            DrawModel(*m, opos, 1.0f, WHITE);
        }

        Collectible* visible_cols[MAX_COLLECTIBLES];
        int col_count = CollectibleManagerGetVisible(cols, player->position.z, 150.0f, visible_cols);
        for (int i = 0; i < col_count; i++) {
            Collectible* c = visible_cols[i];
            Vector3 cpos = {c->position.x, c->position.y, c->position.z};
            float rot = GetTime() * 100.0f; 
            Model* m = (c->ctype == COLLECTIBLE_TYPE_JEWEL) ? &assets->jewel : &assets->coin;
            DrawModelEx(*m, cpos, (Vector3){0,1,0}, rot, (Vector3){1.0f,1.0f,1.0f}, WHITE);
        }

        EndMode3D();

        char score_text[64];
        sprintf(score_text, "SCORE: %05d", (int)state->score);
        DrawTextEx(assets->font, score_text, (Vector2){20, 20}, 40, 0, WHITE);
        
        char coin_text[64];
        sprintf(coin_text, "COINS: %04d", state->coins);
        DrawTextEx(assets->font, coin_text, (Vector2){20, 70}, 40, 0, GOLD);

        if (state->screen == SCREEN_PAUSED) {
            DrawRectangle(0, 0, sw, sh, (Color){0, 0, 0, 150});
            DrawTextCentered(assets->font, "PAUSED", sh * 0.2f, 80, WHITE, sw);
            
            const char* opts[] = {"RESUME", "RESTART", "OPTIONS", "QUIT"};
            for (int i = 0; i < pause_menu_nav->option_count; i++) {
                Color c = (i == pause_menu_nav->selected) ? YELLOW : GRAY;
                DrawTextCentered(assets->font, opts[i], sh * 0.4f + i * 60, 50, c, sw);
            }
        }
        
        if (state->screen == SCREEN_GAME_OVER) {
            DrawRectangle(0, 0, sw, sh, (Color){0, 0, 0, 200});
            DrawTextCentered(assets->font, "GAME OVER", sh * 0.2f, 80, RED, sw);
            
            char final_score[64];
            sprintf(final_score, "FINAL SCORE: %05d", (int)state->score);
            DrawTextCentered(assets->font, final_score, sh * 0.4f, 50, WHITE, sw);
            
            const char* opts[] = {"RESTART", "QUIT"};
            for (int i = 0; i < game_over_nav->option_count; i++) {
                Color c = (i == game_over_nav->selected) ? YELLOW : GRAY;
                DrawTextCentered(assets->font, opts[i], sh * 0.6f + i * 60, 50, c, sw);
            }
        }
    } else if (state->screen == SCREEN_MAIN_MENU) {
        DrawTexturePro(assets->bg_menu, (Rectangle){0,0,assets->bg_menu.width,assets->bg_menu.height}, (Rectangle){0,0,sw,sh}, (Vector2){0}, 0.0f, WHITE);
        
        if (active_subscreen == SUB_SCREEN_NONE) {
            DrawTextCentered(assets->font, "TOON DASH", sh * 0.15f, 100, YELLOW, sw);
            
            char hs[64];
            sprintf(hs, "HIGH SCORE: %u", (unsigned int)state->high_score);
            DrawTextCentered(assets->font, hs, sh * 0.3f, 40, WHITE, sw);

            const char* opts[] = {"PLAY", "HOW TO PLAY", "OPTIONS", "QUIT"};
            for (int i = 0; i < main_menu_nav->option_count; i++) {
                Color c = (i == main_menu_nav->selected) ? YELLOW : GRAY;
                DrawTextCentered(assets->font, opts[i], sh * 0.5f + i * 80, 60, c, sw);
            }
        }
    }

    EndDrawing();
}
