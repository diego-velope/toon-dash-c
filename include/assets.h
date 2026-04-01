#ifndef ASSETS_H
#define ASSETS_H

#include "raylib.h"
#include "state.h"
#include "obstacles.h"
#include "collectibles.h"

typedef struct {
     Model char_oodi;
     Model char_ooli;
     Model char_oozi;
     Model char_oopi;
     Model char_oobi;
     
     Model road_straight;
     Model fence_low;
     Model spike_block;
     Model spike_block_wide;
     Model trap_spikes_large;
     Model coin;
     Model jewel;
     
     Model tree;
     Model tree_pine;
     Model trunk;
     Model mushroom;
     Model rocks;

     Texture2D sky_tex;
     Texture2D bg_menu;
     
     Font font;
} GameAssets;

void LoadGameAssets(GameAssets* assets);
void UnloadGameAssets(GameAssets* assets);

Model* GetCharacterModel(GameAssets* assets, CharacterChoice choice);

#endif
