#pragma once
#include <raylib.h>
#include <stdint.h>

// Time stats
extern int16_t current_day;

// Human stats
struct player_stats {
    int8_t health;
    int8_t sleep;
};

extern struct player_stats player_stats;

// Station stats
struct station_stats {
    int8_t radiation_shield;
    int8_t power_stored;
    int8_t food_stored;
};

extern struct station_stats station_stats;

// Augmenting stats
struct production_stats {
    int8_t power_prod;
    int8_t food_prod;
};

extern struct production_stats production_stats;

// Degradement stats
struct consumed_stats {
    int8_t radiation_shielded_per_day;
    int8_t power_consumed_per_day;
    int8_t food_consumed_per_day;
};

extern struct consumed_stats consumed_stats;

void init_stats();
Texture *GetStatsCard();
void next_day();
