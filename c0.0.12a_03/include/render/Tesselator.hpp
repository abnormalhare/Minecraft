#pragma once

#include <cstring>
#include <GLFW/glfw3.h>
#include "base.hpp"

class Tesselator {
    private:
        static const int MAX_MEMORY_USE = 4194304;
        static const int MAX_FLOATS = 524288;
        float buffer[MAX_FLOATS];
        std::int32_t vertices;
        float u, v;
        float r, g, b;
        bool hasColor = false;
        bool hasTexture = false;
        int len;
        int p;
        
    public:
        static std::shared_ptr<Tesselator> instance;
        Tesselator();
        
        void flush(void);
        void clear(void);
        void init(void);
        void tex(float u, float v);
        void color(int r, int g, int b);
        void color(int rgb);
        void vertexUV(float x, float y, float z, float u, float v);
        void vertex(float x, float y, float z);
};