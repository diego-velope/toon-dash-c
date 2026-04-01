#include "types.h"

float LaneToX(Lane lane, float lane_width) {
    if (lane == LANE_LEFT) return lane_width;
    if (lane == LANE_CENTER) return 0.0f;
    if (lane == LANE_RIGHT) return -lane_width;
    return 0.0f;
}

bool LaneNeighbor(Lane current, int direction, Lane* out_lane) {
    int new_index = (int)current + direction;
    if (new_index == -1) {
        *out_lane = LANE_LEFT;
        return true;
    } else if (new_index == 0) {
        *out_lane = LANE_CENTER;
        return true;
    } else if (new_index == 1) {
        *out_lane = LANE_RIGHT;
        return true;
    }
    return false;
}

Vector3 Position3DToVector3(Position3D pos) {
    return (Vector3){ pos.x, pos.y, pos.z };
}

BoundingBox3D BoundingBox3DFromCenter(Position3D center, Position3D half_size) {
    BoundingBox3D box;
    box.min.x = center.x - half_size.x;
    box.min.y = center.y - half_size.y;
    box.min.z = center.z - half_size.z;
    box.max.x = center.x + half_size.x;
    box.max.y = center.y + half_size.y;
    box.max.z = center.z + half_size.z;
    return box;
}

bool BoundingBox3DIntersects(BoundingBox3D a, BoundingBox3D b) {
    return (a.min.x <= b.max.x && a.max.x >= b.min.x &&
            a.min.y <= b.max.y && a.max.y >= b.min.y &&
            a.min.z <= b.max.z && a.max.z >= b.min.z);
}

GameConfig DefaultGameConfig(void) {
    GameConfig config;
    config.lane_width = 2.35f;
    config.player_speed = 15.0f;
    config.jump_height = 2.5f;
    config.jump_duration = 0.6f;
    config.slide_duration = 0.5f;
    config.spawn_distance = 100.0f;
    config.despawn_distance = 30.0f;
    config.coin_value = 10;
    return config;
}
