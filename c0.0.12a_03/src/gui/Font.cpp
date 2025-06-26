#include "gui/Font.hpp"

// stbi stuff defined in Textures.cpp
#include "stb_image.h"

Font::Font(const char* font_file, std::shared_ptr<Textures>& textureManager) : fontTexture(0) {
    this->charWidths = new int[256];

    int w, h, nrChannels;
    std::uint8_t* img = stbi_load(font_file, &w, &h, &nrChannels, 4);

    for (int i = 0; i < 128; i++) {
        h = i % 16;
        int wIn = i / 16;
        int charWidth = 0;

        for (bool var9 = false; charWidth < 8 && !var9; charWidth++) {
            int var10 = (h << 3) + charWidth;
            var9 = true;

            for (int j = 0; j < 8 && var9; j++) {
                int var12 = ((wIn << 3) + j) * w;
                var12 = img[(var10 + var12) * 4 + 2];
                if (var12 > 128) {
                    var9 = false;
                }
            }
        }

        if (i == 32) {
            charWidth = 4;
        }

        this->charWidths[i] = charWidth;
    }

    this->fontTexture = textureManager->loadTexture(font_file, GL_NEAREST);
    
    stbi_image_free(img);
}

void Font::drawShadow(const char* text, int x, int y, int color) {
    this->draw(text, x + 1, y + 1, color, true);
    this->draw(text, x, y, color, false);
}

void Font::draw(const char* text, int x, int y, int color, bool dim) {
    if (dim) {
        color = (color & 0xFCFCFC) >> 2;
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, this->fontTexture);

    std::shared_ptr<Tesselator> t = Tesselator::instance;

    t->init();
    t->color(color);

    int xOff = 0;
    for (size_t i = 0; i < strlen(text); i++) {
        int var9;
        if (text[i] == '&') {
            if (text[i + 1] - '0' < 10) {
                color = text[i + 1] - '0';
            } else if (text[i + 1] - 'a' < 6) {
                color = text[i + 1] - 'a' + 10;
            }

            var9 = (color & 8) << 3;
            int g = (color & 1) * 191 + var9;
            int b = ((color & 2) >> 1) * 191 + var9;
            color = ((color & 4) >> 2) * 191 + var9;
            color = (color << 16) | (g << 8) | (b);
            i += 2;
            if (dim) {
                color = (color & 0xFCFCFC) >> 2;
            }
        }

        // why Notch? why are you reusing variables? it doesnt optimize anything!
        color = text[i] % 16 << 3;
        var9  = text[i] / 16 << 3;
        t->vertexUV(x + xOff,     y + 8, 0.0f, color       / 128.0f, (var9 + 8) / 128.0f);
        t->vertexUV(x + xOff + 8, y + 8, 0.0f, (color + 8) / 128.0f, (var9 + 8) / 128.0f);
        t->vertexUV(x + xOff + 8, y,     0.0f, (color + 8) / 128.0f, var9       / 128.0f);
        t->vertexUV(x + xOff,     y,     0.0f, color       / 128.0f, var9       / 128.0f);

        xOff += this->charWidths[(size_t)text[i]];
    }

    t->flush();
    glDisable(GL_TEXTURE_2D);
}

int Font::getWidth(const char* text) {
    int width = 0;

    for (size_t i = 0; i < strlen(text); i++) {
        if (text[i] == '&') {
            i++; // skip this one and the next one
        } else {
            width += this->charWidths[(size_t)text[i]];
        }
    }

    return width;
}