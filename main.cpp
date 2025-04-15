#include "Particles.h"
#include "raylib.h"
#include "Renderer.h"
#include "ThreadedSolver.h"

int main()
{
    constexpr int screenWidth = 1024;
    constexpr int screenHeight = 1024;
    constexpr int particleCount = 10000;
    constexpr int threadCount = 16;

    InitWindow(screenWidth, screenHeight, "Particle Simulation");
    SetTargetFPS(60);

    Particles particles(particleCount);
    Renderer renderer(particles, false);
    ThreadPool threadPool(threadCount);
    ThreadedSolver solver(particles, threadPool);

    while (!WindowShouldClose())
    {
        solver.InitiateParticles(particleCount);

        if (IsKeyDown(KEY_SPACE)) renderer.GetCurrentState();
        solver.Update();

        BeginDrawing();
        ClearBackground(BLACK);
        renderer.Draw();
        // solver.DrawDebugLines();
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
