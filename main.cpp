#include "Particles.h"
#include "raylib.h"
#include "ThreadedSolver.h"

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 800;

    Particles particles(1);
    ThreadedSolver solver(particles);

    InitWindow(screenWidth, screenHeight, "Particle Simulation");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        solver.InitiateParticles(1000);
        solver.Update();

        BeginDrawing();
        ClearBackground(BLACK);
        for (int id = 0; id < particles.count; id++) particles.Draw(id);
        solver.DrawDebugLines();
        DrawFPS(10,10);
        DrawText(TextFormat("%02.04f ms", GetFrameTime() * 1000), 100, 10, 20, LIME);
        DrawText(TextFormat("P.Count: %i", particles.count), 10, 30, 20, LIME);
        EndDrawing();
    }

    // De-Initialization
    CloseWindow();
    return 0;
}
