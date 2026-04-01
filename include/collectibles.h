#ifndef COLLECTIBLES_H
#define COLLECTIBLES_H

#include "types.h"
#include "track.h"
#include <stdbool.h>

typedef enum {
    COLLECTIBLE_TYPE_COIN,
    COLLECTIBLE_TYPE_JEWEL
} CollectibleType;

typedef struct {
    CollectibleType ctype;
    Position3D position;
    Lane lane;
    bool collected;
} Collectible;

#define MAX_COLLECTIBLES 100

typedef struct {
    Collectible items[MAX_COLLECTIBLES];
    int item_count;
    float last_spawn_z;
    float min_spacing;
} CollectibleManager;

CollectibleManager NewCollectibleManager(void);
void ResetCollectibleManager(CollectibleManager* manager);
void CollectibleManagerSpawnFromSegments(CollectibleManager* manager, TrackSegment** segments, int segment_count, GameConfig* config);
void UpdateCollectibleManager(CollectibleManager* manager, float player_z, GameConfig* config);
void CheckCollectibleCollection(CollectibleManager* manager, Lane player_lane, float player_y, float player_z, unsigned int* out_coins, unsigned int* out_jewels);
int CollectibleManagerGetVisible(CollectibleManager* manager, float player_z, float view_dist, Collectible** out_items);

#endif // COLLECTIBLES_H
