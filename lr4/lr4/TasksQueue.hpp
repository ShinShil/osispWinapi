#pragma once 
#include <Windows.h>
#include <iostream>

typedef int TASK_FUNC; //typedef for thread lpstartroutine

typedef struct _TASK_QUEUE_ELEM {
    TASK_FUNC task;
    struct _TASK_QUEUE_ELEM* next;
} TASK_QUEUE_ELEM, * PTASK_QUEUE_ELEM;

using namespace std;

class TasksQueue {
private:
    PTASK_QUEUE_ELEM head = NULL;
    PTASK_QUEUE_ELEM last = NULL;
public:
    void push(TASK_FUNC task) {
        PTASK_QUEUE_ELEM newElem = new TASK_QUEUE_ELEM;
        newElem->task = task;
        newElem->next = NULL;
        if (head == NULL) {
            head = newElem;
        }
        else {
            last->next = newElem;
        }
        last = newElem;
    }
    TASK_FUNC pop() {
        if (empty() == FALSE) {
            TASK_FUNC res = head->task;
            PTASK_QUEUE_ELEM tmp = head;
            head = head->next;
            delete tmp;
            return res;
        }
        else {
            return NULL;
        }
    }
    BOOL empty() {
        return head == NULL;
    }
};