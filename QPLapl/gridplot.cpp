#include "gridplot.h"
#include <QDebug>

GridPlot::GridPlot(QWidget *parent) :
        QDialog(parent)
      , graph(new Q3DSurface())
      , container(QWidget::createWindowContainer(graph))
      , widget(new QWidget)
      , hLayout(new QHBoxLayout(widget))
      , vLayout(new QVBoxLayout())
      , selectionGroupBox(new QGroupBox(QStringLiteral("Selection Mode")))
      , modeNoneRB(new QRadioButton(widget))
      , modeItemRB(new QRadioButton(widget))
      , modeSliceRowRB(new QRadioButton(widget))
      , modeSliceColumnRB(new QRadioButton(widget))
      , selectionVBox(new QVBoxLayout)
      , axisMinSliderX(new QSlider(Qt::Horizontal, widget))
      , axisMaxSliderX(new QSlider(Qt::Horizontal, widget))
      , axisMinSliderZ(new QSlider(Qt::Horizontal, widget))
      , axisMaxSliderZ(new QSlider(Qt::Horizontal, widget))
      //, modifier(new SurfaceGraph(graph, &grid))
{
    SetContainerProps();
    hLayout->addWidget(container, 1);
    hLayout->addLayout(vLayout);
    vLayout->setAlignment(Qt::AlignTop);
    SetRbs();
    SetSliders();
    this->setLayout(hLayout);
}

void GridPlot::FillData(const QList<Matrix3DV> & ListOfMatrices)
{
    if (modifier) delete modifier;
    modifier = new SurfaceGraph(graph, &ListOfMatrices[0]);
    QObject::connect(modeNoneRB, &QRadioButton::toggled,
                     modifier, &SurfaceGraph::toggleModeNone);
    QObject::connect(modeItemRB,  &QRadioButton::toggled,
                     modifier, &SurfaceGraph::toggleModeItem);
    QObject::connect(modeSliceRowRB,  &QRadioButton::toggled,
                     modifier, &SurfaceGraph::toggleModeSliceRow);
    QObject::connect(modeSliceColumnRB,  &QRadioButton::toggled,
                     modifier, &SurfaceGraph::toggleModeSliceColumn);
    QObject::connect(axisMinSliderX, &QSlider::valueChanged,
                     modifier, &SurfaceGraph::adjustXMin);
    QObject::connect(axisMaxSliderX, &QSlider::valueChanged,
                     modifier, &SurfaceGraph::adjustXMax);
    QObject::connect(axisMinSliderZ, &QSlider::valueChanged,
                     modifier, &SurfaceGraph::adjustZMin);
    QObject::connect(axisMaxSliderZ, &QSlider::valueChanged,
                     modifier, &SurfaceGraph::adjustZMax);

    modifier->setAxisMinSliderX(axisMinSliderX);
    modifier->setAxisMaxSliderX(axisMaxSliderX);
    modifier->setAxisMinSliderZ(axisMinSliderZ);
    modifier->setAxisMaxSliderZ(axisMaxSliderZ);
    modifier->enableMatrixModel(true);
    modifier->setGreenToRedGradient();
}

void GridPlot::ShowGraph()
{
    qDebug() << "GridPlot::ShowGraph() called";
    this->show();
}

void GridPlot::SaveData()
{

}

void GridPlot::SetContainerProps() {
    QSize screenSize = graph->screen()->size();
    container->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height() / 1.6));
    container->setMaximumSize(screenSize);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy(Qt::StrongFocus);
}

void GridPlot::SetRbs() {
    modeNoneRB->setText(QStringLiteral("No selection"));
    modeNoneRB->setChecked(false);
    modeItemRB->setText(QStringLiteral("Item"));
    modeItemRB->setChecked(false);
    modeSliceRowRB->setText(QStringLiteral("Row Slice"));
    modeSliceRowRB->setChecked(false);
    modeSliceColumnRB->setText(QStringLiteral("Column Slice"));
    modeSliceColumnRB->setChecked(false);
    selectionVBox->addWidget(modeNoneRB);
    selectionVBox->addWidget(modeItemRB);
    selectionVBox->addWidget(modeSliceRowRB);
    selectionVBox->addWidget(modeSliceColumnRB);
    selectionGroupBox->setLayout(selectionVBox);
    vLayout->addWidget(selectionGroupBox);
}

void GridPlot::SetSliders() {
    axisMinSliderX->setMinimum(0);
    axisMinSliderX->setTickInterval(1);
    axisMinSliderX->setEnabled(true);
    axisMaxSliderX->setMinimum(1);
    axisMaxSliderX->setTickInterval(1);
    axisMaxSliderX->setEnabled(true);
    axisMinSliderZ->setMinimum(0);
    axisMinSliderZ->setTickInterval(1);
    axisMinSliderZ->setEnabled(true);
    axisMaxSliderZ->setMinimum(1);
    axisMaxSliderZ->setTickInterval(1);
    axisMaxSliderZ->setEnabled(true);
    vLayout->addWidget(new QLabel(QStringLiteral("Column range")));
    vLayout->addWidget(axisMinSliderX);
    vLayout->addWidget(axisMaxSliderX);
    vLayout->addWidget(new QLabel(QStringLiteral("Row range")));
    vLayout->addWidget(axisMinSliderZ);
    vLayout->addWidget(axisMaxSliderZ);
}
