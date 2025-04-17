#pragma once

#include "base.hpp"
#include <chrono>

class Timer {
    private:
        static const std::int64_t NS_PER_SECOND = 1000000000;
        static const std::int64_t MAX_NS_PER_UPDATE = 1000000000;
        static const std::int32_t MAX_TICKS_PER_UPDATE = 100;
        float ticksPerSecond;
        std::int64_t lastTime;
        
    public:
        std::int32_t ticks;
        float a;
        float timeScale = 1.0f;
        float fps = 0.0f;
        float passedTime = 0.0f;

        static std::int64_t getTimeInNanoSeconds(void);
        static std::int64_t getTimeInMilliSeconds(void);
        Timer(float ticksPerSecond);
        void advanceTime(void);
};