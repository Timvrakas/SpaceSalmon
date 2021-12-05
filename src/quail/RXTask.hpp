#pragma once

#include <FreeRTOS.h>
#include <task.h>
#include "MsgBuffer.hpp"
#include <semphr.h>
#include <timers.h>
#include <hal_rtos.h>
#include "event_groups.h"
#include "ArduinoJson.h"
#include "Task.hpp"

#define MAX_CMD_LENGTH 255 // maximum length of a command, in bytes (based on radio command max size)
#define CMD_ENDLINE 0x0A // endline indicator for a command - null char

typedef struct { uint8_t data[MAX_CMD_LENGTH]; } cmd_packet_t;

class RXTask: public Task<2000>
{
public:
    RXTask(uint8_t priority, uint16_t rx_interval_ms);
    void activity();
    void sendcmd(const char* cmd); // manually write data to the input buffer for debugging

private:
    const uint16_t rx_interval_ms; // time to wait before checking command buffer
    void readInput(); // check if input is available, returns true if successful read
    uint8_t readByte(); // read a byte from the input buffer (radio or serial)
    MsgBuffer<cmd_packet_t, MAX_CMD_LENGTH*2> cmdbuf; // buffer for input - can hold 10 commands before getting full
    StaticJsonDocument<MAX_CMD_LENGTH> curr_cmd; // storage for most recently received json
    StaticJsonDocument<MAX_CMD_LENGTH> wait_cmd; // storage for command used in waitThen callback
    static StaticJsonDocument<1024> usercmds; // storage for usercmds, cmd strings defined by the user
    //bool usercmds_valid; // true if usercmds are valid

    TimerHandle_t pulseTimers[8]; // xTimers for callback (timer IDS correspond to the array index & solenoid channel)
    StaticTimer_t pulsebufs[8]; // xTimer static buffer for pulse timers
    TimerHandle_t waitTimer; // xTimer for waitThen commands
    StaticTimer_t waitbuf; // xTimer static buffer for waitTimer

    void process_cmd_json(JsonObjectConst cmd); // uses the command defs from above to determine what to do with a received command
    void process_cmd_array(JsonArrayConst cmd_arr);

    // functions used for executing commands
    static void open_solenoid(uint8_t sol_ch); // sets solenoid state to open
    static void open_solenoids(JsonArrayConst sol_ch); // sets multiple solenoid state to open
    static void close_solenoid(uint8_t sol_ch); // sets solenoid state of given channel to closed
    static void close_solenoids(JsonArrayConst sol_ch); // sets solenoid state of multiple channels to closed
    void pulse_solenoid(uint8_t sol_ch, uint16_t pulse_dur_ms); // reads a time in milliseconds from serial, opens solenoid, sets a timer to close solenoid after specified duration
    void pulse_solenoids(JsonArrayConst sol_ch, uint16_t pulse_dur_ms);
    static void _close_solenoid(TimerHandle_t xTimer); //close the solenoid associated with the given xTimer
    static void fire_ematch(uint8_t em_ch);
    static void fire_ematches(JsonArrayConst em_ch);
    void wait_then(JsonObjectConst cmd, uint16_t wait_time);
    void wait_callback(TimerHandle_t xTimer);

    // TODO: implement the following
    // void config_radio(); // reads radio settings from serial and sends to radio settings msgbuffer
    // void abort_fuel(); // aborts fuel-side 
    // void abort_ox(); // aborts ox-side
    // void launch(); // starts launch sequence
    // static void fire_igniter(TimerHandle_t xTimer); // fires engine igniter ematches
    // static void open_main_valves(TimerHandle_t xTimer); // opens main engine valves
    // void cancel_launch(); // cancels the launch sequence
};