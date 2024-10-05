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

void advanceTime(Timer* timer) {
    s64 now = nanoTime();
    s64 passedNs = now - timer->lastTime;
    timer->lastTime = now;
    
    if (passedNs < 0) {
        passedNs = 0;
    }
    if (passedNs > 1000000000) {
        passedNs = 1000000000;
    }
    timer->fps = 1000000000 / (float)passedNs;
    timer->passedTime += (float)passedNs * timer->timeScale * timer->ticksPerSecond / 1000000000.0f;
    timer->ticks = (s32)timer->passedTime;
    if (timer->ticks > 100) {
        timer->ticks = 100;
    }
    timer->passedTime -= timer->ticks;
    timer->a = timer->passedTime;
}