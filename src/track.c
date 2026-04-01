#include "track.h"
#include <stdlib.h>

Track NewTrack(void) {
    Track track;
    ResetTrack(&track);
    return track;
}

void ResetTrack(Track* track) {
    track->segment_count = 0;
    track->segment_length = 20.0f;
    track->last_z = -10.0f;
    track->difficulty = 0.0f;
    track->track_width = 8.0f;

    for (int i = 0; i < 5; i++) {
        track->segments[track->segment_count].z_position = track->last_z;
        track->segments[track->segment_count].segment_type = SEGMENT_GROUND;
        track->segment_count++;
        track->last_z += track->segment_length;
    }
}

void UpdateTrack(Track* track, float player_z, GameConfig* config) {
    track->difficulty = (player_z > 0.0f ? player_z / 3000.0f : 0.0f);
    if (track->difficulty > 1.0f) track->difficulty = 1.0f;

    float spawn_boundary = player_z + config->spawn_distance;

    while (track->last_z < spawn_boundary && track->segment_count < MAX_TRACK_SEGMENTS) {
        SegmentType type = SEGMENT_GROUND;
        
        int r = rand() % 100;
        float obstacle_chance = 35.0f + track->difficulty * 35.0f; 
        
        if (r < (int)obstacle_chance) {
            type = SEGMENT_OBSTACLE_ZONE;
        } else if (r < (int)obstacle_chance + 15) {
            type = SEGMENT_COIN_ZONE;
        }

        if (type == SEGMENT_OBSTACLE_ZONE && track->segment_count > 1) {
            if (track->segments[track->segment_count - 1].segment_type == SEGMENT_OBSTACLE_ZONE &&
                track->segments[track->segment_count - 2].segment_type == SEGMENT_OBSTACLE_ZONE) {
                type = SEGMENT_GROUND;
            }
        }

        track->segments[track->segment_count].z_position = track->last_z;
        track->segments[track->segment_count].segment_type = type;
        track->segment_count++;
        track->last_z += track->segment_length;
    }

    int shift = 0;
    for (int i = 0; i < track->segment_count; i++) {
        if (track->segments[i].z_position < player_z - config->despawn_distance) {
            shift++;
        }
    }

    if (shift > 0) {
        for (int i = 0; i < track->segment_count - shift; i++) {
            track->segments[i] = track->segments[i + shift];
        }
        track->segment_count -= shift;
    }
}

int TrackGetObstacleZones(Track* track, float player_z, float view_dist, TrackSegment** out_segments) {
    int count = 0;
    for (int i = 0; i < track->segment_count; i++) {
        if (track->segments[i].segment_type == SEGMENT_OBSTACLE_ZONE) {
            float z = track->segments[i].z_position;
            if (z > player_z - 10.0f && z < player_z + view_dist) {
                out_segments[count++] = &track->segments[i];
            }
        }
    }
    return count;
}

int TrackGetCoinZones(Track* track, float player_z, float view_dist, TrackSegment** out_segments) {
    int count = 0;
    for (int i = 0; i < track->segment_count; i++) {
        if (track->segments[i].segment_type == SEGMENT_COIN_ZONE) {
            float z = track->segments[i].z_position;
            if (z > player_z - 10.0f && z < player_z + view_dist) {
                out_segments[count++] = &track->segments[i];
            }
        }
    }
    return count;
}

int TrackGetVisible(Track* track, float player_z, float view_dist, TrackSegment** out_segments) {
    int count = 0;
    for (int i = 0; i < track->segment_count; i++) {
        float z = track->segments[i].z_position;
        if (z > player_z - 10.0f && z < player_z + view_dist) {
            out_segments[count++] = &track->segments[i];
        }
    }
    return count;
}

float TrackDifficulty(Track* track) {
    return track->difficulty;
}
