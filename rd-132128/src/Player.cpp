#include "Player.hpp"

bool Player::isKeyDown(int key) {
    int state = glfwGetKey(this->getWindow(), key);
    return state == GLFW_PRESS;
}

Player::Player(std::shared_ptr<Level>& level, GLFWwindow* window) : Entity(level, window) {
    this->heightOffset = 1.62f;
}

void Player::tick(void) {
    this->xo = this->x;
    this->yo = this->y;
    this->zo = this->z;

    float xa = 0.0f;
    float ya = 0.0f;

    if (this->isKeyDown(GLFW_KEY_R)) this->resetPos();

    if (this->isKeyDown(GLFW_KEY_UP) || this->isKeyDown(GLFW_KEY_W)) {
        ya--;
    }
    if (this->isKeyDown(GLFW_KEY_DOWN) || this->isKeyDown(GLFW_KEY_S)) {
        ya++;
    }
    if (this->isKeyDown(GLFW_KEY_LEFT) || this->isKeyDown(GLFW_KEY_A)) {
        xa--;
    }
    if (this->isKeyDown(GLFW_KEY_RIGHT) || this->isKeyDown(GLFW_KEY_D)) {
        xa++;
    }
    if (this->isKeyDown(GLFW_KEY_SPACE) || this->isKeyDown(GLFW_KEY_MENU)) {
        if (this->onGround) this->yd = 0.12f;
    }

    this->moveRelative(xa, ya, this->onGround ? 0.02f : 0.005f);
    this->yd -= 0.005;
    this->move(this->xd, this->yd, this->zd);
    this->xd *= 0.91f;
    this->yd *= 0.98f;
    this->zd *= 0.91f;

    if (this->onGround) {
        this->xd *= 0.8f;
        this->zd *= 0.8f;
    }
}