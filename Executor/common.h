#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <mqueue.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

#define BUFFER_SIZE 8
#define MSG_QUEUE_NAME "/function_queue"
#define SEM_NAME "/function_semaphore"
#define MUT_MEM_NAME "/memory_semaphore"
#define MUT_FILE_NAME "/file_semaphore"

// Визначаємо структуру повідомлення
struct Message {
    double start;
    double step;
    double end;
    int functionId;
};

#endif // COMMON_H
