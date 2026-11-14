#include <raylib.h>
#include <stats.h>
#include <stdbool.h>

Texture *stats_card = 0;
void setup(void) {
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(800, 600, "SnakeGame");
    init_stats();
    stats_card = GetStatsCard(); // create once

    // InitAudioDevice();

    // Load resources
}

void loop(void) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawTexture(*stats_card, 0, 0, WHITE);

    Rectangle btn = (Rectangle){750, 550, 50, 50};
    DrawRectangleRec(btn, BLACK);

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) &&
        CheckCollisionPointRec(GetMousePosition(), btn)) {
        next_day();
        UnloadTexture(*stats_card);
        stats_card = GetStatsCard();
    }

    EndDrawing();
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
