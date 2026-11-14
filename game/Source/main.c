#include <raylib.h>
#include <stdbool.h>

void setup(void) {
    InitWindow(800, 600, "SnakeGame");

    // InitAudioDevice();

    // Load resources
}

void loop(void) {
    ClearBackground(RAYWHITE);
    DrawRectangle(200, 100, 400, 400, RED);
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
