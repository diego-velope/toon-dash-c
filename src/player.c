#include "player.h"

Player NewPlayer(void) {
    Player player;
    ResetPlayer(&player);
    return player;
}

void ResetPlayer(Player* player) {
    player->position = (Position3D){ 0.0f, 0.0f, 0.0f };
    player->lane = LANE_CENTER;
    player->state = PLAYER_STATE_RUNNING;
    player->distance_traveled = 0.0f;
    player->jump_progress = 0.0f;
    player->slide_progress = 0.0f;
    player->target_lane = LANE_CENTER;
    player->lane_change_progress = 1.0f;
    player->animation_tick = 0.0f;
}

bool PlayerJump(Player* player, GameConfig* config) {
    if (player->state == PLAYER_STATE_RUNNING) {
        player->state = PLAYER_STATE_JUMPING;
        player->jump_progress = 0.0f;
        return true;
    } else if (player->state == PLAYER_STATE_SLIDING) {
        player->state = PLAYER_STATE_JUMPING;
        player->jump_progress = 0.0f;
        player->slide_progress = 0.0f;
        return true;
    }
    return false;
}

bool PlayerSlide(Player* player, GameConfig* config) {
    if (player->state == PLAYER_STATE_RUNNING) {
        player->state = PLAYER_STATE_SLIDING;
        player->slide_progress = 0.0f;
        return true;
    } else if (player->state == PLAYER_STATE_JUMPING) {
        player->state = PLAYER_STATE_SLIDING;
        player->slide_progress = 0.0f;
        player->jump_progress = 0.0f;
        player->position.y = 0.0f; // Snap to ground instantly
        return true;
    }
    return false;
}

bool PlayerChangeLane(Player* player, int direction) {
    if (player->lane_change_progress >= 1.0f) {
        Lane new_lane;
        if (LaneNeighbor(player->lane, direction, &new_lane)) {
            player->target_lane = new_lane;
            player->lane_change_progress = 0.0f;
            return true;
        }
    }
    return false;
}

void UpdatePlayer(Player* player, float world_dt, float anim_dt, GameConfig* config) {
    player->distance_traveled += config->player_speed * world_dt;
    player->position.z = player->distance_traveled;
    player->animation_tick += anim_dt;

    if (player->state == PLAYER_STATE_JUMPING) {
        player->jump_progress += anim_dt / config->jump_duration;
        if (player->jump_progress >= 1.0f) {
            player->jump_progress = 0.0f;
            player->state = PLAYER_STATE_RUNNING;
            player->position.y = 0.0f;
        } else {
            float t = player->jump_progress;
            float weighted_t;
            if (t < 0.4f) {
                weighted_t = t / 0.4f * 0.5f;
            } else {
                weighted_t = 0.5f + (t - 0.4f) / 0.6f * 0.5f;
            }
            player->position.y = 4.0f * config->jump_height * weighted_t * (1.0f - weighted_t);
        }
    } else if (player->state == PLAYER_STATE_SLIDING) {
        player->slide_progress += anim_dt / config->slide_duration;
        if (player->slide_progress >= 1.0f) {
            player->slide_progress = 0.0f;
            player->state = PLAYER_STATE_RUNNING;
        }
    }

    if (player->lane_change_progress < 1.0f) {
        player->lane_change_progress += world_dt * 5.0f; 
        if (player->lane_change_progress >= 1.0f) {
            player->lane_change_progress = 1.0f;
            player->lane = player->target_lane;
        }
    }

    float start_x = LaneToX(player->lane, config->lane_width);
    float target_x = LaneToX(player->target_lane, config->lane_width);
    player->position.x = start_x + (target_x - start_x) * player->lane_change_progress;
}

BoundingBox3D PlayerGetBoundingBox(Player* player) {
    float height = (player->state == PLAYER_STATE_SLIDING) ? 0.4f : 1.0f;
    Position3D half_size = { 0.55f, height * 2.0f, 0.35f };
    return BoundingBox3DFromCenter(player->position, half_size);
}

bool PlayerIsAirborne(Player* player) {
    return player->position.y > 0.1f;
}

bool PlayerIsSliding(Player* player) {
    return player->state == PLAYER_STATE_SLIDING;
}

bool PlayerIsAlive(Player* player) {
    return player->state != PLAYER_STATE_DEAD;
}

void PlayerDie(Player* player) {
    player->state = PLAYER_STATE_DEAD;
}
