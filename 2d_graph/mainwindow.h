#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h" //для малювання графіка завантаженості
#include <QtGui> //для підтримки GUI елементів (QLabel тощо)
#include <iostream> //введення/виведення
#include <fstream>
#include <unistd.h>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QCustomPlot *customPlot; //полотно для малювання
    QLabel *cpu; // відображення завантаженості процесора
    QLabel *cpuPersent;// підпис: CPU
    QLabel *memory; // відображення завантаженості пам’яті
    QLabel *memoryPersent; // підпис: Memory
    QLabel *memoryTotal; // відображення доступної пам’яті
    QLabel *memoryUsed; // відображення використаної пам’яті
    QLabel *memoryTotalTitle; // підпис: всього пам’яті
    QLabel *memoryUsedTitle; // підпис: використано пам’яті
    std::vector<float> readCpuStats(); //функція зчитування даних про завантаженість процесора
    int getCpuLoad(double dt); //функція визначення завантаженості процесора
    int getMemoryLoad(QString &MemoryPresent, QString &MemoryUsed);

    //функція визначення завантаженості пам’яті
    QString mesure(int n);//функція формування вигляду Gb-Mb-Kb для завантаженості пам’яті
    int k=0;//змінна-лічильник для підрахунку кількості секунд, що пройшли від моменту запуску програми

private slots:
    void realtimeDataSlot();//слот, який буде викликатись таймером кожного разу
};
#endif // MAINWINDOW_H
