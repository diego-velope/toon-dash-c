#ifndef TV_INPUT_H
#define TV_INPUT_H

#include <stdbool.h>

typedef enum {
  TV_ACTION_UP,
  TV_ACTION_DOWN,
  TV_ACTION_LEFT,
  TV_ACTION_RIGHT,
  TV_ACTION_ACTION,
  TV_ACTION_BACK,
  TV_ACTION_COUNT
} TvAction;

typedef struct {
  bool current_state[TV_ACTION_COUNT];
  bool previous_state[TV_ACTION_COUNT];
} TvInputManager;

void InitTvInputManager(void);
TvInputManager *GetTvInputManager(void);
void UpdateTvInputManager(void);
bool TvInputIsActionPressed(TvAction action);
bool TvInputIsActionJustPressed(TvAction action);
void TvInputClear(void);

// For fallback to keyboard
typedef struct {
  bool up;
  bool down;
  bool left;
  bool right;
  bool action;
  bool back;

  bool prev_up;
  bool prev_down;
  bool prev_left;
  bool prev_right;
  bool prev_action;
  bool prev_back;
} TvInput;

TvInput NewTvInput(void);
void UpdateTvInput(TvInput *input);
bool TvInputIsUpJustPressed(TvInput *input);
bool TvInputIsDownJustPressed(TvInput *input);
bool TvInputIsLeftJustPressed(TvInput *input);
bool TvInputIsRightJustPressed(TvInput *input);
bool TvInputIsBackJustPressed(TvInput *input);

// Exposed functions for tv-pal.js
void mq_handle_up(int pressed);
void mq_handle_down(int pressed);
void mq_handle_left(int pressed);
void mq_handle_right(int pressed);
void mq_handle_action(int pressed);
void mq_handle_back(int pressed);

#endif // TV_INPUT_H
