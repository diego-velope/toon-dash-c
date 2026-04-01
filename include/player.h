#ifndef PLAYER_H
#define PLAYER_H

#include "types.h"
#include <stdbool.h>

typedef enum {
    PLAYER_STATE_RUNNING,
    PLAYER_STATE_JUMPING,
    PLAYER_STATE_SLIDING,
    PLAYER_STATE_DEAD
} PlayerState;

typedef struct {
    Position3D position;
    Lane lane;
    PlayerState state;
    float distance_traveled;
    float jump_progress;
    float slide_progress;
    Lane target_lane;
    float lane_change_progress;
    float animation_tick;
} Player;

Player NewPlayer(void);
void ResetPlayer(Player* player);
bool PlayerJump(Player* player, GameConfig* config);
bool PlayerSlide(Player* player, GameConfig* config);
bool PlayerChangeLane(Player* player, int direction);
void UpdatePlayer(Player* player, float world_dt, float anim_dt, GameConfig* config);
BoundingBox3D PlayerGetBoundingBox(Player* player);
bool PlayerIsAirborne(Player* player);
bool PlayerIsSliding(Player* player);
bool PlayerIsAlive(Player* player);
void PlayerDie(Player* player);

#endif // PLAYER_H
