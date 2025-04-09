#include "Particles.h"
#include "raylib.h"
#include "Renderer.h"
#include "ThreadedSolver.h"

int main()
{
    constexpr int screenWidth = 1024;
    constexpr int screenHeight = 1024;

    InitWindow(screenWidth, screenHeight, "Particle Simulation");
    SetTargetFPS(60);

    Particles particles(4580);
    Renderer renderer(particles, true);
    ThreadedSolver solver(particles);

    while (!WindowShouldClose())
    {
        solver.InitiateParticles(4580);

        if (IsKeyDown(KEY_SPACE)) renderer.GetCurrentState();
        solver.Update();

        BeginDrawing();
        ClearBackground(BLACK);
        // solver.DrawDebugLines();
        renderer.Draw();
        DrawFPS(10,10);
        DrawText(TextFormat("%02.04f ms", GetFrameTime() * 1000), 100, 10, 20, LIME);
        DrawText(TextFormat("P.Count: %i", particles.count), 10, 30, 20, LIME);
        EndDrawing();
    }

    solver.DebugUpdate();
    // De-Initialization
    CloseWindow();
    return 0;
}
