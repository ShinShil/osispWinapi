#pragma once
#include "TasksQueue.hpp"

class TaskManager {
private:
    TasksQueue queue;
public:
    TaskManager(TasksQueue queue) {
        this->queue = queue;
    }

    void StartAllJobs() {
        while (queue.empty() == FALSE) {
            //start new thread with queue.pop()
        }
    }
};