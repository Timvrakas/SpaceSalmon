#pragma once

#include <FreeRTOS.h>
#include <task.h>
#include "Task.hpp"
#include "event_groups.h"
#include <hal_rtos.h>

#define VALVES_UPDATED 0b01
#define NUM_SOLENOIDS 8

typedef enum{
    OPEN = 1,
    CLOSED = 0,
} solenoid_state_t; // state indicators for SVs to keep the nomenclature consistent

typedef enum{
    NORMALLY_CLOSED = OPEN,
    NORMALLY_OPEN = CLOSED,
} solenoid_normal_t; // defines when the solenoid ought to be powered

typedef struct {
    char* name;
    uint8_t pwm;
    solenoid_normal_t normal;
} solenoid_t;

class ValveTask : public Task<2000> {
    public:
        ValveTask(uint8_t priority, uint8_t valve_pin_start);
        ValveTask(uint8_t priority);

        void activity();
        void updateValves();
 
    private:
        uint8_t valve_pin_start;
        solenoid_t valves[NUM_SOLENOIDS];

        StaticEventGroup_t evBuf;
        EventGroupHandle_t valveManager;
};