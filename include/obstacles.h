#ifndef OBSTACLES_H
#define OBSTACLES_H

#include "types.h"
#include "track.h"
#include <stdbool.h>

typedef enum {
    OBSTACLE_TYPE_LOW_BARRIER,
    OBSTACLE_TYPE_HIGH_BARRIER,
    OBSTACLE_TYPE_FULL_BARRIER
} ObstacleType;

float ObstacleTypeHeight(ObstacleType type);
float ObstacleTypeYOffset(ObstacleType type);
bool ObstacleTypeAvoidableByJump(ObstacleType type);
bool ObstacleTypeAvoidableBySlide(ObstacleType type);

typedef struct {
    Position3D position;
    ObstacleType obstacle_type;
    Lane lane;
    bool passed;
} Obstacle;

#define MAX_OBSTACLES 50

typedef struct {
    Obstacle obstacles[MAX_OBSTACLES];
    int obstacle_count;
    float last_spawn_z;
    float min_spacing;
} ObstacleManager;

ObstacleManager NewObstacleManager(void);
void ResetObstacleManager(ObstacleManager* manager);
void ObstacleManagerSpawnFromSegments(ObstacleManager* manager, TrackSegment** segments, int segment_count, GameConfig* config);
void UpdateObstacleManager(ObstacleManager* manager, float player_z, GameConfig* config);
Obstacle* CheckObstacleCollision(ObstacleManager* manager, BoundingBox3D player_bbox, Lane player_lane, bool is_jumping, bool is_sliding);
int ObstacleManagerGetVisible(ObstacleManager* manager, float player_z, float view_dist, Obstacle** out_obstacles);

#endif // OBSTACLES_H
