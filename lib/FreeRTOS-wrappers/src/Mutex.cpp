#include "Mutex.hpp"

Mutex::Mutex(){
    handle = xSemaphoreCreateMutexStatic(&buf);
}

void Mutex::take(TickType_t block){
    xSemaphoreTake(handle, block);
}

void Mutex::give(){
    xSemaphoreGive(handle);
}

Mutex::~Mutex(){
    vSemaphoreDelete(handle);
}

//When copy-constructing, just make a new mutex:
Mutex::Mutex(const Mutex& e): Mutex(){}