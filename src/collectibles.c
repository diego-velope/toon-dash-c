#include "collectibles.h"
#include <stdlib.h>
#include <math.h>

CollectibleManager NewCollectibleManager(void) {
    CollectibleManager manager;
    ResetCollectibleManager(&manager);
    return manager;
}

void ResetCollectibleManager(CollectibleManager* manager) {
    manager->item_count = 0;
    manager->last_spawn_z = 0.0f;
    manager->min_spacing = 5.0f;
}

void CollectibleManagerSpawnFromSegments(CollectibleManager* manager, TrackSegment** segments, int segment_count, GameConfig* config) {
    for (int i = 0; i < segment_count; i++) {
        if (segments[i]->z_position > manager->last_spawn_z + manager->min_spacing) {
            
            if (manager->item_count >= MAX_COLLECTIBLES - 10) break;

            float start_z = segments[i]->z_position - 5.0f;
            Lane lane = (Lane)((rand() % 3) - 1);
            
            // Spawn a line of coins
            int coin_count = 3 + (rand() % 5);
            for (int c = 0; c < coin_count; c++) {
                if (manager->item_count >= MAX_COLLECTIBLES) break;

                float z = start_z + (float)c * 3.0f;
                manager->items[manager->item_count].position = (Position3D){ LaneToX(lane, config->lane_width), 0.5f, z };
                manager->items[manager->item_count].lane = lane;
                
                if ((rand() % 100) < 5) {
                    manager->items[manager->item_count].ctype = COLLECTIBLE_TYPE_JEWEL;
                } else {
                    manager->items[manager->item_count].ctype = COLLECTIBLE_TYPE_COIN;
                }
                
                manager->items[manager->item_count].collected = false;
                manager->item_count++;
                manager->last_spawn_z = z;
            }
        }
    }
}

void UpdateCollectibleManager(CollectibleManager* manager, float player_z, GameConfig* config) {
    int shift = 0;
    for (int i = 0; i < manager->item_count; i++) {
        if (manager->items[i].position.z < player_z - config->despawn_distance || manager->items[i].collected) {
            shift++;
        }
    }

    if (shift > 0) {
        for (int i = 0; i < manager->item_count - shift; i++) {
            manager->items[i] = manager->items[i + shift];
        }
        manager->item_count -= shift;
    }
}

void CheckCollectibleCollection(CollectibleManager* manager, Lane player_lane, float player_y, float player_z, unsigned int* out_coins, unsigned int* out_jewels) {
    *out_coins = 0;
    *out_jewels = 0;

    for (int i = 0; i < manager->item_count; i++) {
        Collectible* item = &manager->items[i];
        if (!item->collected && item->lane == player_lane) {
            float dist_z = fabs(item->position.z - player_z);
            float dist_y = fabs(item->position.y - player_y);
            
            if (dist_z < 1.0f && dist_y < 1.5f) {
                item->collected = true;
                if (item->ctype == COLLECTIBLE_TYPE_JEWEL) {
                    *out_jewels += 1;
                } else {
                    *out_coins += 1;
                }
            }
        }
    }
}

int CollectibleManagerGetVisible(CollectibleManager* manager, float player_z, float view_dist, Collectible** out_items) {
    int count = 0;
    for (int i = 0; i < manager->item_count; i++) {
        float z = manager->items[i].position.z;
        if (!manager->items[i].collected && z > player_z - 5.0f && z < player_z + view_dist) {
            out_items[count++] = &manager->items[i];
        }
    }
    return count;
}
