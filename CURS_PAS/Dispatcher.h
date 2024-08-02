#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <common.h>
#include <QList>

enum Type {generator, executor};

struct ProcessInfo{
    pid_t pid;
    Type type;

    bool operator==(const ProcessInfo &other) const {
        return pid == other.pid; // Compare based on PID or other unique identifier
    }
};

class Dispatcher{
public:
    int N;//кількість генераторів
    int M;//кількість виконавців
    int K;//кількість передач даних генератора
    int X;//мілісекунд очікування після успіху
    static bool procesorteken;
    static QList<pid_t> buffer;//буфер
    static QList<ProcessInfo> processes;//процеси
    mqd_t mq;//чергa повідомлень
    sem_t* sem;//семафор скільки повідомлень
    sem_t* mutm;//семафор буферy
    sem_t* mutf;//семафор для м'ютексу у файлі

    Dispatcher(int n, int m, int k, int x);

    void NMKX_initialization(int n, int m, int k, int x);

    void start();

    int semaphore_memory_initialization();

    void semaphore_memory_close();

    void create_processes();

    void wait_for_generetors();

    void wait_for_executors();

    static void child_signal_handler(int signal, siginfo_t *info, void *ptr);

    static void hatchet();
};

#endif // DISPATCHER_H
