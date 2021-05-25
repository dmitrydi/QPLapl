#ifndef PQGRAPHWINDOW_H
#define PQGRAPHWINDOW_H

#include <QDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QLogValueAxis>
#include <QtCharts/QValueAxis>
#include "wellmanager.h"

namespace Ui {
class PQGraphWindow;
}

class PQGraphWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PQGraphWindow(QWidget *parent = nullptr);
    ~PQGraphWindow();
public slots:
    void FillData(const QVector<DataPoint>&);
    void ShowGraph();
private:
    Ui::PQGraphWindow *ui;
    QtCharts::QLineSeries *series;
    QtCharts::QChart *chart;
    QtCharts::QChartView *chartView;
    QtCharts::QValueAxis *xLinAxis, *yLinAxis;
    QtCharts::QLogValueAxis *xLogAxis, *yLogAxis;
    void CreateChart();

signals:
    void DataFilled(QtCharts::QLineSeries*);
    void AxisUpdated();
    void SaveData();
private slots:
    void setXLinAxis(QtCharts::QLineSeries*);
    void setYLinAxis(QtCharts::QLineSeries*);
    void setXLogAxis(QtCharts::QLineSeries*);
    void setYLogAxis(QtCharts::QLineSeries*);
    void onXStateChanged(int);
    void onYStateChanged(int);
    void UpdateChart();
};

#endif // PQGRAPHWINDOW_H
