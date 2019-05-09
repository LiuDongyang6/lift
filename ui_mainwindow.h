/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLabel *label;
    QPushButton *up;
    QPushButton *down;
    QPushButton *close;
    QPushButton *open;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1800, 1000);
        MainWindow->setAutoFillBackground(false);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(310, 290, 21, 20));
        up = new QPushButton(centralWidget);
        up->setObjectName(QStringLiteral("up"));
        up->setGeometry(QRect(330, 290, 20, 20));
        QIcon icon;
        icon.addFile(QString::fromUtf8("C:/Users/39270/Desktop/\346\226\260\345\273\272\346\226\207\344\273\266\345\244\271/up.ico"), QSize(), QIcon::Normal, QIcon::Off);
        up->setIcon(icon);
        up->setCheckable(true);
        up->setChecked(false);
        up->setAutoDefault(false);
        up->setFlat(true);
        down = new QPushButton(centralWidget);
        down->setObjectName(QStringLiteral("down"));
        down->setGeometry(QRect(350, 290, 20, 20));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8("C:/Users/39270/Desktop/\346\226\260\345\273\272\346\226\207\344\273\266\345\244\271/down.ico"), QSize(), QIcon::Normal, QIcon::Off);
        down->setIcon(icon1);
        down->setCheckable(true);
        down->setFlat(true);
        close = new QPushButton(centralWidget);
        close->setObjectName(QStringLiteral("close"));
        close->setGeometry(QRect(340, 560, 101, 51));
        QIcon icon2;
        icon2.addFile(QStringLiteral("close.png"), QSize(), QIcon::Normal, QIcon::Off);
        close->setIcon(icon2);
        close->setIconSize(QSize(40, 40));
        close->setFlat(true);
        open = new QPushButton(centralWidget);
        open->setObjectName(QStringLiteral("open"));
        open->setGeometry(QRect(350, 530, 93, 28));
        QIcon icon3;
        icon3.addFile(QStringLiteral("open.png"), QSize(), QIcon::Normal, QIcon::Off);
        open->setIcon(icon3);
        open->setIconSize(QSize(40, 40));
        open->setAutoDefault(false);
        open->setFlat(true);
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QApplication::translate("MainWindow", "F3", nullptr));
        up->setText(QString());
        down->setText(QString());
        close->setText(QString());
        open->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
