#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

// TODO:
// align inputs
// set int checkers for grid inputs
// catch void inputs
// add grid visibility checkbox
// add timesteps to grid graph
