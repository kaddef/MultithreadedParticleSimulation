#pragma once
#include <vector>

#include "raylib.h"

struct Particles {
    int count;
    float radius = 10.0f;
    std::vector<Vector2> positions;
    std::vector<Vector2> lastPositions;
    std::vector<Vector2> accelerations;
    std::vector<Color> colors;
    std::vector<Vector2> gridPos;

    Particles(int expectedCount);
    ~Particles();
    Vector2 GetVelocity(int id) const;
    int InsertParticle(Vector2 pos, Color color, int gridSize);
    void SetVelocity(int id, Vector2 velocity, float deltaTime);
    void Update(int id, float deltaTime);
    void Accelerate(int id, Vector2 acceleration);
    void AddVelocity(int id, Vector2 velocity, float deltaTime);
    void Draw(int id) const;// This is a non-performant way to render. Consider implementing texture-based rendering or, even better, vertex-based rendering for better performance.
};
