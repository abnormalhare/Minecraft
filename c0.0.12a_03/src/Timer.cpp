#include "Timer.hpp"

std::int64_t Timer::getTimeInNanoSeconds(void) {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

std::int64_t Timer::getTimeInMilliSeconds(void) {
    return getTimeInNanoSeconds() / 1000000;
}

Timer::Timer(float ticksPerSecond) {
    this->ticksPerSecond = ticksPerSecond;
    this->lastTime = getTimeInNanoSeconds();
}

void Timer::advanceTime(void) {
    std::int64_t now = getTimeInNanoSeconds();
    std::int64_t passedNs = now - this->lastTime;
    this->lastTime = now;

    if (passedNs < 0) {
        passedNs = 0;
    }
    if (passedNs > MAX_NS_PER_UPDATE) {
        passedNs = MAX_NS_PER_UPDATE;
    }

    this->fps = (passedNs / (float)NS_PER_SECOND);
    this->passedTime += passedNs * this->timeScale * this->ticksPerSecond / (float)NS_PER_SECOND;
    this->ticks = this->passedTime;
    if (this->ticks > MAX_TICKS_PER_UPDATE) {
        this->ticks = MAX_TICKS_PER_UPDATE;
    }
    this->passedTime -= this->ticks;
    this->a = this->passedTime;
}