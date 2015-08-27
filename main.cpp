#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("%ORGANIZATION%");
    QCoreApplication::setApplicationName("%APPNAME%");
    QCoreApplication::setApplicationVersion("%APPVERSION%");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
