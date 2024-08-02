#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDataVisualization>
#include <iostream> //введення/виведення
#include <fstream>
#include <unistd.h>
#include <vector>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Q3DSurface *surfaceGraph;
    QWidget *container;
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
