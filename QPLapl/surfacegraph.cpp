#include "surfacegraph.h"

#include <QtDataVisualization/QValue3DAxis>
#include <QtDataVisualization/Q3DTheme>
#include <QtGui/QImage>
#include <QtCore/qmath.h>

using namespace QtDataVisualization;

SurfaceGraph::SurfaceGraph(Q3DSurface *surface, Matrix3DV *matr_ptr)
    : m_graph(surface)
{
    m_graph->setAxisX(new QValue3DAxis);
    m_graph->setAxisY(new QValue3DAxis);
    m_graph->setAxisZ(new QValue3DAxis);


    matrix3DVProxy = new QSurfaceDataProxy();
    matrix3DVSeries = new QSurface3DSeries(matrix3DVProxy);

    fillMatrixProxy(matr_ptr);

}

SurfaceGraph::~SurfaceGraph()
{
    delete m_graph;
}

void SurfaceGraph::fillMatrixProxy(Matrix3DV *matr_ptr) {
    auto xs = matr_ptr->GetAxis(MatrixAxis::X);
    auto ys = matr_ptr->GetAxis(MatrixAxis::Y);
    xMin = static_cast<float>(*std::min_element(begin(xs), end(xs)));
    xMax = static_cast<float>(*std::max_element(begin(xs), end(xs))); // to optimize
    yMin = static_cast<float>(*std::min_element(begin(ys), end(ys)));
    yMax = static_cast<float>(*std::max_element(begin(ys), end(ys)));
    valMin = static_cast<float>(matr_ptr->GetMinVal());
    valMax = static_cast<float>(matr_ptr->GetMaxVal());
    nX = static_cast<int>(xs.size());
    nY = static_cast<int>(ys.size());
    QSurfaceDataArray *dataArray = new QSurfaceDataArray;
    dataArray->reserve(nY);
    for (int i = 0; i < nY; ++i) {
        int index = 0;
        QSurfaceDataRow *newRow = new QSurfaceDataRow(nX);
        for (int j = 0; j < nX; ++j) {
            float x = static_cast<float>(matr_ptr->operator()(j,i,0).x);
            float y = static_cast<float>(matr_ptr->operator()(j,i,0).y);
            float val = static_cast<float>(matr_ptr->operator()(j,i,0).val);
            qDebug() << x << " " << y << " " << val;
            (*newRow)[index++].setPosition(QVector3D(x,val,y));
        }
        *dataArray << newRow;
    }
    matrix3DVProxy->resetArray(dataArray);
}

void SurfaceGraph::enableMatrixModel(bool enable) {
    if (enable) {

        matrix3DVSeries->setDrawMode(QSurface3DSeries::DrawSurface);
        matrix3DVSeries->setFlatShadingEnabled(true);
        m_graph->axisX()->setLabelFormat("%.2f");
        m_graph->axisZ()->setLabelFormat("%.2f");
        m_graph->axisX()->setRange(xMin, xMax);
        m_graph->axisY()->setRange(valMin, valMax);
        m_graph->axisZ()->setRange(yMin, yMax);
        m_graph->axisX()->setLabelAutoRotation(30);
        m_graph->axisY()->setLabelAutoRotation(90);
        m_graph->axisZ()->setLabelAutoRotation(30);
        m_graph->addSeries(matrix3DVSeries);
        // Reset range sliders
        m_rangeMinX = xMin;
        m_rangeMinZ = yMin;
        m_stepX = (xMax - xMin) / float(nX - 1);
        m_stepZ = (yMax - yMin) / float(nY - 1);
        m_axisMinSliderX->setMaximum(nX - 2);
        m_axisMinSliderX->setValue(0);
        m_axisMaxSliderX->setMaximum(nX - 1);
        m_axisMaxSliderX->setValue(nX - 1);
        m_axisMinSliderZ->setMaximum(nY - 2);
        m_axisMinSliderZ->setValue(0);
        m_axisMaxSliderZ->setMaximum(nY - 1);
        m_axisMaxSliderZ->setValue(nY - 1);
    }
}

void SurfaceGraph::adjustXMin(int min)
{
    float minX = m_stepX * float(min) + m_rangeMinX;

    int max = m_axisMaxSliderX->value();
    if (min >= max) {
        max = min + 1;
        m_axisMaxSliderX->setValue(max);
    }
    float maxX = m_stepX * max + m_rangeMinX;

    setAxisXRange(minX, maxX);
}

void SurfaceGraph::adjustXMax(int max)
{
    float maxX = m_stepX * float(max) + m_rangeMinX;

    int min = m_axisMinSliderX->value();
    if (max <= min) {
        min = max - 1;
        m_axisMinSliderX->setValue(min);
    }
    float minX = m_stepX * min + m_rangeMinX;

    setAxisXRange(minX, maxX);
}

void SurfaceGraph::adjustZMin(int min)
{
    float minZ = m_stepZ * float(min) + m_rangeMinZ;

    int max = m_axisMaxSliderZ->value();
    if (min >= max) {
        max = min + 1;
        m_axisMaxSliderZ->setValue(max);
    }
    float maxZ = m_stepZ * max + m_rangeMinZ;

    setAxisZRange(minZ, maxZ);
}

void SurfaceGraph::adjustZMax(int max)
{
    float maxX = m_stepZ * float(max) + m_rangeMinZ;

    int min = m_axisMinSliderZ->value();
    if (max <= min) {
        min = max - 1;
        m_axisMinSliderZ->setValue(min);
    }
    float minX = m_stepZ * min + m_rangeMinZ;

    setAxisZRange(minX, maxX);
}

//! [5]
void SurfaceGraph::setAxisXRange(float min, float max)
{
    m_graph->axisX()->setRange(min, max);
}

void SurfaceGraph::setAxisZRange(float min, float max)
{
    m_graph->axisZ()->setRange(min, max);
}

void SurfaceGraph::changeTheme(int theme)
{
    qDebug() << "SurfaceGraph::changeTheme called";
    m_graph->activeTheme()->setType(Q3DTheme::Theme(theme));
    qDebug() << "SurfaceGraph::changeTheme OK";
}

void SurfaceGraph::setGreenToRedGradient()
{
    QLinearGradient gr;
    gr.setColorAt(0.0, Qt::darkGreen);
    gr.setColorAt(0.5, Qt::yellow);
    gr.setColorAt(0.8, Qt::red);
    gr.setColorAt(1.0, Qt::darkRed);

    m_graph->seriesList().at(0)->setBaseGradient(gr);
    m_graph->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
}


