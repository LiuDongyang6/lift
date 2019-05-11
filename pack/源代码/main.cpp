#include "mainwindow.h"
#include"dispatcher.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
	w.setFixedSize(QSize(1400, 800));
    w.show();
	dispatcher::getInstance()->init();
    return a.exec();
}
