#include "character/Zombie.hpp"

ZombieModel Zombie::zombieModel;

Zombie::Zombie(std::shared_ptr<Level>& level, GLFWwindow* window, float x, float y, float z) : Entity(level, window) {
    this->rotA = (randd + 1.0) * 0.01f;
    zombieModel = ZombieModel();
    
    this->setPos(x, y, z);
    
    this->timeOffs = randd * 1239813.0f;
    this->rot = randd * PI * 2.0;
    this->speed = 1.0f;
}

void Zombie::tick(void) {
    this->xo = this->x;
    this->yo = this->y;
    this->zo = this->z;

    float xa = 0.0f;
    float ya = 0.0f;

    if (this->y < -100.0f) {
        this->remove();
    }

    this->rot += this->rotA;
    this->rotA *= 0.99;
    this->rotA = float(this->rotA + (randd - randd) * randd * randd * 0.08);

    xa = sin(this->rot);
    ya = cos(this->rot);
    
    if (this->onGround && randd < 0.08) this->yd = 0.5f;

    this->moveRelative(xa, ya, this->onGround ? 0.01f : 0.02f);
    this->yd -= 0.08;
    this->move(this->xd, this->yd, this->zd);
    
    this->xd *= 0.91f;
    this->yd *= 0.98f;
    this->zd *= 0.91f;

    if (this->onGround) {
        this->xd *= 0.7f;
        this->zd *= 0.7f;
    }
}

void Zombie::render(float a) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, Textures::loadTexture("char.png", GL_NEAREST));
    glPushMatrix();

    double time = Timer::getTimeInNanoSeconds() / 1.0E9 * 10.0 * this->speed + this->timeOffs;
    float size = 0.058333334f;
    float yy = abs(sin(time * 0.6662)) * 5.0 - 23.0;

    glTranslatef(this->xo + (this->x - this->xo) * a, this->yo + (this->y - this->yo) * a, this->zo + (this->z - this->zo) * a);
    glScalef(1.0f, -1.0f, 1.0f);
    glScalef(size, size, size);
    glTranslatef(0.0f, yy, 0.0f);

    float c = 180.0 / PI;
    glRotatef(this->rot * c + 180.0f, 0.0f, 1.0f, 0.0f);
    zombieModel.render(time);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}