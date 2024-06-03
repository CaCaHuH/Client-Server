#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle("Fusion");
    MainWindow w;
    w.setWindowTitle("Client");
    w.create();
    w.show();
    return a.exec();
}
