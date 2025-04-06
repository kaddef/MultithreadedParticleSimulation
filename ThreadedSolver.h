//
// Created by erkan on 5.04.2025.
//

#ifndef THREADEDSOLVER_H
#define THREADEDSOLVER_H
#include <string>

#include "Particles.h"


class ThreadedSolver {
public:
    Particles& particles;
    Vector2 gravity = {0.0f, 1000.f};
    float dt = 1.0f / 60.0f; // Its fixed to 60 fps use GetFrameTime()
    int substeps = 8;
    float dampening = 0.9f;
    const float influenceRadius = 100.0f;
    const float mouseStrength = 8000.0f;
    static constexpr int gridSize = 20.f;
    std::vector<int> grids [800/gridSize][800/gridSize];


    bool spawnComplete = false;
    double spawnClock = 0;
    double spawnDelay = 0.05f;

    ThreadedSolver(Particles& particles);

    void Update();

    void UpdateParticles(float dt);
    void ApplyGravity();
    void HandleInput();
    void HandleBorder();
    void SpatialCollision();

    void CollideCells(int x1, int y1, int x2, int y2);

    void UpdateGrid();

    void InitiateParticles(int maxCount);

    void MousePush(Vector2 mousePos);
    void MousePull(Vector2 mousePos);

    void DrawDebugLines();
};



#endif //THREADEDSOLVER_H
