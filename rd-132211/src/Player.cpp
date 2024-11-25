#include "Player.hpp"

bool Player::isKeyDown(int key) {
    int state = glfwGetKey(this->window, key);
    return state == GLFW_PRESS;
}

Player::Player(std::shared_ptr<Level>& level, GLFWwindow* window) {
    this->level = level;
    this->window = window;
    resetPos();
}

#define randf static_cast<float>(rand()) / static_cast<float>(RAND_MAX)

void Player::resetPos(void) {
    float x = randf * this->level->width;
    float y = this->level->depth + 10;
    float z = randf * this->level->height;

    setPos(x, y, z);
}

void Player::setPos(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;

    float w = 0.3f;
    float h = 0.9f;

    this->bb = AABB(x - w, y - h, z - w, x + w, y + h, z + w);
}

void Player::turn(float xo, float yo) {
    this->yRot += xo * 0.15;
    this->xRot -= yo * 0.15;

    if (this->xRot < -90.0f) this->xRot = -90.0f;
    if (this->xRot > 90.0f) this->xRot = 90.0f;
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

void Player::move(float xa, float ya, float za) {
    float xaOrg = xa;
    float yaOrg = ya;
    float zaOrg = za;

    AABB bb = this->bb.expand(xa, ya, za);
    std::vector<AABB> cubes = this->level->getCubes(bb);

    size_t i;
    for (i = 0; i < cubes.size(); i++) {
        ya = cubes.at(i).clipYCollide(this->bb, ya);
    }
    this->bb.move(0.0f, ya, 0.0f);
    for (i = 0; i < cubes.size(); i++) {
        xa = cubes.at(i).clipXCollide(this->bb, xa);
    }
    this->bb.move(xa, 0.0f, 0.0f);
    for (i = 0; i < cubes.size(); i++) {
        za = cubes.at(i).clipZCollide(this->bb, za);
    }
    this->bb.move(0.0f, 0.0f, za);

    this->onGround = yaOrg != ya && yaOrg < 0.0f;

    if (xaOrg != xa) this->xd = 0.0f;
    if (yaOrg != ya) this->yd = 0.0f;
    if (zaOrg != za) this->zd = 0.0f;

    this->x = (this->bb.x0 + this->bb.x1) / 2.0f;
    this->y = this->bb.y0 + 1.62f;
    this->z = (this->bb.z0 + this->bb.z1) / 2.0f;
}

void Player::moveRelative(float xa, float za, float speed) {
    float dist = xa * xa + za * za;

    if (dist < 0.01f) return;
    dist = speed / sqrt(dist);
    xa *= dist;
    za *= dist;

    float sin = sinf(this->yRot * PI / 180.0f);
    float cos = cosf(this->yRot * PI / 180.0f);

    this->xd += xa * cos - za * sin;
    this->zd += za * cos + xa * sin;
}