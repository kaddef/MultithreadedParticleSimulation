//
// Created by erkan on 5.04.2025.
//

#include "ThreadedSolver.h"

#include "Benchmark.h"
#include "raymath.h"
#include <cmath>

ThreadedSolver::ThreadedSolver(Particles& particles, ThreadPool& threadPool) :
    particles(particles),
    threadPool(threadPool)
{}

void ThreadedSolver::Update() {
    accumulator += GetFrameTime();
    if (accumulator <= dt) return;
    const float substepDt = dt/static_cast<float>(substeps);
    for (int i = 0; i < substeps; i++) {
        this->HandleInput();
        this->ApplyGravity();
        this->HandleBorder();
        // this->SpatialCollision();
        this->ThreadedCollision();
        this->UpdateParticles(substepDt);
        this->UpdateGrid();
    }
    accumulator -= dt;
}

void ThreadedSolver::UpdateParticles(float dt) {
    for (int id = 0; id < particles.count; id++) {
        particles.Update(id, dt); // Update position

        const Vector2& pos = particles.positions[id];
        particles.gridPos[id] = {std::floor(pos.x/gridSize), std::floor(pos.y/gridSize)};

        Vector2 vel = particles.GetVelocity(id);
        if (vel.x * vel.x + vel.y * vel.y > 2 * gridSize) particles.SetVelocity(id, {0.0f, 0.0f}, 1.0);
    }
}

void ThreadedSolver::ApplyGravity() {
    for (int id = 0; id < particles.count; id++) {
        particles.Accelerate(id, gravity);
    }
}

void ThreadedSolver::HandleInput() {
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) gravity = {0.0f,-1000.0f};
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) gravity = {0.0f,1000.0f};
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) gravity = {-1000.0f,0.0f};
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) gravity = {1000.0f,0.0f};
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) MousePull(GetMousePosition());
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) MousePush(GetMousePosition());
}

void ThreadedSolver::HandleBorder() {
    for (int id = 0; id < particles.count; id++) {
        Vector2& pos = particles.positions[id];
        Vector2 vel = particles.GetVelocity(id);
        float& radius = particles.radius;

        if (pos.x <= radius || pos.x + radius >= GetScreenWidth()) {
            const Vector2 bouncedVel = {-vel.x * dampening, vel.y * dampening};
            if (pos.x <= radius) pos.x = radius;
            else if (pos.x + radius >= GetScreenWidth()) pos.x = GetScreenWidth() - radius;
            particles.SetVelocity(id, bouncedVel, 1.0); // THIS 1.0 DT MAY BE CAUSING WAVE ARTIFACTS
        }

        if (pos.y <= radius || pos.y + radius >= GetScreenHeight()) {
            const Vector2 bouncedVel = {vel.x * dampening, -vel.y * dampening};
            if (pos.y <= radius) pos.y = radius;
            else if (pos.y + radius >= GetScreenHeight()) pos.y = GetScreenHeight() - radius;
            particles.SetVelocity(id, bouncedVel, 1.0); // THIS 1.0 DT MAY BE CAUSING WAVE ARTIFACTS
        }
    }
}

void ThreadedSolver::SpatialCollision() {
    int rowCellCount = GetScreenWidth()/gridSize;
    int colCellCount = GetScreenHeight()/gridSize;
    int dx[] = {1, 1, 0, 0, -1};
    int dy[] = {0, 1, 0, 1, 1};

    for (int i = 0; i < rowCellCount; i++) {
        for (int j = 0; j < colCellCount; j++) {
            const int currentGridIndex = GetGridIndex(i, j);
            if (grids[currentGridIndex].empty()) continue;
            for (int k = 0; k < 5; k++) {
                int collideX = i + dx[k], collideY = j + dy[k];
                if (collideX < 0 || collideY < 0 || collideX >= rowCellCount || collideY >= colCellCount || grids[currentGridIndex].empty()) continue;
                CollideCells(i, j, collideX, collideY);
            }
        }
    }
}

void ThreadedSolver::CollideCells(int x1, int y1, int x2, int y2) {
    const int gridIndex1 = GetGridIndex(x1, y1);
    const int gridIndex2 = GetGridIndex(x2, y2);

    for (int id1 : grids[gridIndex1]) {
        for (int id2 : grids[gridIndex2]) {
            if (id1 == id2) continue;
            Vector2& p1 = particles.positions[id1];
            Vector2& p2 = particles.positions[id2];
            float radius = particles.radius;

            const Vector2 collisionAxis = Vector2Subtract(p1, p2);
            const float distance = Vector2Distance(p1, p2);
            const float centerToCenter = 2 * radius;

            if (distance < centerToCenter) {
                const float overlap = std::abs(centerToCenter - distance);
                const Vector2 normal = Vector2Normalize(collisionAxis);

                const Vector2 particle1Pullback = Vector2Scale(normal, overlap*(radius/(radius + radius))); // This much radius is for if radius of particles are not same
                const Vector2 particle2Pullback = Vector2Scale(normal, -overlap*(radius/(radius + radius)));

                p1.x += particle1Pullback.x;
                p1.y += particle1Pullback.y;

                p2.x += particle2Pullback.x;
                p2.y += particle2Pullback.y;

                // Try
                Vector2 p1Vel = particles.GetVelocity(id1);
                Vector2 p2Vel = particles.GetVelocity(id2);
                if (Vector2Length(p1Vel) > 2 * gridSize) particles.SetVelocity(id1, {0.0f, 0.0f}, 1.0);
                if (Vector2Length(p2Vel) > 2 * gridSize) particles.SetVelocity(id2, {0.0f, 0.0f}, 1.0);
            }
        }
    }
}

void ThreadedSolver::ThreadedCollision() {
    int sliceCount = threadPool.numberOfThreads * 2;
    int sliceSize = gridRowCount / sliceCount;
    //Left pass
    for (int i = 0; i < threadPool.numberOfThreads; i++) {
        threadPool.taskQueue.AddTask([this, i, sliceSize]{
            const int start = 2 * i * sliceSize;
            const int end = start + sliceSize;
            int dx[] = {1, 1, 0, 0, -1};
            int dy[] = {0, 1, 0, 1, 1};
            for (int i = start; i < end; i++) {
                 for (int j = 0; j < gridRowCount; j++) {
                     const int currentGridIndex = GetGridIndex(i, j);
                     if (grids[currentGridIndex].empty()) continue;

                     for (int k = 0; k < 5; k++) {
                         int collideX = i + dx[k], collideY = j + dy[k];
                         if (collideX < 0 || collideY < 0 || collideX >= gridColCount || collideY >= gridRowCount || grids[currentGridIndex].empty()) continue;
                         CollideCells(i, j, collideX, collideY);
                     }
                 }
            }
        });
    }
    threadPool.taskQueue.WaitUntilDone();
    //Right pass
    for (int i = 0; i < threadPool.numberOfThreads; i++) {
        threadPool.taskQueue.AddTask([this, i, sliceSize]{
            const int start = (2 * i + 1) * sliceSize;
            const int end = start + sliceSize;
            int dx[] = {1, 1, 0, 0, -1};
            int dy[] = {0, 1, 0, 1, 1};
            for (int i = start; i < end; i++) {
                 for (int j = 0; j < gridRowCount; j++) {
                     const int currentGridIndex = GetGridIndex(i, j);
                     if (grids[currentGridIndex].empty()) continue;

                     for (int k = 0; k < 5; k++) {
                         int collideX = i + dx[k], collideY = j + dy[k];
                         if (collideX < 0 || collideY < 0 || collideX >= gridColCount || collideY >= gridRowCount || grids[currentGridIndex].empty()) continue;
                         CollideCells(i, j, collideX, collideY);
                     }
                 }
            }
        });
    }
    threadPool.taskQueue.WaitUntilDone();
}

void ThreadedSolver::InitiateParticles(int maxCount) {
    if (spawnComplete) return;
    if (spawnClock < spawnDelay) { spawnClock+=GetFrameTime(); return; }

    for (float i = 0; i < 20; i++) {
        int particleId = particles.InsertParticle({750 - i * 20,10}, RED, gridSize);
        particles.SetVelocity(particleId, {-500,-500}, dt / static_cast<float>(substeps));
    }
    spawnClock = 0;
    if (particles.count >= maxCount) spawnComplete = true;
}

void ThreadedSolver::MousePull(Vector2 position) {
    for (int id = 0; id < particles.count; id++) {
        Vector2 toMouse = Vector2Subtract(position, particles.positions[id]);
        float distance = Vector2Length(toMouse);

        if (distance < influenceRadius) {
            float strength = mouseStrength; // (1.0f + distance * 0.1f);
            Vector2 direction = Vector2Normalize(toMouse);  // Direction TO mouse
            Vector2 force = Vector2Scale(direction, strength);
            particles.Accelerate(id, force);
        }
    }
}

void ThreadedSolver::MousePush(Vector2 position) {
    for (int id = 0; id < particles.count; id++) {
        Vector2 fromMouse = Vector2Subtract(particles.positions[id], position);
        float distance = Vector2Length(fromMouse);

        if (distance < influenceRadius) {
            float strength = mouseStrength; // (1.0f + distance * 0.1f);
            Vector2 direction = Vector2Normalize(fromMouse);  // Direction AWAY from mouse
            Vector2 force = Vector2Scale(direction, strength);

            particles.Accelerate(id, force);
        }
    }
}

void ThreadedSolver::UpdateGrid() {
    #pragma omp parallel for
    for(int i = 0; i < totalGridCells; i++) {
        grids[i].clear();
    }

    #pragma omp parallel for
    for (int id = 0; id < particles.count; id++) {
        const Vector2& pos = particles.positions[id];
        const int gridIndex = GetGridIndex(pos);
        if (gridIndex >= 0 && gridIndex < totalGridCells) {
            #pragma omp critical
            {
                grids[gridIndex].push_back(id);
            }
        }
    }
}

void ThreadedSolver::DrawDebugLines() {
    for (int x = 0; x < GetScreenWidth(); x+=gridSize) {
        for (int y = 0; y < GetScreenHeight(); y+=gridSize) {
            DrawLineV({0.0f,(float)y}, {(float)GetScreenWidth(), (float)y}, WHITE);
        }
        DrawLineV({(float)x,0.0f}, {(float)x, (float)GetScreenHeight()}, WHITE);
    }
}

void ThreadedSolver::DebugUpdate() {
    {
        Benchmark benchmark("Input");
        this->HandleInput();
    }
    {
        Benchmark benchmark("Gravity");
        this->ApplyGravity();
    }
    {
        Benchmark benchmark("Border");
        this->HandleBorder();
    }
    {
        Benchmark benchmark("Collision");
        // this->SpatialCollision();
        this->ThreadedCollision();
    }
    {
        Benchmark benchmark("Update");
        this->UpdateParticles(dt);
    }
    {
        Benchmark benchmark("Grid");
        this->UpdateGrid();
    }
}
