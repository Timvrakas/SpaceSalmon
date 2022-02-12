#pragma once

#include <FreeRTOS.h>
#include <task.h>
#include <message_buffer.h>
#include <semphr.h>
#include "event_groups.h"
#include <hal_rtos.h>

#include "Task.hpp"

#define NEW_DATA 0b01
#define SENSORS_READY 0b10
#define ERROR_THRESHOLD 10
#define READ_TIMOUT 100

class ADCTask : public Task<2000>
{
public:
    ADCTask(uint8_t priority);

    struct {
        uint32_t dataword;
        uint8_t channel;
    } typedef adcdata_t;

    /**
     * @brief Initializes the ADC, after execution ready to process data
     */ 
    void initADC();

    /**
     * @brief set flag to indicate that all sensor channels have been configured
     */
    void sensorsConfigured();

    /**
     * @brief Restore ADC to a status where it can be initialized
     */
    void resetADC();

    /**
     * @brief Read data from ADC into curr_data, assumes new data is present
     */
    long getData(adcdata_t &data);

    void activity();

private:
    static void adcISR();
    StaticEventGroup_t evbuf;
    EventGroupHandle_t evgroup;

    uint8_t timeout_count = 0;
    uint8_t err_count = 0;
    uint8_t data_count = 0;

};

#include "ArduinoJson.h"
