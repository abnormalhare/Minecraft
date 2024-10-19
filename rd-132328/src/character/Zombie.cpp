#include "character/Zombie.hpp"

Zombie::Zombie(Level* level, GLFWwindow* window, float x, float y, float z) : Entity(level, window) {
    this->rotA = (randf + 1.0) * 0.01f;
    
    this->x = x;
    this->y = y;
    this->z = z;

    this->timeOffs = randf * 1239813.0f;
    this->rot = randf * PI * 2;
    this->speed = 1.0f;

    this->head = new Cube(0, 0);
    this->head->addBox(-4.0f, -8.0f, -4.0f, 8, 8, 8);
    this->body = new Cube(16, 16);
    this->body->addBox(-4.0F, 0.0F, -2.0F, 8, 12, 4);
    this->arm0 = new Cube(40, 16);
    this->arm0->addBox(-3.0F, -2.0F, -2.0F, 4, 12, 4);
    this->arm0->setPos(-5.0F, 2.0F, 0.0F);
    this->arm1 = new Cube(40, 16);
    this->arm1->addBox(-1.0F, -2.0F, -2.0F, 4, 12, 4);
    this->arm1->setPos(5.0F, 2.0F, 0.0F);
    this->leg0 = new Cube(0, 16);
    this->leg0->addBox(-2.0F, 0.0F, -2.0F, 4, 12, 4);
    this->leg0->setPos(-2.0F, 12.0F, 0.0F);
    this->leg1 = new Cube(0, 16);
    this->leg1->addBox(-2.0F, 0.0F, -2.0F, 4, 12, 4);
    this->leg1->setPos(2.0F, 12.0F, 0.0F);
}

void Zombie::tick(void) {
    this->xo = this->x;
    this->yo = this->y;
    this->zo = this->z;

    float xa = 0.0f;
    float ya = 0.0f;
    
    this->rot += this->rotA;
    this->rotA = ((double)this->rotA) * 0.99;
    this->rotA = (((double)this->rotA) + randf - randf) * randf * randf * 0.01;

    xa = sin(this->rot);
    ya = cos(this->rot);

    if (this->onGround && randf < 0.01) {
        this->yd = 0.12f;
    }
    this->moveRelative(xa, ya, this->onGround ? 0.02f : 0.005f);
    this->yd = ((double)this->yd) - 0.005;
    
    this->move(this->xd, this->yd, this->zd);
    this->xd *= 0.91f;
    this->yd *= 0.98f;
    this->zd *= 0.91f;
    
    if (this->y > 100.0f) resetPos();
    if (this->onGround) {
        this->xd *= 0.8f;
        this->zd *= 0.8f;
    }
}

void Zombie::render(float a) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, Textures::loadTexture("char.png", GL_NEAREST));
    glPushMatrix();

    double time = Timer::getTimeInNanoSeconds() / 1.0E9 * 10.0 * this->speed + this->timeOffs;
    float size = 7.0f / 120.0f;
    float yy = (-abs(sin(time * 0.6662)) * 5.0 - 23.0);

    glTranslatef(this->xo + (this->x - this->xo) * a, this->yo + (this->y - this->yo) * a, this->zo + (this->z - this->zo) * a);
    glScalef(1.0f, -1.0f, 1.0f);
    glScalef(size, size, size);
    glTranslatef(0.0f, yy, 0.0f);

    float c = 180.0f / PI;
    glRotatef(this->rot * c + 180.0f, 0.0f, 1.0f, 0.0f);
    this->head->yRot = sin(time * 0.83) * 1.0f;
    this->head->xRot = sin(time) * 0.8f;
    this->arm0->xRot = sin(time * 0.6662 + PI) * 2.0f;
    this->arm0->zRot = (sin(time * 0.2312) + 1.0) * 1.0f;
    this->arm1->xRot = sin(time * 0.6662) * 2.0f;
    this->arm1->zRot = (sin(time * 0.2812) - 1.0) * 1.0f;
    this->leg0->xRot = sin(time * 0.6662) * 1.4f;
    this->leg1->xRot = sin(time * 0.6662 + PI) * 1.4f;

    this->head->render();
    this->body->render();
    this->arm0->render();
    this->arm1->render();
    this->leg0->render();
    this->leg1->render();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}