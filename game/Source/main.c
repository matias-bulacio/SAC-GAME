#include "event.h"
#include <raylib.h>
#include <stats.h>
#include <stdbool.h>
#include <stddef.h>
#include <time.h>

Texture *stats_card = 0;
Texture bg;
void setup(void) {
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(520, 430, "Mission Trainer Simulator");
    init_stats();
    SetRandomSeed(time(NULL));
    stats_card = GetStatsCard(); // create once
    bg = LoadTextureFromImage(LoadImage(
        TextFormat("%sResources/Images/fondo.png", GetApplicationDirectory())));
    // InitAudioDevice();

    // Load resources
}

size_t event_idx = -1;
Texture *event_card = NULL;
bool next_card = true;
void loop(void) {
    ClearBackground(RAYWHITE);
    DrawTexture(bg, 180, 0, WHITE);

    DrawTexture(*stats_card, 0, 0, WHITE);

    if (next_card || event_idx == -1) { // Roll card
        if (event_idx != -1)
            next_day();

        UnloadTexture(*stats_card);
        stats_card = GetStatsCard();
        event_idx = GetRandomValue(0, EVENT_LIST_COUNT - 1);
        if (event_card != NULL)
            UnloadTexture(*event_card);
        event_card = draw_event_card(event_idx);
        next_card = false;
    }
    Rectangle src = {0, 0, (float)event_card->width,
                     -(float)event_card->height};
    Rectangle dst = {200, 20, (float)event_card->width,
                     (float)event_card->height};
    Vector2 origin = {0, 0};

    DrawTexturePro(*event_card, src, dst, origin, 0.0f, WHITE);
    next_card = input_loop_event_card(event_idx, (Vector2){200, 20});
}

void final(void) { CloseWindow(); }

int main() {
    setup();
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        loop();
        EndDrawing();
    }

    final();
}
