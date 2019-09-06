#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QStringList list = QCoreApplication::libraryPaths();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
