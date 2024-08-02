/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTableWidget *tableWidget;
    QSpinBox *spinBox;
    QSpinBox *spinBox_2;
    QSpinBox *spinBox_3;
    QSpinBox *spinBox_4;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        tableWidget = new QTableWidget(centralwidget);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setGeometry(QRect(370, 10, 411, 531));
        spinBox = new QSpinBox(centralwidget);
        spinBox->setObjectName("spinBox");
        spinBox->setGeometry(QRect(10, 30, 44, 26));
        spinBox->setValue(4);
        spinBox_2 = new QSpinBox(centralwidget);
        spinBox_2->setObjectName("spinBox_2");
        spinBox_2->setGeometry(QRect(180, 30, 44, 26));
        spinBox_2->setValue(2);
        spinBox_3 = new QSpinBox(centralwidget);
        spinBox_3->setObjectName("spinBox_3");
        spinBox_3->setGeometry(QRect(10, 80, 44, 26));
        spinBox_3->setValue(2);
        spinBox_4 = new QSpinBox(centralwidget);
        spinBox_4->setObjectName("spinBox_4");
        spinBox_4->setGeometry(QRect(180, 80, 44, 26));
        spinBox_4->setValue(99);
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(10, 60, 141, 17));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(10, 10, 171, 17));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(180, 10, 171, 17));
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(180, 60, 181, 17));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(10, 120, 121, 41));
        pushButton_2 = new QPushButton(centralwidget);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(180, 120, 121, 41));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "K \320\272\321\226\320\273\321\214\320\272\321\226\321\201\321\202\321\214 \320\277\320\265\321\200\320\265\320\264\320\260\321\207", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "N \320\272\321\226\320\273\321\214\320\272\321\226\321\201\321\202\321\214 \320\263\320\265\320\275\320\265\321\200\320\260\321\202\320\276\321\200\321\226\320\262", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", " M \320\272\321\226\320\273\321\214\320\272\321\226\321\201\321\202\321\214 \320\262\320\270\320\272\320\276\320\275\320\260\320\262\321\206\321\226\320\262", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "X \320\274\321\226\320\273\321\226\321\201\320\265\320\272\321\203\320\275\320\264 \320\276\321\207\321\226\320\272\321\203\320\262\320\260\320\275\320\275\321\217", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "START", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "END", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
