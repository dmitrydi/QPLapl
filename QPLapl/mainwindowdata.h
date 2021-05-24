#ifndef MAINWINDOWDATA_H
#define MAINWINDOWDATA_H

#include <QMainWindow>
#include <QObject>
#include "gwell.h"
#include "labelmapper.h"
#include <vector>


class MainWindowData: public QObject
{
    Q_OBJECT
public:
    explicit MainWindowData(QMainWindow *mainwindow, QObject *parent = nullptr);
    void fetchDataFromWindow();
    double xed() const;
    double xwd() const;
private:
    QMainWindow *mainwindow;
    WellType g_welltype;
    Boundary g_boundary;
    DrainageArea g_drainage_area;
    UNITSYSTEM unitsystem;
    double xe, xw, ye, yw, zw, re, rw, Fcd, xf, lh;
    double _xed, _xwd, _yed, _ywd, _zwd, _red, _rwd, _ld, _hd;
    int nfrac;
    std::vector<double> tds;
    void makeDimentionlessParams();

};

#endif // MAINWINDOWDATA_H
