#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <Dispatcher.h>

#include <thread>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,timer(new QTimer(this))
    ,dispatcher(4,4,2,100)
{
    dispatcher_running=false;
    ui->setupUi(this);

    initializeTable();

    // Налаштування таймера для оновлення таблиці кожні 0,1 секунди
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTable);
    timer->start(100); // 100 мілісекунд

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeTable() {
    ui->tableWidget->setRowCount(dispatcher.processes.size());
    ui->tableWidget->setColumnCount(3);
    QStringList headers;
    headers << "Тип" <<"PID" << "виконується";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
}

void MainWindow::updateTable() {
    initializeTable();
    if(dispatcher_running){
        for (int row = 0; row < dispatcher.processes.size(); ++row) {
            QTableWidgetItem *type = new QTableWidgetItem(dispatcher.processes[row].type == generator ? "Генератор" : "Виконавець");
            QTableWidgetItem *intItem = new QTableWidgetItem(QString::number(dispatcher.processes[row].pid));
            QTableWidgetItem *boolItem = new QTableWidgetItem(kill(dispatcher.processes[row].pid, 0)==-1 ? "закінчив" : (dispatcher.buffer.indexOf(dispatcher.processes[row].pid)==-1) ? "так" : "ні");
            errno = 0;
            ui->tableWidget->setItem(row, 0, type);
            ui->tableWidget->setItem(row, 1, intItem);
            ui->tableWidget->setItem(row, 2, boolItem);
        }
    }
}

void MainWindow::on_pushButton_clicked()
{
    if(!dispatcher_running){
        cout<<ui->spinBox->value()<<" "<<ui->spinBox_2->value()<<" "<<ui->spinBox_3->value()<<" "<<ui->spinBox_4->value()<<endl;
        dispatcher=Dispatcher(ui->spinBox->value(),ui->spinBox_2->value(),ui->spinBox_3->value(),ui->spinBox_4->value());

        dispatcher_running = true;

        std::thread t([this]() {
            this->dispatcher.start();
        });
        t.detach();

    }
}


void MainWindow::on_pushButton_2_clicked()
{
    dispatcher_running= false;

    for(auto proc : dispatcher.processes)
        kill(proc.pid, SIGTERM);
    dispatcher.semaphore_memory_close();
    Dispatcher::procesorteken=false;
    Dispatcher::processes.clear();
    Dispatcher::buffer.clear();
}

