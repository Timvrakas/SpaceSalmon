#pragma once
#include "FreeRTOS.h"
#include "task.h"

template <int stackSize>
class Task {
    private:
        TaskHandle_t taskHandle;
        StaticTask_t xTaskBuffer;
        StackType_t xStack[stackSize];

    public:
        Task(uint8_t priority, const char *name) {
            Task::taskHandle = xTaskCreateStatic(Task::TaskFunctionAdapter, //static function to run
                                                name,                      //task name
                                                stackSize,                 //stack depth (words!)
                                                this,                      //parameters
                                                priority,                  //priority
                                                xStack,                    //stack object
                                                &xTaskBuffer);             //TCB object
        }

        // This needs to be overriden in derived tasks
        virtual void activity()
        {
            vTaskSuspend(getTaskHandle()); //go commit die
        };

        TaskHandle_t getTaskHandle()
        {
            return taskHandle;
        }

        void suspend(void){
            vTaskSuspend(getTaskHandle());
        }

        void resume(void){
            vTaskResume(getTaskHandle());
        }

        eTaskState get_state(void){
            return eTaskGetState(getTaskHandle());
        }

        static void TaskFunctionAdapter(void *pvParameters)
        {
            Task *task = static_cast<Task *>(pvParameters);

            task->activity();

            vTaskSuspend(task->taskHandle);
        }
};