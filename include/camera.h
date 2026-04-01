#ifndef CAMERA_H
#define CAMERA_H

#include "raylib.h"
#include "types.h"

typedef struct {
    Camera3D camera;
    float current_y_offset;
} GameCamera;

GameCamera NewGameCamera(void);
void UpdateGameCamera(GameCamera* gc, Position3D player_pos, float dt);

#endif
