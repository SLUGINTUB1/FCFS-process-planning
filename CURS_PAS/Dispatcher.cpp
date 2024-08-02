#include <Dispatcher.h>

using namespace std;

QList<ProcessInfo> Dispatcher::processes;
QList<pid_t> Dispatcher::buffer;
bool Dispatcher::procesorteken;

Dispatcher::Dispatcher(int n, int m, int k, int x){
    semaphore_memory_close();
    NMKX_initialization(n,m,k,x);
    Dispatcher::procesorteken=false;

    // Реєстрація обробника сигналу для SIGUSR1 SIGUSR2 з розширеною інформацією
    struct sigaction sigact;
    memset(&sigact, 0, sizeof(sigact));
    sigact.sa_sigaction = child_signal_handler;
    sigact.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &sigact, NULL);
    sigaction(SIGUSR2, &sigact, NULL);

}
//запуск логіки диспетчера
void Dispatcher::start(){
    semaphore_memory_initialization();

    create_processes();

    wait_for_generetors();


    //Якщо повідомлень не 0, то процес засинає на 10 мс
    int value;
    sem_getvalue(sem, &value);
    while(value != 0){
        sem_getvalue(sem, &value);
        usleep(10000);

    }

    //відправлення повідомлень про припинення роботи
    Message data;
    data.functionId = -1;
    for(int i=0;i<M;i++){
        if (mq_send(mq, (const char*)&data, sizeof(data), 0) == -1) {
            i--;
        }
        else{
            sem_post(sem);
            sem_getvalue(sem, &value);
            cout << "Stop signal "<< i + 1 <<" semaphore "<<value << endl;
        }
    }

    wait_for_executors();


    semaphore_memory_close();
    Dispatcher::procesorteken=false;
    Dispatcher::processes.clear();
    Dispatcher::buffer.clear();
}

void Dispatcher::NMKX_initialization(int n, int m, int k, int x){
    N=n;
    M=m;
    K=k;
    X=x;
}
//ініціалізація черги повідомлень і семафорів
int Dispatcher::semaphore_memory_initialization(){
    mq = mq_open(MSG_QUEUE_NAME, O_CREAT | O_EXCL | O_RDWR | O_NONBLOCK, 0666, NULL);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        mq_close(mq);
        return -1;
    }
    sem = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0666, 0);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        mq_close(mq);
        return -1;
    }
    mutm = sem_open(MUT_MEM_NAME, O_CREAT | O_EXCL, 0666, BUFFER_SIZE);
    if (mutm == SEM_FAILED) {
        perror("sem_open");
        mq_close(mq);
        sem_close(sem);
        return -1;
    }
    mutf = sem_open(MUT_FILE_NAME, O_CREAT | O_EXCL, 0666, 1);
    if (mutf == SEM_FAILED) {
        perror("sem_open");
        mq_close(mq);
        sem_close(sem);
        sem_close(mutm);
        return -1;
    }
    return 0;
}
//закриття черги повідомлень і семафорів
void Dispatcher::semaphore_memory_close(){
    // Закриття та роз’єднання черги повідомлень
    mq_close(mq);
    mq_unlink(MSG_QUEUE_NAME);

    // Закриття та роз’єднання семафорів
    sem_close(Dispatcher::sem);
    sem_close(mutm);
    sem_close(mutf);
    sem_unlink(SEM_NAME);
    sem_unlink(MUT_MEM_NAME);
    sem_unlink(MUT_FILE_NAME);
}
//створення процесів
void Dispatcher::create_processes(){
    // Створюємо процеси-генератори
    for (int i = 0; i < N; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            string numStr1 = to_string(i);
            const char* charArg1 = numStr1.c_str();
            string numStr2 = to_string(K);
            const char* charArg2 = numStr2.c_str();
            string numStr3 = to_string(X);
            const char* charArg3 = numStr3.c_str();
            cout<<"Create generator "<< i <<endl;
            execlp("./Generator",
                   "./Generator",
                   charArg1, charArg2, charArg3, NULL);
            perror("execlp");
            exit(1);
        }
        else{
            ProcessInfo proc; proc.pid = pid; proc.type = generator;
            Dispatcher::processes.append(proc);
        }
    }
    for (int i = 0; i < M; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            string numStr = to_string(i);
            const char* charArg1 = numStr.c_str();
            cout<<"Create executor "<< i <<endl;
            execlp("./Executor",
                   "./Executor",
                   charArg1,NULL);
            perror("execlp");
            exit(1);
        }
        else{
            ProcessInfo proc; proc.pid = pid; proc.type = executor;
            Dispatcher::processes.append(proc);
        }
    }
}
//Очікування завершення всіх дочірніх процесів генераторів
void Dispatcher::wait_for_generetors(){
    // Очікування завершення всіх дочірніх процесів
    for(auto proc : Dispatcher::processes){
        if(proc.type == generator){
            int status;
            pid_t wait_result;

            // Повторіть спробу waitpid, доки він не повернеться успішно або не виникне помилка, відмінна від EINTR
            while ((wait_result = waitpid(proc.pid, &status, 0)) == -1 && errno == EINTR);

            if (wait_result == -1) {
                // Обробка помилок при збої waitpid
                cerr << "waitpid() failed: " << strerror(errno) << endl;
                return;
            }

            // Перевірка статусу та відповідна обробка
            if (WIFEXITED(status)) {
                cout << "Generator "<<proc.pid<<" process exited with status: " << WEXITSTATUS(status) << endl;
            } else if (WIFSIGNALED(status)) {
                cout << "Generator "<<proc.pid<<" process terminated by signal: " << WTERMSIG(status) << endl;
            } else {
                cout << "Generator "<<proc.pid<<" process state changed but not exited or terminated by signal." << endl;
            }

        }
    }
}
//Очікування завершення всіх дочірніх процесів виконавеців
void Dispatcher::wait_for_executors(){
    // Очікування завершення всіх дочірніх процесів
    for(auto proc : Dispatcher::processes){
        if(proc.type == executor){
            int status;
            pid_t wait_result;

            // Повторіть спробу waitpid, доки він не повернеться успішно або не виникне помилка, відмінна від EINTR
            while ((wait_result = waitpid(proc.pid, &status, 0)) == -1 && errno == EINTR);

            if (wait_result == -1) {
                // Обробка помилок при збої waitpid
                cerr << "waitpid() failed: " << strerror(errno) << endl;
                return;
            }
             // Перевірка статусу та відповідна обробка
            if (WIFEXITED(status)) {
                cout << "Executor "<<proc.pid<<" process exited with status: " << WEXITSTATUS(status) << endl;
            } else if (WIFSIGNALED(status)) {
                cout << "Executor "<<proc.pid<<" process terminated by signal: " << WTERMSIG(status) << endl;
            } else {
                cout << "Executor "<<proc.pid<<" process state changed but not exited or terminated by signal." << endl;
            }
        }
    }
}

// Функція обробки сигналів
void Dispatcher::child_signal_handler(int signal, siginfo_t *info, void *ptr) {
    if (signal == SIGUSR1) {
        Dispatcher::buffer.append(info->si_pid);
        cout << "Dispatcher received SIGUSR1 signal from child with PID: " << info->si_pid << endl;

        if(!procesorteken){
            procesorteken = true;
            pid_t leaving_proc = Dispatcher::buffer.takeFirst();
            while(kill(leaving_proc, SIGCONT)==-1){
                cout<<"SIGUSR2 ";
                perror("kill");
            }
            cout << "Process: " << leaving_proc <<" left the buffer"<< endl;
        }

            hatchet();
    }
    else if(signal == SIGUSR2){
        procesorteken = false;
        cout << "Dispatcher received SIGUSR2 signal from child with PID: " << info->si_pid << endl;
        if(Dispatcher::buffer.size() != 0){
                if(!procesorteken){
                procesorteken = true;
                pid_t leaving_proc = Dispatcher::buffer.takeFirst();
                while(kill(leaving_proc, SIGCONT)==-1){
                    cout<<"SIGUSR2 ";
                    perror("kill");
                }
                cout << "Process: " << leaving_proc <<" left the buffer"<< endl;
                }
        }

        if(!procesorteken)
            hatchet();
    }
}

void Dispatcher::hatchet(){
    for(auto proc : Dispatcher::processes){
        if(kill(proc.pid, 0) != -1){
            if(Dispatcher::buffer.indexOf(proc.pid)==-1){
                while(kill(proc.pid, SIGCONT)==-1){
                    cout<<"hatchet SIGCONT ";
                    perror("kill");
                }
            }
            else{
                while(kill(proc.pid, SIGSTOP)==-1){
                    cout<<"hatchet SIGSTOP ";
                    perror("kill");
                }
            }
        }
    }
}
