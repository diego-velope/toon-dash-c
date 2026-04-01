#ifndef TYPES_H
#define TYPES_H

#include "raylib.h"
#include <stdbool.h>

typedef enum {
    LANE_LEFT = -1,
    LANE_CENTER = 0,
    LANE_RIGHT = 1
} Lane;

float LaneToX(Lane lane, float lane_width);
bool LaneNeighbor(Lane current, int direction, Lane* out_lane);

typedef struct {
    float x;
    float y;
    float z;
} Position3D;

Vector3 Position3DToVector3(Position3D pos);

typedef struct {
    Position3D min;
    Position3D max;
} BoundingBox3D;

BoundingBox3D BoundingBox3DFromCenter(Position3D center, Position3D half_size);
bool BoundingBox3DIntersects(BoundingBox3D a, BoundingBox3D b);

typedef struct {
    float lane_width;
    float player_speed;
    float jump_height;
    float jump_duration;
    float slide_duration;
    float spawn_distance;
    float despawn_distance;
    unsigned int coin_value;
} GameConfig;

GameConfig DefaultGameConfig(void);

#endif // TYPES_H
