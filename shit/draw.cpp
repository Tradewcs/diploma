#include "raylib.h"
#include "NFA.cpp"
#include <cmath>

void DrawArrow(Vector2 start, Vector2 end, Color color) {
    DrawLineEx(start, end, 2.0f, color);

    float angle = atan2(end.y - start.y, end.x - start.x);
    float arrowSize = 10.0f;

    Vector2 arrowLeft = {end.x - arrowSize * cos(angle - 0.5f), end.y - arrowSize * sin(angle - 0.5f)};
    Vector2 arrowRight = {end.x - arrowSize * cos(angle + 0.5f), end.y - arrowSize * sin(angle + 0.5f)};
    DrawTriangle(end, arrowLeft, arrowRight, color);
}

void DrawNFA(const NFA &nfa, std::map<int, Vector2> &positions) {
    for (int state : nfa.states) {
        Vector2 pos = positions.at(state);

        if (nfa.acceptStates.count(state)) {
            DrawCircleLines(pos.x, pos.y, 30, BLACK);
            DrawCircleLines(pos.x, pos.y, 25, BLACK);
        } else {
            DrawCircleLines(pos.x, pos.y, 30, BLACK);
        }

        std::string text = "S" + std::to_string(state);
        DrawText(text.c_str(), pos.x - 10, pos.y - 10, 20, BLACK);
    }

    Vector2 startPos = positions.at(nfa.startState);
    DrawArrow({startPos.x - 50, startPos.y}, {startPos.x - 30, startPos.y}, BLACK);

    for (const auto &[key, states] : nfa.transitionTable) {
        int fromState = key.first;
        char symbol = key.second;

        Vector2 fromPos = positions.at(fromState);
        for (int toState : states) {
            Vector2 toPos = positions.at(toState);

            DrawArrow(fromPos, toPos, BLACK);

            Vector2 midPoint = {(fromPos.x + toPos.x) / 2, (fromPos.y + toPos.y) / 2};
            DrawText(std::string(1, symbol).c_str(), midPoint.x, midPoint.y, 20, RED);
        }
    }
}

int main() {
    // Initialize raylib
    InitWindow(800, 600, "NFA Visualization");
    SetTargetFPS(60);

    // Create an example NFA
    NFA nfa (
        {0, 1, 2},                                // States
        {'a', 'b'},
        0,                                        // Start state
        {2}                                       // Accept states
    );
    nfa.addTransition(0, 'a', {1});
    nfa.addTransition(1, 'b', {2});

    // Define positions for states (manually arranged)
    std::map<int, Vector2> positions = {
        {0, {200, 300}},
        {1, {400, 300}},
        {2, {600, 300}}
    };

    // Main loop
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawNFA(nfa, positions);

        EndDrawing();
    }

    // Cleanup
    CloseWindow();
    return 0;
}
