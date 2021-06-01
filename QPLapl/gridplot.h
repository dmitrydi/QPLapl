#ifndef GRIDPLOT_H
#define GRIDPLOT_H

#include <QWidget>
#include <QDialog>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QtGui/QPainter>
#include <QtGui/QScreen>
#include <QComboBox>
#include "surfacegraph.h"
#include "matrix3dv.h"

class GridPlot : public QDialog
{
    Q_OBJECT
public:
    explicit GridPlot(QWidget *parent = nullptr);
public slots:
    void FillData(const QList<Matrix3DV>&);
    void ShowGraph();
    void SaveData();
signals:

private:
    Q3DSurface *graph;
    QWidget *container;
    QWidget *widget;
    QHBoxLayout *hLayout;
    QVBoxLayout *vLayout;
    QGroupBox *selectionGroupBox;
    QRadioButton *modeNoneRB;
    QRadioButton *modeItemRB;
    QRadioButton *modeSliceRowRB;
    QRadioButton *modeSliceColumnRB;
    QVBoxLayout *selectionVBox;
    QSlider *axisMinSliderX;
    QSlider *axisMaxSliderX;
    QSlider *axisMinSliderZ;
    QSlider *axisMaxSliderZ;
    Matrix3DV grid;
    SurfaceGraph *modifier;
    void SetContainerProps();
    void SetRbs();
    void SetSliders();
    void SetTimeBox();
    QComboBox *timeBox;
};

#endif // GRIDPLOT_H
