#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication qa(argc, argv);
    MainWindow w(Aksonometric);
    w.show();
    return qa.exec();
}
