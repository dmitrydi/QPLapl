#include "pqgraphwindow.h"
#include "ui_pqgraphwindow.h"

#include <QDebug>

using namespace QtCharts;

PQGraphWindow::PQGraphWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PQGraphWindow),
    series(new QLineSeries()),
    chart(new QChart()),
    chartView(new QChartView(chart)),
    xLinAxis(new QValueAxis(this)),
    yLinAxis(new QValueAxis(this)),
    xLogAxis(new QLogValueAxis(this)),
    yLogAxis(new QLogValueAxis(this))
{
    ui->setupUi(this);
    this->setWindowTitle("Pressure");
    this->setWindowModality(Qt::WindowModality::NonModal);
    CreateChart();
    connect(this, &PQGraphWindow::DataFilled, this, &PQGraphWindow::setXLinAxis);
    connect(this, &PQGraphWindow::DataFilled, this, &PQGraphWindow::setYLinAxis);
    connect(this, &PQGraphWindow::DataFilled, this, &PQGraphWindow::setXLogAxis);
    connect(this, &PQGraphWindow::DataFilled, this, &PQGraphWindow::setYLogAxis);
    connect(ui->checkBoxXLog, &QCheckBox::stateChanged, this, &PQGraphWindow::onXStateChanged);
    connect(ui->checkBoxYLog, &QCheckBox::stateChanged, this, &PQGraphWindow::onYStateChanged);
    connect(this, &PQGraphWindow::AxisUpdated, this, &PQGraphWindow::UpdateChart);
    onXStateChanged(Qt::Unchecked);
    onYStateChanged(Qt::Unchecked);
}

void PQGraphWindow::UpdateChart() {
    chartView->update();
}

void PQGraphWindow::setXLinAxis(QtCharts::QLineSeries* series) {
    xLinAxis->setTitleText("Time");
    xLinAxis->setLabelFormat("%g");
    xLinAxis->setTickCount(series->count());
    xLinAxis->setMax(series->at(series->count()-1).x());
    emit AxisUpdated();
}
void PQGraphWindow::setYLinAxis(QtCharts::QLineSeries*) {
    yLinAxis->setTitleText("Pressure");
    yLinAxis->setLabelFormat("%g");
    yLinAxis->setTickCount(series->count());
    yLinAxis->setMax(series->at(0).y()*1.1);
    emit AxisUpdated();
}
void PQGraphWindow::setXLogAxis(QtCharts::QLineSeries*) {
    xLogAxis->setTitleText("Values");
    xLogAxis->setLabelFormat("%g");
    xLogAxis->setBase(10.0);
    xLogAxis->setMinorTickCount(-1);
    xLogAxis->setMax(series->at(series->count()-1).x());
    emit AxisUpdated();
}
void PQGraphWindow::setYLogAxis(QtCharts::QLineSeries*) {
    yLogAxis->setTitleText("Values");
    yLogAxis->setLabelFormat("%g");
    yLogAxis->setBase(10.0);
    yLogAxis->setMinorTickCount(-1);
    yLogAxis->setMax(series->at(0).y()*1.1);
    emit AxisUpdated();
}
void PQGraphWindow::onXStateChanged(int state) {
    QAbstractAxis *removeaxis, *insertaxis;
    if(state == Qt::Checked){
        removeaxis = xLinAxis;
        insertaxis = xLogAxis;;
    }
    else{
        removeaxis = xLogAxis;
        insertaxis = xLinAxis;
    }
    if(chart->axes(Qt::Horizontal).contains(removeaxis))
        chart->removeAxis(removeaxis);
    chart->addAxis(insertaxis, Qt::AlignBottom);

    for(auto serie: chart->series()){
        if(serie->attachedAxes().contains(removeaxis))
            serie->detachAxis(removeaxis);
        serie->attachAxis(insertaxis);
    }
}
void PQGraphWindow::onYStateChanged(int state) {
    QAbstractAxis *removeaxis, *insertaxis;
    if(state == Qt::Checked){
        removeaxis = yLinAxis;
        insertaxis = yLogAxis;;
    }
    else{
        removeaxis = yLogAxis;
        insertaxis = yLinAxis;
    }
    if(chart->axes(Qt::Vertical).contains(removeaxis))
        chart->removeAxis(removeaxis);
    chart->addAxis(insertaxis, Qt::AlignLeft);

    for(auto serie: chart->series()){
        if(serie->attachedAxes().contains(removeaxis))
            serie->detachAxis(removeaxis);
        serie->attachAxis(insertaxis);
    }
}

void PQGraphWindow::FillData(const QVector<DataPoint>& datapoints) {
    series->clear();
    for (const auto& dp: datapoints) {
        series->append({dp.first, dp.second});
    }
    qDebug() << "Series filled with " << series->count();
    emit DataFilled(series);
}

void PQGraphWindow::CreateChart() {
    chart->addSeries(series);
    chart->legend()->hide();
    chart->setTitle("Logarithmic axis example");
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->verticalLayout->addWidget(chartView, 1);
}

void PQGraphWindow::ShowGraph() {
    show();
}

PQGraphWindow::~PQGraphWindow()
{
    delete ui;
}
