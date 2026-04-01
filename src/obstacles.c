#include "obstacles.h"
#include <stdlib.h>

float ObstacleTypeHeight(ObstacleType type) {
    switch (type) {
        case OBSTACLE_TYPE_LOW_BARRIER: return 1.0f;
        case OBSTACLE_TYPE_HIGH_BARRIER: return 2.0f;
        case OBSTACLE_TYPE_FULL_BARRIER: return 3.0f;
    }
    return 1.0f;
}

float ObstacleTypeYOffset(ObstacleType type) {
    if (type == OBSTACLE_TYPE_HIGH_BARRIER) return 1.5f;
    return 0.0f;
}

bool ObstacleTypeAvoidableByJump(ObstacleType type) {
    return type == OBSTACLE_TYPE_LOW_BARRIER;
}

bool ObstacleTypeAvoidableBySlide(ObstacleType type) {
    return type == OBSTACLE_TYPE_HIGH_BARRIER;
}

ObstacleManager NewObstacleManager(void) {
    ObstacleManager manager;
    ResetObstacleManager(&manager);
    return manager;
}

void ResetObstacleManager(ObstacleManager* manager) {
    manager->obstacle_count = 0;
    manager->last_spawn_z = 0.0f;
    manager->min_spacing = 15.0f;
}

void ObstacleManagerSpawnFromSegments(ObstacleManager* manager, TrackSegment** segments, int segment_count, GameConfig* config) {
    for (int i = 0; i < segment_count; i++) {
        if (segments[i]->z_position > manager->last_spawn_z + manager->min_spacing) {
            
            if (manager->obstacle_count >= MAX_OBSTACLES - 3) break;

            float z = segments[i]->z_position;
            
            // Random pattern
            int r = rand() % 100;
            int type_r = rand() % 100;
            ObstacleType otype = OBSTACLE_TYPE_LOW_BARRIER;
            if (type_r < 30) otype = OBSTACLE_TYPE_HIGH_BARRIER;
            else if (type_r < 50) otype = OBSTACLE_TYPE_FULL_BARRIER;

            Lane lane1 = (Lane)((rand() % 3) - 1);
            Lane lane2 = (Lane)((rand() % 3) - 1);
            
            if (r < 70) {
                // One obstacle
                manager->obstacles[manager->obstacle_count].position = (Position3D){ LaneToX(lane1, config->lane_width), ObstacleTypeYOffset(otype), z };
                manager->obstacles[manager->obstacle_count].lane = lane1;
                manager->obstacles[manager->obstacle_count].obstacle_type = otype;
                manager->obstacles[manager->obstacle_count].passed = false;
                manager->obstacle_count++;

                if (r < 40 && lane1 != lane2) {
                    // Two obstacles
                    manager->obstacles[manager->obstacle_count].position = (Position3D){ LaneToX(lane2, config->lane_width), ObstacleTypeYOffset(otype), z };
                    manager->obstacles[manager->obstacle_count].lane = lane2;
                    manager->obstacles[manager->obstacle_count].obstacle_type = otype;
                    manager->obstacles[manager->obstacle_count].passed = false;
                    manager->obstacle_count++;
                }
            }

            manager->last_spawn_z = z;
        }
    }
}

void UpdateObstacleManager(ObstacleManager* manager, float player_z, GameConfig* config) {
    int shift = 0;
    for (int i = 0; i < manager->obstacle_count; i++) {
        if (manager->obstacles[i].position.z < player_z - config->despawn_distance) {
            shift++;
        }
    }

    if (shift > 0) {
        for (int i = 0; i < manager->obstacle_count - shift; i++) {
            manager->obstacles[i] = manager->obstacles[i + shift];
        }
        manager->obstacle_count -= shift;
    }
}

Obstacle* CheckObstacleCollision(ObstacleManager* manager, BoundingBox3D player_bbox, Lane player_lane, bool is_jumping, bool is_sliding) {
    // Forgiveness hitbox sizing - exact matching Rust logic
    float forgiveness = 0.5f;

    for (int i = 0; i < manager->obstacle_count; i++) {
        Obstacle* obs = &manager->obstacles[i];
        
        Position3D half = {0.8f, 1.0f, 0.4f};
        if (obs->obstacle_type == OBSTACLE_TYPE_HIGH_BARRIER) half.y = 0.5f;
        
        BoundingBox3D obs_box = BoundingBox3DFromCenter(
            (Position3D){obs->position.x, obs->position.y + half.y, obs->position.z},
            half
        );

        // Adjust for forgiveness
        player_bbox.min.z += forgiveness;
        player_bbox.max.z -= forgiveness;
        player_bbox.min.x += forgiveness * 0.5f;
        player_bbox.max.x -= forgiveness * 0.5f;

        if (BoundingBox3DIntersects(player_bbox, obs_box)) {
            if (obs->obstacle_type == OBSTACLE_TYPE_LOW_BARRIER && is_jumping) {
                // Avoided
            } else if (obs->obstacle_type == OBSTACLE_TYPE_HIGH_BARRIER && is_sliding) {
                // Avoided
            } else {
                return obs;
            }
        }
    }
    return NULL;
}

int ObstacleManagerGetVisible(ObstacleManager* manager, float player_z, float view_dist, Obstacle** out_obstacles) {
    int count = 0;
    for (int i = 0; i < manager->obstacle_count; i++) {
        float z = manager->obstacles[i].position.z;
        if (z > player_z - 5.0f && z < player_z + view_dist) {
            out_obstacles[count++] = &manager->obstacles[i];
        }
    }
    return count;
}
