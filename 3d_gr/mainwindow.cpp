#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <string>

double key = 0;
QList<float> list;
float lastY=0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create the 3D surface graph
    surfaceGraph = new Q3DSurface();

    // Create a container for the graph
    container = QWidget::createWindowContainer(surfaceGraph);
    container->setMinimumSize(QSize(400, 300));
    //  container->setMaximumSize(QSize(800, 600));
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy(Qt::StrongFocus);

    // Set a layout for the central widget if it doesn't already have one
    if (!ui->widget->layout()) {
        QVBoxLayout *layout = new QVBoxLayout(ui->widget);
        ui->widget->setLayout(layout);
    }

    // Add the container to the central widget's layout
    ui->widget->layout()->addWidget(container);



    // QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    // timeTicker->setTimeFormat("%m:%s");//формат часу

    // customPlot->xAxis->setTicker(timeTicker);//додавання по вісі абсцис часу
    QString total,used;

    lastY = (float) getMemoryLoad(total,used);
    QTimer *timer = new QTimer;

    connect(timer,  SIGNAL(timeout()), this,   SLOT(realtimeDataSlot()));
    timer->start(100); //Запуск таймера з встановленим інтервалом 0
}



void MainWindow::realtimeDataSlot()
{
    // Data proxy and series
    QSurfaceDataProxy *proxy = new QSurfaceDataProxy();
    QSurface3DSeries *series = new QSurface3DSeries(proxy);

    QString total,used;
    // Fill the data
    QSurfaceDataArray *dataArray = new QSurfaceDataArray();
    dataArray->reserve(50);
    for (int i = 0; i < 5; i+=1) {
        std::cout<<list.size()<<std::endl;
        QSurfaceDataRow *newRow = new QSurfaceDataRow(50);
        for (int j = 0; j < 50; j++) {

            float y = (float) lastY + (getMemoryLoad(total,used)- lastY)/5*i;
            float x = (float) (key+i);
            float z = (float) j;

            (*newRow)[j].setPosition(QVector3D(x, y, z));

            lastY = y;
            /*
            float x = (float)i / 10.0f;
            float y = (float)qSin(qSqrt(x*x + j*j));
            float z = (float)j / 10.0f;
            (*newRow)[j].setPosition(QVector3D(x, y, z));
             */

        }
        *dataArray << newRow;
        key+=0.1;
    }


    std::cout<<"hi"<<std::endl;


    proxy->resetArray(dataArray);

    surfaceGraph->addSeries(series);
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
int
MainWindow::getMemoryLoad(QString &MemoryPresent, QString &MemoryUsed)
{
    //зчитування даних
    std::string mem;
    std::ifstream mem_file("/proc/meminfo");
    mem_file >> mem;
    mem_file >> mem;
    //визначення доступної пам’яті
    int num = atoi(mem.c_str());
    int percent = num / 100;
    MemoryPresent=mesure(num);
    //визначення зайнятої пам’яті.
    int free = 0;
    for (int i = 0 ; i < 3 ; i++) {
        mem_file >> mem; mem_file >> mem; mem_file >> mem;
        free += atoi(mem.c_str());
    }

    num -= free;
    MemoryUsed=mesure(num);
    percent = num / percent;

    return percent;
}


//зсув
int MainWindow::getCpuLoad(double dt)
{
    std::vector<float> stats1 = readCpuStats();
    // QProcess::execute("sleep",QStringList()<<QString::number(dt));
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
