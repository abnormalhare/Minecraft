#include <GLFW/glfw3.h>
#include "base.hpp"

class Tesselator {
    private:
        static const int MAX_VERTICES = 100000;
        float vertexBuffer[MAX_VERTICES * 3];
        float texCoordBuffer[MAX_VERTICES * 2];
        float colorBuffer[MAX_VERTICES * 3];
        std::int32_t vertices = 0;
        float u, v;
        float r, g, b;
        bool hasColor = false;
        bool hasTexture = false;
        
    public:
        void flush(void);
        void clear(void);
        void init(void);
        void tex(float u, float v);
        void color(float r, float g, float b);
        void vertex(float x, float y, float z);
};