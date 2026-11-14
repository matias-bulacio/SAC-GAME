#include "raylib.h"
#include <stats.h>

// Time stats
int16_t current_day = 216;

struct player_stats player_stats;
struct station_stats station_stats;
struct production_stats production_stats;
struct consumed_stats consumed_stats;

Texture stats_card_texture;

void init_stats() {
    player_stats = (struct player_stats){
        .health = 100,
        .sleep = 100,
    };

    station_stats = (struct station_stats){
        .food_stored = 10,
        .power_stored = 40,
        .radiation_shield = 20,
    };

    production_stats = (struct production_stats){
        .food_prod = 0,
        .power_prod = 3,
    };

    consumed_stats = (struct consumed_stats){
        .food_consumed_per_day = 2,
        .power_consumed_per_day = 7,
        .radiation_shielded_per_day = 1,
    };
}

Texture *GetStatsCard() {
    Image i = GenImageColor(180, 600, GetColor(0x333333ff));

    const int font_size = 24;
    const int line_gap = 4;
    const int section_gap = 36;

    int y = 10;

    const char *human_text =
        TextFormat("Human\n"
                   "  Health: %d\n"
                   "  Sleep: %d\n",
                   player_stats.health, player_stats.sleep);

    ImageDrawText(&i, human_text, 10, y, font_size, WHITE);
    y += (font_size + line_gap) * 3 + section_gap;

    const char *station_text =
        TextFormat("Station\n"
                   "  Food: %d\n"
                   "  Power: %d\n"
                   "  Shield: %d\n",
                   station_stats.food_stored, station_stats.power_stored,
                   station_stats.radiation_shield);

    ImageDrawText(&i, station_text, 10, y, font_size, WHITE);
    y += (font_size + line_gap) * 4 + section_gap;

    const char *production_text =
        TextFormat("Production\n"
                   "  Food: %d\n"
                   "  Power: %d\n",
                   production_stats.food_prod, production_stats.power_prod);

    ImageDrawText(&i, production_text, 10, y, font_size, WHITE);

    stats_card_texture = LoadTextureFromImage(i);

    UnloadImage(i);
    return &stats_card_texture;
}

void next_day() {
    current_day++;

    station_stats.food_stored -= consumed_stats.food_consumed_per_day;
    station_stats.power_stored -= consumed_stats.power_consumed_per_day;

    station_stats.food_stored += production_stats.food_prod;
    station_stats.power_stored += production_stats.power_prod;

    if (station_stats.food_stored < 0)
        station_stats.food_stored = 0;
    if (station_stats.power_stored < 0)
        station_stats.power_stored = 0;

    station_stats.radiation_shield -= consumed_stats.radiation_shielded_per_day;

    if (station_stats.power_stored <= 0) // Extra waste
        station_stats.radiation_shield -=
            consumed_stats.radiation_shielded_per_day;

    if (station_stats.radiation_shield < 0)
        station_stats.radiation_shield = 0;
}
