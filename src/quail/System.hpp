#pragma once

class System;

#include <Arduino.h>

#include "SPI.h"
#include "ad7124-lib/ad7124.h"


//#include "Sensor.hpp"
#include "PressureSensor.hpp"
// #include "ThermalSensor.hpp"
//#include "LoadSensor.hpp"
#include "ADCTask.hpp"
#include "StateData.hpp"
//#include "TelemTask.hpp"
//#include "RadioTask.hpp"
//#include "USBTask.hpp"
//#include "CmdTask.hpp"
//#include "SolenoidTask.hpp"
#include "FireTask.hpp"


class System
{ 
public:
    SPIClass adc_spi = SPIClass(&sercom1, 5, 6, 7, SPI_PAD_0_SCK_1, SERCOM_RX_PAD_3);
    SPIClass squib_spi = SPIClass(&sercom0, 17, 18, 19, SPI_PAD_0_SCK_1, SERCOM_RX_PAD_3);

    Ad7124Chip adc = Ad7124Chip(5,8,&adc_spi);

    //PressureSensor PT0 = PressureSensor("PT0", Ad7124::AIN0Input, RANGE_1000);
    // PressureSensor PT1 =  PressureSensor("PT1", Ad7124::AIN1Input, RANGE_1000);
    // PressureSensor PT2 =  PressureSensor("PT2", Ad7124::AIN2Input, RANGE_1000);
    // PressureSensor PT3 =  PressureSensor("PT3", Ad7124::AIN3Input, RANGE_1000);
    // PressureSensor PT4 =  PressureSensor("PT4", Ad7124::AIN4Input, RANGE_1000);
    // LoadSensor LC0 = LoadSensor("LC0", Ad7124::AIN5Input);
    // LoadSensor LC1 = LoadSensor("LC1", Ad7124::AIN7Input);

    Sensor* sensors [0] = {
        // &PT0,
        // &PT1,
        // &PT2,
        // &PT3,
        // &PT4,
        // &LC0,
        // &LC1
    };

    StateData statedata = StateData(); //holds current state of sensors/SVs/ematches systems for output + control
    
    class Tasks
    {
    public:
        ADCTask adctask = ADCTask(3); //reads from ADC
        FireTask firetask = FireTask(3, 20, 21); //fires squibs for ematches
        //SolenoidTask soltask = SolenoidTask(3); //sets solenoid states
        //TelemTask telemtask = TelemTask(3, TELEM_MODE); // passes state data to output (TX) based on mode selection
        //USBTask usbtask = USBTask(4); //collects and sends information over USB
        //RadioTask radiotask = RadioTask(4); //collects and sends information over radio
        //CmdTask cmdtask = CmdTask(3); //processes commands from radio/usb (RX)
        //SensorTask sensortask = SensorTask(1); //processes ADC values depending on sensor type
    };

    Tasks tasks;
};

#include "main.hpp"