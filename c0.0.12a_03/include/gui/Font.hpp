#pragma once

#include "base.hpp"
#include "render/Textures.hpp"
#include "render/Tesselator.hpp"

class Font {
private:
    int* charWidths;
    int fontTexture;

    void draw(const char* text, int x, int y, int color, bool dim);

public:
    Font(const char* text, std::shared_ptr<Textures>& textureManager);
    void drawShadow(const char* text, int x, int y, int color);
    int getWidth(const char* text);
};