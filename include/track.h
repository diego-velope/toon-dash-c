#ifndef TRACK_H
#define TRACK_H

#include "types.h"
#include <stdbool.h>

typedef enum {
    SEGMENT_GROUND,
    SEGMENT_OBSTACLE_ZONE,
    SEGMENT_COIN_ZONE
} SegmentType;

typedef struct {
    float z_position;
    SegmentType segment_type;
} TrackSegment;

#define MAX_TRACK_SEGMENTS 100

typedef struct {
    TrackSegment segments[MAX_TRACK_SEGMENTS];
    int segment_count;
    float segment_length;
    float last_z;
    float difficulty;
    float track_width;
} Track;

Track NewTrack(void);
void ResetTrack(Track* track);
void UpdateTrack(Track* track, float player_z, GameConfig* config);
int TrackGetObstacleZones(Track* track, float player_z, float view_dist, TrackSegment** out_segments);
int TrackGetCoinZones(Track* track, float player_z, float view_dist, TrackSegment** out_segments);
int TrackGetVisible(Track* track, float player_z, float view_dist, TrackSegment** out_segments);
float TrackDifficulty(Track* track);

#endif // TRACK_H
