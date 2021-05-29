#ifndef GRIDGRAPHWINDOW_H
#define GRIDGRAPHWINDOW_H

#include <QWidget>
#include "matrix3dv.h"
#include "qgrid1d.h"
#include <QList>


class GridGraphWindow : public QWidget
{
    Q_OBJECT
public:
    explicit GridGraphWindow(QWidget *parent = nullptr);
public slots:
    void FillData(const QList<Matrix3DV>&);
    void ShowGraph();
signals:
    void SaveData();
};

#endif // GRIDGRAPHWINDOW_H
