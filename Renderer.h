//
// Created by erkan on 6.04.2025.
//

#ifndef RENDERER_H
#define RENDERER_H
#include <string>

#include "Particles.h"


class Renderer {
public:
    Particles& particles;
    Texture2D texture;
    Renderer(Particles& particles, bool isDrawing);

    //Drawing image variables
    bool isDrawing;
    std::string pCoordFilePath = "../output.txt";
    std::string colorFilePath = "../colors.txt";
    // bool colorsLoaded = false;
    int colorQueueIndex = 0;
    std::vector<Color> colorQueue;

    void Draw();
    void GetCurrentState();
    void LoadColorsFromFile();
};



#endif //RENDERER_H
