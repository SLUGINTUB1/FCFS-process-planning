#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSharedPointer>
#include <string>

double key = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QTimer *timer = new QTimer;
    cpuPersent = new QLabel(this);
    cpuPersent->setGeometry(QRect(QPoint(415, 660),QSize(50, 20)));
    cpuPersent->setText("CPU");
    cpuPersent->setStyleSheet("QLabel { color : orange; }");

    cpu = new QLabel(this);
    cpu->setGeometry(QRect(QPoint(450, 660),QSize(50, 20)));

    memoryPersent = new QLabel(this);
    memoryPersent->setGeometry(QRect(QPoint(300, 660),QSize(60, 20)));
    memoryPersent->setText("Memory");
    memoryPersent->setStyleSheet("QLabel { color : violet; }");

    memory = new QLabel(this);
    memory->setGeometry(QRect(QPoint(370, 660),QSize(50, 20)));

    memoryTotal = new QLabel(this);
    memoryTotal->setGeometry(QRect(QPoint(405, 680),QSize(150, 20)));

    memoryTotalTitle = new QLabel(this);
    memoryTotalTitle->setGeometry(QRect(QPoint(300, 680),QSize(150, 20)));
    memoryTotalTitle->setText("Total Memory: ");

    memoryUsed = new QLabel(this);
    memoryUsed->setGeometry(QRect(QPoint(405, 700),QSize(150, 20)));

    memoryUsedTitle = new QLabel(this);
    memoryUsedTitle->setGeometry(QRect(QPoint(300, 700),QSize(150, 20)));
    memoryUsedTitle->setText("Memory used: ");

    customPlot = new QCustomPlot();

    ui->gridLayout_2->addWidget(customPlot,0,0,1,1);
    customPlot->addGraph();
    customPlot->graph(0)->setPen(QPen(QColor(235, 152, 52)));

    customPlot->addGraph();
    customPlot->graph(1)->setPen(QPen(QColor(180, 52, 235)));

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%m:%s");

    customPlot->xAxis->setTicker(timeTicker);
    customPlot->axisRect()->setupFullAxesBox();

    customPlot->yAxis->setRange(0, 100);
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    customPlot->xAxis->setRange(key, 4,  Qt::AlignRight);
    connect(timer,  SIGNAL(timeout()), this,   SLOT(realtimeDataSlot()));
    timer->start(10);
}

QString MainWindow::mesure(int n)
{
    QString e;
    int gb = (n / 1024) / 1024;
    int mb = (n-gb*1024*1024) /1024;
    int kb = (n - (gb*1024*1024+mb*1024));
    if (gb > 0)
        e = QString::number(gb) + QString(" Gb ");
    else
        e = QString("");
    if (mb > 0)
        e += QString::number(mb) + QString(" Mb ");
    if (kb > 0)
        e += QString::number(kb) + QString(" Kb ");
    return e;
}
int MainWindow::getMemoryLoad(QString &MemoryPresent, QString &MemoryUsed)
{
    //зчитування даних
    std::string mem;
    std::ifstream mem_file("/proc/meminfo");
    mem_file >> mem;
    mem_file >> mem;
    //визначення доступної пам’яті
    int num = atoi(mem.c_str());
    int percent = num / 100;
    MemoryPresent = mesure(num);
    //визначення зайнятої пам’яті.
    int free = 0;
    for (int i = 0 ; i < 3 ; i++) {
        mem_file >> mem; mem_file >> mem; mem_file >> mem;
        free += atoi(mem.c_str());
    }

    num -= free;
    MemoryUsed=mesure(abs(num));
    percent = num / percent;

    return abs(percent);
}

void MainWindow::realtimeDataSlot()
{
   // static QTime time(QTime::currentTime());

    /*
    // визначенню двох нових точок для вибірки даних
    double key = time.elapsed()/1000.0; // час, що пройшов з моменту запуску – перша точка
    static double lastPointKey = 0; //друга точка

   // std::cout<<"time"<<time<<std::endl;

    std::cout<<"key"<<key<<std::endl;

    if (key - lastPointKey > 0.002) // оновлення кожні 2 мс
    {
*/
        // виведення даних
        int cpu_persent=getCpuLoad(1);
        customPlot->graph(0)->addData(key,cpu_persent);
        cpu->setText(QString::number(cpu_persent)+'%');
        QString total,used;
        int memory_persent=getMemoryLoad(total,used);
        customPlot->graph(1)->addData(key, memory_persent);
        memory->setText(QString::number(memory_persent)+'%');
        memoryTotal->setText(total);
        memoryUsed->setText(used);
        key+=0.1;
        k++;
   // }
     if(k>4)//прокрутка графіку буде відбуватись після 4 секунд.
        customPlot->xAxis->setRange(key, 4,  Qt::AlignRight); //        (прокрутка) графіку з константою 4

    customPlot->replot();
}

//зсув
int MainWindow::getCpuLoad(double dt)
{
    std::vector<float> stats1 = readCpuStats();
    QProcess::execute("sleep",QStringList()<<QString::number(dt));
    std::vector<float> stats2 = readCpuStats();
    int size1 = stats1.size();
    int size2 = stats2.size();
    if (!size1 || !size2 || size1 != size2) return 2;
    for (int i = 0; i < size1; ++i) stats2[i] -= stats1[i];
    int sum = 1;
    for (int i = 0; i < size1; ++i) sum += stats2[i];
    int load = 100 - (stats2[size2 - 1] * 100 / sum);
    return load;
}
std::vector<float> MainWindow::readCpuStats()
{
    std::vector<float> ret;
    std::ifstream stat_file("/proc/stat");
    int val;
    std::string tmp;
    stat_file >> tmp;
    for (int i = 0; i < 4; ++i)
    {
        stat_file >> val;
        ret.push_back(val);
    }
    stat_file.close();
    return ret;
}

MainWindow::~MainWindow()
{
    delete ui;
}
