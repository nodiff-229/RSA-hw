//
// Created by nodiff on 2022/11/6.
//

#pragma once

#include <time.h>
#include <iostream>



class Clock {
public:
    static clock_t startTime;
    static clock_t endTime;

public:
    Clock();

    virtual ~Clock();

    static void start();
    static void end();

};

void Clock::start() {
        startTime = clock();
}

void Clock::end() {
    endTime = clock();
    std::cout << "生成公私钥完成,耗时为:" << double(endTime - startTime) / CLOCKS_PER_SEC * 1000 << "ms" <<std::endl;
    startTime = 0;
    endTime = 0;
}

clock_t Clock::startTime = 0;
clock_t Clock::endTime = 0;

Clock::Clock() {}

Clock::~Clock() {

}
