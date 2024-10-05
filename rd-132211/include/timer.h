#ifndef TIMER_H
#define TIMER_H

#include "types.h"
#include "java/time.h"

#include <windows.h>
#include <stdint.h>
#include <stdio.h>

typedef struct _Timer {
    float ticksPerSecond;
    s64 lastTime;
    s32 ticks;
    float a;
    float timeScale;
    float fps;
    float passedTime;
} Timer;

Timer* newTimer(float ticksPerSecond);
void advanceTime(Timer* timer);

#endif