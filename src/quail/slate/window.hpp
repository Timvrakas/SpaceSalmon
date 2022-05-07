#pragma once

#include "../config.h"
#include <FreeRTOS.h>

class Window {
    public:
        Window(){};

        void push(float val) { 
            curr = (curr + 1) % WINDOW_SIZE;
            arr[curr] = val; 

            lastAvg = currAvg;
            currAvg = _avg();

            lastTick = currTick;
            currTick = xTaskGetTickCount();
        }

        void operator+=(float val) { push(val); }

        float avg() { return currAvg; }

        float peek() { return arr[curr]; }

        float delta() { return (currAvg - lastAvg) / dt(); }

        TickType_t dt() { return currTick - lastTick; }

    private:
        float arr[WINDOW_SIZE];
        uint8_t curr = -1;

        float lastAvg = 0;
        float currAvg = 0;

        TickType_t lastTick = 0;
        TickType_t currTick = 1;

        float _avg() {
            float sum = 0;
            
            for (uint8_t i = 0; i < WINDOW_SIZE; i++) {
                sum += arr[i];
            }
            return sum / WINDOW_SIZE;
        }
};