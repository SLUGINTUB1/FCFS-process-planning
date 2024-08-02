#include <common.h>

using namespace std;

mqd_t mq;
sem_t* sem;
sem_t* mutm;

// генеруємо випадкові числа
void generateNumbers(int id, double &begin, double &step, double &end, int &fNum)
{
    srand(time(NULL)*id);
    //генерування
    begin = rand() % 10;
    end = rand() % 10;
    step = rand() % 9 + 1;
    step = step / 10;
    fNum = rand() % 4;
    //перевірка
    if (begin == end)
        do
            begin = rand() % 3;
        while(begin == end);
    if (begin > end)
    {
        double temp;
        temp = end;
        end = begin;
        begin = temp;
    }
}

// функція для процес-генератора
void generator(int id, int K, int X, mqd_t mq, sem_t* sem, sem_t* mutm) {
    for(int i = 0; i < K;){
        //генеруємо випадкові числа
        Message data;
        generateNumbers((2 ^ id) * (3 ^ i), data.start, data.step, data.end, data.functionId);
        //Якщо буфер заповнений, то процес засинає на 10 мс
        int value;
        if(sem_getvalue(mutm, &value)==-1){
            perror("sem_getvalue");
            usleep(10000);
            continue;
        }
        if(value = 0){
            usleep(10000);
            continue;
        }
        while(sem_wait(mutm) == -1){
            cout<<"Generetor "<<getpid()<<" transfer "<< i <<" sem_wait mutm";
            perror("sem_wait");
        }
        //Доступ до буферу здійснюється в монопольному режимі
        while(kill(getppid(), SIGUSR1) == -1){
            cout<<"Generetor "<<getpid()<<" transfer "<< i <<" SIGUSR1";
            perror("kill");
        }
        while(kill(getpid(), SIGSTOP) == -1){
            cout<<"Generetor "<<getpid()<<" transfer "<< i <<" SIGSTOP";
            perror("kill");
        }
        while(sem_post(mutm) == -1){
            cout<<"Generetor "<<getpid()<<" transfer "<< i <<" sem_post mutm";
            perror("sem_wait");
        }
        sleep(1);//////////////////////////////////////////////////////////////////////////////////
        // Setting up the timeout
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += 0.1;
        if (mq_timedsend(mq, (const char*)&data, sizeof(data), 0, &ts) == -1) {
            cout<<"Generetor "<<getpid()<<" transfer "<< i <<" ";
            perror("mq_receive");
            while(kill(getppid(), SIGUSR2) == -1){
                cout<<"Generetor "<<getpid()<<" transfer "<< i <<" SIGUSR2";
                perror("kill");
            }
        }
        else{
            cout<<"Generator "<<getpid()<<" transfer "<< i + 1 <<endl;
            i++;
            //Після вдалої передачі даних процес призупиняється на Х мс
            while(sem_post(sem) == -1){
                cout<<"Generetor "<<getpid()<<" transfer "<< i <<" ";
                perror("sem_post");
            }
            while(kill(getppid(), SIGUSR2) == -1){
                cout<<"Generetor "<<getpid()<<" transfer "<< i <<" SIGUSR2";
                perror("kill");
            }
            usleep(1000*X);
        }
    }
    //Після здійснення К вдалих циклів передачі завершує свою роботу
    return;
}

void handleSigterm(int signum) {
    cout << "Generator "<<getpid()<<" received SIGTERM (signal " << signum << "). Terminating" << endl;
    //закриття семафора та черги повідомлень
    mq_close(mq);
    sem_close(sem);
    sem_close(mutm);

    exit(signum);
}


int main(int argc, char *argv[])
{
    if (argc != 4) {
        cout<< argv[0]<<" Arguments: GeneratorID KNumberOfTransfers XmsStop"<<endl;
        return -1;
    }

    signal(SIGTERM, handleSigterm);

    // Ініціалізація семафора та черги повідомлень
    mq = mq_open(MSG_QUEUE_NAME, O_WRONLY);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        exit(1);
    }
    sem = sem_open(SEM_NAME, 0);
    if (sem == SEM_FAILED) {
        cerr << "Semaphore creation error" << endl;
        mq_close(mq);
        return -1;
    }
    mutm = sem_open(MUT_MEM_NAME, 0);
    if (mutm == SEM_FAILED) {
        cerr << "Semaphore for memory creation error" << endl;
        mq_close(mq);
        sem_close(sem);
        return -1;
    }


    generator(atoi(argv[1]),atoi(argv[2]),atoi(argv[3]), mq, sem, mutm);

    //закриття семафора та черги повідомлень
    mq_close(mq);
    sem_close(sem);
    sem_close(mutm);

    return 0;
}
