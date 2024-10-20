#include "Player.hpp"

Player::Player(Level* level, GLFWwindow* window) : Entity(level, window) {
    this->heightOffset = 1.62f;
}

void Player::tick(void) {
    this->xo = this->x;
    this->yo = this->y;
    this->zo = this->z;

    float xa = 0.0f;
    float ya = 0.0f;

    if (isKeyDown(GLFW_KEY_R)) {
        resetPos();
    }
    if (isKeyDown(GLFW_KEY_UP) || isKeyDown(GLFW_KEY_W)) {
        ya--;
    }
    if (isKeyDown(GLFW_KEY_DOWN) || isKeyDown(GLFW_KEY_S)) {
        ya++;
    }
    if (isKeyDown(GLFW_KEY_LEFT) || isKeyDown(GLFW_KEY_A)) {
        xa--;
    }
    if (isKeyDown(GLFW_KEY_RIGHT) || isKeyDown(GLFW_KEY_D)) {
        xa++;
    }
    if (isKeyDown(GLFW_KEY_SPACE) || isKeyDown(GLFW_KEY_MENU)) {
        if (this->onGround) {
            this->yd = 0.12f;
        }
    }

    moveRelative(xa, ya, this->onGround ? 0.02f : 0.005f);
    this->yd -= 0.005;

    move(this->xd, this->yd, this->zd);
    this->xd *= 0.91f;
    this->yd *= 0.98f;
    this->zd *= 0.91f;
    if (this->onGround) {
        this->xd *= 0.8f;
        this->zd *= 0.8f;
    }
}