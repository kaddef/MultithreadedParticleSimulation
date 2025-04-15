//
// Created by erkan on 5.04.2025.
//

#ifndef THREADEDSOLVER_H
#define THREADEDSOLVER_H

#include "Particles.h"
#include "ThreadPool.h"


class ThreadedSolver {
public:
    Particles& particles;
    ThreadPool& threadPool;
    Vector2 gravity = {0.0f, 1000.f};
    float dt = 1.0f / 60.0f;
    int substeps = 8;
    float dampening = 0.99f;
    float accumulator = 0;
    const float influenceRadius = 100.0f;
    const float mouseStrength = 8000.0f;
    static constexpr int gridSize = 8.0f;
    static constexpr int gridRowCount = 1024 / gridSize;
    static constexpr int gridColCount = 1024 / gridSize;
    static constexpr int totalGridCells = gridRowCount * gridColCount;
    std::vector<int> grids [totalGridCells];


    bool spawnComplete = false;
    double spawnClock = 0;
    double spawnDelay = 0.05f;

    ThreadedSolver(Particles& particles, ThreadPool& threadPool);

    void Update();

    void UpdateParticles(float dt);
    void ApplyGravity();
    void HandleInput();
    void HandleBorder();
    void SpatialCollision();

    //Threaded versions of the functions above
    void ThreadedCollision();

    void CollideCells(int x1, int y1, int x2, int y2);

    void UpdateGrid();

    void InitiateParticles(int maxCount);

    void MousePush(Vector2 mousePos);
    void MousePull(Vector2 mousePos);

    inline int GetGridIndex(int x, int y) { 
        return y * gridRowCount + x; 
    }
    inline int GetGridIndex(const Vector2& pos) { 
        return static_cast<int>(pos.y/gridSize) * gridRowCount + static_cast<int>(pos.x/gridSize); 
    }

    void DrawDebugLines();
    void DebugUpdate();
};



#endif //THREADEDSOLVER_H
