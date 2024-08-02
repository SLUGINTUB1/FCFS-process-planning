#include <common.h>

using namespace std;

Message *messageData;
mqd_t mq;
sem_t* sem;
sem_t* mutm;
sem_t* mutf;

// табулювання
int tabulation(int id1, int id2){
    //відкриваємо файл
    string fileName = "output.txt";
    ofstream outputFile;
    outputFile.open(fileName, ios::app);

    if (!outputFile.is_open()) {
        cerr << "Failed to open the file." << endl;
        return -1;
    }

    //записуємо результати
    for (double x = messageData->start; x <= messageData->end; x += messageData->step) {
        double result = 0;
        switch (messageData->functionId) {
        case 0: result = x; break;
        case 1: result = sin(x); break;
        case 2: result = cos(x); break;
        case 3: result = x*x; break;
        default: break;
        }
        outputFile << "Executor " << id1 << " " << id2 << " | x: " << x << " | f(x): " << result << endl;
    }

    //закриваємо файл
    outputFile.close();
    return 0;
}

// функція для процес-виконавеця
void executor(int id, mqd_t mq, sem_t* sem, sem_t* mutm, sem_t* mutf) {
    int i = 0;
    while(true){

        struct mq_attr attr;
        if(mq_getattr(mq, &attr) == -1) {
            perror("mq_getattr");
            continue;
        }
        size_t msgSize = attr.mq_msgsize;

        // Динамічно розподіляти пам'ять
        messageData = (Message*)malloc(msgSize);
        if (messageData == nullptr) {
            cout <<"Executor "<<getpid()<<" transfer "<< i << " Memory allocation failed" << endl;
            continue;
        }

        //семафор чекає повідомлень
        if (sem_wait(sem) == -1) {
            free(messageData); // Звільнити пам'ять у разі помилки
            cout <<"Executor "<<getpid()<<" transfer "<< i << " sem_wait sem failed" << endl;
            perror("sem_wait");
            continue;
        }
        while(sem_wait(mutm) == -1){
            cout<<"Executor "<<getpid()<<" transfer "<< i <<" sem_wait mutm failed";
            perror("sem_wait");
        }
        //Доступ до буферу здійснюється в монопольному режимі
        while(kill(getppid(), SIGUSR1)==-1){
            cout<<"Executor "<<getpid()<<" transfer "<< i <<" SIGUSR1";
            perror("kill");
        }
        while(kill(getpid(), SIGSTOP)==-1){
            cout<<"Executor "<<getpid()<<" transfer "<< i <<" SIGSTOP";
            perror("kill");
        }
        while(sem_post(mutm) == -1){
            cout<<"Executor "<<getpid()<<" transfer "<< i <<" sem_post mutm";
            perror("sem_wait");
        }

        sleep(1);////////////////////////////////////////////////////////////

        // читання повідомлення
        if (mq_receive(mq, (char*)messageData, msgSize, NULL) == -1) {
            cout<<"Executor "<<getpid()<<" transfer "<< i <<" ";
            perror("mq_receive");
            while(sem_post(sem)==-1)
                perror("sem_post");
            while(kill(getppid(), SIGUSR2)==-1){
                cout<<"Executor "<<getpid()<<" transfer "<< i <<" SIGUSR2";
                perror("kill");
            }
            free(messageData); // Звільнити пам'ять у разі помилки
            continue;
        }

        if (messageData->functionId < 0) {
            free(messageData);
            while(kill(getppid(), SIGUSR2)==-1){
                cout<<"Executor "<<getpid()<<" transfer "<< i <<" SIGUSR2";
                perror("kill");
            }
            return; // вихід, якщо подається сигнал зупинки через канал зв'язку
        }

        i++;
        cout<<"Executor "<<getpid()<<" transfer "<< i <<endl;
        while(kill(getppid(), SIGUSR2)==-1){
            cout<<"Executor "<<getpid()<<" transfer "<< i <<" SIGUSR2";
            perror("kill");
        }

        if (sem_wait(mutf) == -1) {
            perror("sem_wait");
            free(messageData);
            continue;
        }
        if (tabulation(id, i) == -1) {
            cout<<"Error when outputing in file. Executor "<<getpid()<<" transfer"<<i<<endl;
        }
        sem_post(mutf);

        free(messageData); // Звільнити пам'ять
    }
}

void handleSigterm(int signum) {
    cout << "Executor "<<getpid()<<" received SIGTERM (signal " << signum << "). Terminating" << endl;
    free(messageData);

    //закриття семафора та черги повідомлень
    mq_close(mq);
    sem_close(sem);
    sem_close(mutm);
    sem_close(mutf);

    exit(signum);
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        cout<< argv[0]<<" Arguments: ExecutorID "<<endl;
        return -1;
    }

    signal(SIGTERM, handleSigterm);

    // Ініціалізація семафора та черги повідомлень
    mq = mq_open(MSG_QUEUE_NAME, O_RDONLY);
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
    mutf = sem_open(MUT_FILE_NAME, 0);
    if (mutf == SEM_FAILED) {
        cerr << "Semaphore for file creation error" << endl;
        mq_close(mq);
        sem_close(sem);
        sem_close(mutm);
        return -1;
    }

    executor(atoi(argv[1]) ,mq ,sem ,mutm ,mutf);

    //закриття семафора та черги повідомлень
    mq_close(mq);
    sem_close(sem);
    sem_close(mutm);
    sem_close(mutf);

    return 0;
}
