# Space Salmon

Space Salmon is a a flight computer, designed by Tim Vrakas for the Stanford Student Space Initaive in 2018. This codebase provides datalogging, and eventually flight software development.

## ThunderGuppy
ThunderGuppy is the hardware version based on the SpaceSalmon architecure. It has similar hardware, but is designed to fit in a 2" airframe.

### How To Setup The Build environment
* Install VSCode
* Add the extension Platform IO in the extension tab on the left
* Using git, clone the repo at https://github.com/stanford-ssi/SpaceSalmon.git 
* Open the the repo in VSCode
* Follow the instructions in the "Weird Bugs Section" of the README
* Use the check button in the bottom bar of the VSCode window to build
* Use the right arrow button next to the build button to upload

### Weird Build Bugs
* You need to edit the `cstdio` file in the folowing location: `~/.platformio/packages/toolchain-gccarmnoneeabi/arm-none-eabi/include/c++/7.2.1/cstdio` and comment out these five `#undef` entries. They are not all next to each other in the file.  
//#undef printf  
//#undef sprintf  
//#undef vsprintf  
//#undef snprintf  
//#undef vsnprintf

### Uploading
If BOSSAC uploading fails, use this:  
`python uf2conv.py -b 0x4000 .pio\build\guppy\firmware.bin`  

### Lessons Learned
* You can't write data into a buffer into another task, beacuse you don't know that it's constructor has been intialized! The main case is logging. Don't try logging from the constructors! We should probably prevent the logger from taking input before its initialized...

### TODOs
* fix the `#undef` bug listed above
* fix the build version script, have it linked in last
* Port back to Ardunio (going on in `arduino-backport` branch)
* fix but that caused no deploy

### SHITL
The code supports SHITL (Some Hardware in the Loop) testing. This allows you to feed a log of sensor data from a past flight into the hardware in the place of live sensor data. To perform SHTIL you must do the following. The logs from a SHITL test are treated the same as flight logs.
* Enable the `sys.shitl` boolean flag, which will require you to rebuilding and reflash the firmware. The blinking pattern will be noticably different than when running off sensor data. In SHITL mode, the reading of sensor data from SD card will illuminate the Sensor LED.
* Place a `shitl.txt` file on the SD card. You may trim from both ends of a flight log to make the SHITL test faster.
Under SHITL, the system runs at ~180% speed, which is limited by SD access and CPU time. Sensor data is not re-logged, but filter data and flightPlan logs are printed for every data point provided.
After performing a SHITL, the output filter data should be compared to implementations in `sim/`, and the objectives of the flight plan should be verified (Test with real igniters!)

### Task Priority
4 - Filter  
3 - Sensor  
2 - Altimeter  
1 - Logger  
