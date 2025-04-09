//
// Created by erkan on 6.04.2025.
//

#include "Renderer.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include "raymath.h"

Renderer::Renderer(Particles &particles, bool isDrawing) :
    particles(particles)
{
    Image image = LoadImage("../CircleTexture.png");
    ImageResize(&image, 8,8);
    texture = LoadTextureFromImage(image);
    UnloadImage(image);
    this->isDrawing = isDrawing;
    if (isDrawing) {
        LoadColorsFromFile();
    }
}

void Renderer::Draw() {
    if (isDrawing) {
        for (int index = 0; index < particles.count; index++) {
            // DrawCircleV(particles.positions[id], particles.radius, particles.colors[id]); // This draw circle centered in given position
            Vector2 drawPos = Vector2SubtractValue(particles.positions[index], particles.radius);
            DrawTextureV(texture, drawPos, colorQueue[index]); // White draws original texture
        }
    } else {
        for (int id = 0; id < particles.count; id++) {
            // DrawCircleV(particles.positions[id], particles.radius, particles.colors[id]); // This draw circle centered in given position
            Vector2 drawPos = Vector2SubtractValue(particles.positions[id], particles.radius);
            DrawTextureV(texture, drawPos, WHITE); // White draws original texture
        }
    }
}

void Renderer::GetCurrentState() {
    std::ofstream fout(pCoordFilePath);
    if (!fout.is_open()) {
        std::cout << "Error opening file: " << colorFilePath << std::endl;
        return;
    }

    for (int id = 0; id < particles.count; id++) {
        const Vector2& position = particles.positions[id];
        fout << position.x << " " << position.y << std::endl;
    }
}

void Renderer::LoadColorsFromFile() {
    std::ifstream RgbColorFile(colorFilePath);
    if (!RgbColorFile) {
        std::cout << "Error opening file: " << colorFilePath << std::endl;
        exit(1);
    }

    std::string line;
    int r, g, b;

    while (getline(RgbColorFile, line)) {
        std::stringstream ss(line);
        if (!(ss >> r >> g >> b)) {
            std::cerr << "Invalid color format in line: " << line << std::endl;
            continue;
        }
        colorQueue.emplace_back(r,g,b,255);
    }
}
