#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <functional>
#include "raylib.h"

void StreamLogInput(std::vector<std::string>& logContainer) {
    std::string currentLine;

    while (std::getline(std::cin, currentLine)) {
        if (logContainer.size() > 18) {
            logContainer.erase(logContainer.begin());
        }
        logContainer.push_back(currentLine);
    }
}

int main() {
    const int windowWidth = 600;
    const int windowHeight = 500;
    InitWindow(windowWidth, windowHeight, "Pygame++ Diagnostics & Telemetry");
    SetTargetFPS(60);

    std::vector<std::string> engineLogs;
    engineLogs.push_back("[Pygame++ System] Diagnostic Monitor Initialized.");

    std::thread inputThread(StreamLogInput, std::ref(engineLogs));
    inputThread.detach();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawRectangle(0, 0, windowWidth, 45, DARKGRAY);
        DrawText("Pygame++ ENGINE RUNTIME TELEMETRY", 15, 12, 20, GREEN);
        DrawRectangle(0, 45, windowWidth, 2, LIME);

        for (size_t i = 0; i < engineLogs.size(); i++) {
            DrawText(engineLogs[i].c_str(), 15, 65 + (i * 22), 16, RAYWHITE);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}