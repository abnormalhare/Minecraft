#include "timer.h"

s64 NS_PER_SECOND = 1000000000;
s64 MAX_NS_PER_UPDATE = 1000000000;
s32 MAX_TICKS_PER_UPDATE = 100;

Timer* newTimer(float ticksPerSecond) {
    Timer* timer = malloc(sizeof(Timer));

    if (!timer) {
        fprintf(stderr, "Failed to allocate for timer");
    }

    timer->ticksPerSecond = ticksPerSecond;
    timer->lastTime = nanoTime();
    timer->ticks = 0;
    timer->timeScale = 1.0f;
    timer->fps = 0.0f;
    timer->passedTime = 0.0f;

    return timer;
}

void advanceTime(Timer* this) {
    s64 now = nanoTime();
    s64 passedNs = now - this->lastTime;
    this->lastTime = now;
    
    if (passedNs < 0) {
        passedNs = 0;
    }
    if (passedNs > 1000000000) {
        passedNs = 1000000000;
    }
    this->fps = 1000000000 / (float)passedNs;
    this->passedTime += (float)passedNs * this->timeScale * this->ticksPerSecond / 1000000000.0f;
    this->ticks = (s32)this->passedTime;
    if (this->ticks > 100) {
        this->ticks = 100;
    }
    this->passedTime -= this->ticks;
    this->a = this->passedTime;
}