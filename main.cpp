#include "Particles.h"
#include "raylib.h"
#include "Renderer.h"
#include "ThreadedSolver.h"

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Particle Simulation");
    SetTargetFPS(60);

    Particles particles(5000);
    Renderer renderer(particles, false);
    ThreadedSolver solver(particles);

    while (!WindowShouldClose())
    {
        solver.InitiateParticles(5000);

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

    // De-Initialization
    CloseWindow();
    return 0;
}
