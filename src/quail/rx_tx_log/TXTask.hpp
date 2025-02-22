#pragma once

#include <FreeRTOS.h>
#include <task.h>
#include <message_buffer.h>
#include <semphr.h>
#include <hal_rtos.h>
#include <Adafruit_SleepyDog.h>
#include "event_groups.h"
#include "ArduinoJson.h"
#include "Task.hpp"
#include "CmdServer.hpp"
#include "SlateServer.hpp"

#define RADIO_FACTOR 1 // controls frequency of radio TX wrt USB TX - gives a radio interval of tx_interval_ms*RADIO_FACTOR
#define LOG_FACTOR 1 // controls frequency of logging wrt USB TX - gives a log interval of tx_interval_ms/LOG_FACTOR

class TXTask:Task<4000>{
public:
    TXTask(uint8_t priority, uint16_t tx_interval_ms);
    void activity(); // every so often, collect state JSON, log and write to selected TX
    static void writeUSB(const char* buf); // responsibly write to USB
private:
    const uint16_t tx_interval_ms; // frequency, in milliseconds, of TX
    char json_buffer[DATA_PCKT_LEN]; //create char buffer with space
};