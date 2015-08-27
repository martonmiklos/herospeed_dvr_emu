#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("MM");
    QCoreApplication::setApplicationName("Herospeed DVR emulator");
    QCoreApplication::setApplicationVersion("0.1");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
