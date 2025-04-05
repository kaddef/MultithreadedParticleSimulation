//
// Created by erkan on 5.04.2025.
//

#include "Particles.h"

#include "raymath.h"
#include <cmath>

Particles::Particles(const int expectedCount) :
    count(0)
{
    positions.reserve(expectedCount);
    lastPositions.reserve(expectedCount);
    accelerations.reserve(expectedCount);
    colors.reserve(expectedCount);
    gridPos.reserve(expectedCount);
}

Particles::~Particles() {
    positions.clear();
    lastPositions.clear();
    accelerations.clear();
    colors.clear();
    gridPos.clear();
}

int Particles::InsertParticle(Vector2 pos, Color color, int gridSize) {
    positions.push_back(pos);
    lastPositions.push_back(pos);
    accelerations.push_back({0.0f, 0.0f});
    colors.push_back(color);
    gridPos.push_back({std::floor(pos.x/gridSize), std::floor(pos.y/gridSize)});
    return count++;
}

void Particles::Update(const int id, const float deltaTime) {
    Vector2& pos = positions[id];
    Vector2& lastPos = lastPositions[id];
    Vector2& acc = accelerations[id];

    Vector2 velocity = Vector2Subtract(pos, lastPos);
    lastPos = pos;
    velocity = Vector2Add(velocity, Vector2Scale(acc, deltaTime * deltaTime));
    pos = Vector2Add(pos, velocity);

    acc = {0,0};
}

void Particles::SetVelocity(const int id, const Vector2 velocity, const float deltaTime) {
    lastPositions[id] = Vector2Subtract(positions[id], Vector2Scale(velocity, deltaTime));
}

Vector2 Particles::GetVelocity(const int id) const {
    return Vector2Subtract(positions[id], lastPositions[id]);
}

void Particles::Accelerate(const int id, const Vector2 acceleration) {
    Vector2& acc = accelerations[id];
    acc = Vector2Add(acc, acceleration); // this returns new struct maybe we should do manual addition ?
}

void Particles::AddVelocity(const int id, const Vector2 velocity, const float deltaTime) {
    Vector2& lastPos = lastPositions[id];
    lastPos = Vector2Subtract(lastPos, Vector2Scale(velocity, deltaTime)); // this returns new struct maybe we should do manual subtraction ?
}

void Particles::Draw(int id) const {
    DrawCircleV(positions[id], radius, colors[id]);
}
