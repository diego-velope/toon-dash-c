#ifndef RENDERER_H
#define RENDERER_H

#include "raylib.h"
#include "assets.h"
#include "state.h"
#include "player.h"
#include "track.h"
#include "obstacles.h"
#include "collectibles.h"
#include "camera.h"

void DrawGame(
    GameAssets* assets, GameState* state, GameSettings* settings, 
    Player* player, Track* track, ObstacleManager* obs, 
    CollectibleManager* cols, GameCamera* cam, CharacterChoice choice,
    MenuNavigator* main_menu_nav, MenuNavigator* pause_menu_nav, MenuNavigator* game_over_nav,
    MenuSubScreen active_subscreen
);

#endif
