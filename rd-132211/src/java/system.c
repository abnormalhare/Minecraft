#include "java/system.h"

GLFWwindow* window;
Mouse* mouse;

bool setDisplayMode(s32 width, s32 height) {
    if (!glfwInit()) {
        printf("Failed to Init GLFW");
        return 1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    window = glfwCreateWindow(width, height, "Game", NULL, NULL);
    if (!window) {
        printf("Failed to create GLFW Window");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    return 0;
}

void displayUpdate(void) {
    glfwSwapBuffers(window);
}

void displayDestroy(void) {
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool isCloseRequested(void) {
    return glfwWindowShouldClose(window);
}

void setMouseGrabbed(bool isGrabbed) {
    if (isGrabbed) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void _mousePosCallback(GLFWwindow* window, double xpos, double ypos) {
    mouse->prevX = mouse->currX;
    mouse->prevY = mouse->currY;

    mouse->currX = xpos;
    mouse->currY = ypos;
}

void _mouseButtonCallback(GLFWwindow* window, s32 button, s32 action, s32 mods) {
    mouse->button = action == GLFW_PRESS ? button : -1;
}

void mouseCreate(void) {
    mouse = malloc(sizeof(Mouse));
    if (!mouse) {
        fprintf(stderr, "Failed to allocate to mouse");
        return;
    }

    mouse->currX = 0;
    mouse->currY = 0;
    mouse->prevX = 0;
    mouse->prevY = 0;
    glfwSetCursorPosCallback(window, _mousePosCallback);

    mouse->button = -1;
    glfwSetMouseButtonCallback(window, _mouseButtonCallback);
}

float mouseGetDX(void) {
    return mouse->currX - mouse->prevX;
}

float mouseGetDY(void) {
    return mouse->currY - mouse->prevY;
}

void mouseDestroy(void) {
    free(mouse);
}