#include "camera.h"

GameCamera NewGameCamera(void) {
    GameCamera gc;
    gc.camera.position = (Vector3){ 0.0f, 6.0f, -8.0f };
    gc.camera.target = (Vector3){ 0.0f, 1.0f, 0.0f };
    gc.camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    gc.camera.fovy = 60.0f;
    gc.camera.projection = CAMERA_PERSPECTIVE;
    gc.current_y_offset = 6.0f;
    return gc;
}

void UpdateGameCamera(GameCamera* gc, Position3D player_pos, float dt) {
    float target_y = player_pos.y + 6.0f;
    gc->current_y_offset += (target_y - gc->current_y_offset) * 10.0f * dt;

    gc->camera.position.x = player_pos.x * 0.3f; 
    gc->camera.position.y = gc->current_y_offset;
    gc->camera.position.z = player_pos.z - 6.0f; 

    gc->camera.target.x = player_pos.x;
    gc->camera.target.y = player_pos.y + 1.5f;
    gc->camera.target.z = player_pos.z + 5.0f;
}
