#include "pqtview.h"



PQTView::PQTView(
        const AbstractLineInput *aliquidSource,
        const AbstractLineInput *apressureSource,
        const AbstractLineInput *aunitsController,
        const QHash<QString, QString>& amapTimeUnits,
        const QHash<QString, QString>& amapLiquidUnits,
        const QHash<QString, QString>& amapPressureUnits,
        const AbstractLineInput *avisibilityController,
        const QHash<QString, VisibilityState>& amapLiquidVisibility,
        const QHash<QString, VisibilityState>& amapPressureVisibility,
        QWidget *parent):
    QWidget(parent)
  , liquidSource(aliquidSource)
  , pressureSource(apressureSource)
  , unitsController(aunitsController)
  , mapTimeUnits(amapTimeUnits)
  , mapLiquidUnits(amapLiquidUnits)
  , mapPressureUnits(amapPressureUnits)
  , visibilityController(avisibilityController)
  , mapLiquidVisibility(amapLiquidVisibility)
  , mapPressureVisibility(amapPressureVisibility)
{
    vLayout = new QVBoxLayout(this);
    hLayout = new QHBoxLayout();
    vLayout->setContentsMargins(0,0,0,0);
    hLayout->setContentsMargins(0,0,0,0);

    view = new QTableView();
    vLayout->addWidget(view);
    model = new QStandardItemModel;
    model->setColumnCount(3);
    model->setRowCount(1);
    view->setModel(model);
    model->setHeaderData(0, Qt::Horizontal,"Time, " + GetCurrentUnitsByController(mapTimeUnits), Qt::DisplayRole);
    model->setHeaderData(1, Qt::Horizontal, "Pressure, " + GetCurrentUnitsByController(mapPressureUnits), Qt::DisplayRole);
    model->setHeaderData(2, Qt::Horizontal, "Liquid Rate, " + GetCurrentUnitsByController(mapLiquidUnits), Qt::DisplayRole);

    vLayout->addLayout(hLayout);
    buttonCalculate = new QPushButton("Calculate");
    hLayout->addWidget(buttonCalculate);
    buttonShow = new QPushButton("Show");
    hLayout->addWidget(buttonShow);
    buttonSave = new QPushButton("Save");
    hLayout->addWidget(buttonSave);

    if (unitsController)
        connect(unitsController, &AbstractLineInput::UnitsControllerChanged, this, &PQTView::onUnitsControllerChanged);
    if (visibilityController)
        connect(visibilityController, &AbstractLineInput::VisibilityControllerChanged, this, &PQTView::onVisibilityControllerChanged);
    SetCurrentColumnVisibility();

    connect(liquidSource, &AbstractLineInput::SendText, this, &PQTView::onLiquidSourceChanged);
    connect(pressureSource, &AbstractLineInput::SendText, this, &PQTView::onPressureSourceChanged);

    connect(model, &QStandardItemModel::itemChanged, this, &PQTView::onModelItemChanged);
    connect(buttonCalculate, &QPushButton::pressed, this, &PQTView::CalcButtonPressed);
    connect(buttonSave, &QPushButton::pressed, this, &PQTView::SaveButtonPressed);
    connect(buttonShow, &QPushButton::pressed, this, &PQTView::ShowButtonPressed);
}

std::vector<double> PQTView::GetTValues() const
{
    return GetColumnValues(0);
}

std::vector<double> PQTView::GetPValues() const
{
    return GetColumnValues(1);
}

std::vector<double> PQTView::GetQValues() const
{
    return GetColumnValues(2);
}

QString PQTView::GetCurrentUnitsByController(const QHash<QString, QString> &mapUnits) const
{
    if (unitsController && mapUnits.contains(unitsController->CurrentText()))
        return mapUnits.value(unitsController->CurrentText());
    return {};
}

void PQTView::SetCurrentColumnVisibility()
{
    if (visibilityController) {
        const QString& state = visibilityController->CurrentText();
        if (visibilityController && mapLiquidVisibility.contains(state))
            if (mapLiquidVisibility.value(state) == VisibilityState::Invisible)
                view->hideColumn(2);

        if (visibilityController && mapPressureVisibility.contains(state))
            if (mapPressureVisibility.value(state) == VisibilityState::Invisible)
                view->hideColumn(1);
    }
}

std::vector<double> PQTView::GetColumnValues(int column) const
{
    int rcount = model->rowCount()-1;
    std::vector<double> ans;
    ans.reserve(rcount);
    for (int i = 0; i < rcount; ++i) {
        auto index = model->index(i, column);
        ans.push_back((model->data(index)).toDouble());
    }
    return ans;
}

void PQTView::onUnitsControllerChanged(const QString &)
{
    model->setHeaderData(0, Qt::Horizontal, "Time, " + GetCurrentUnitsByController(mapTimeUnits), Qt::DisplayRole);
    model->setHeaderData(1, Qt::Horizontal, "Pressure, " + GetCurrentUnitsByController(mapPressureUnits), Qt::DisplayRole);
    model->setHeaderData(2, Qt::Horizontal, "Liquid Rate, " + GetCurrentUnitsByController(mapLiquidUnits), Qt::DisplayRole);
}

void PQTView::onVisibilityControllerChanged(const QString & state)
{
    if (visibilityController && mapLiquidVisibility.contains(state)) {
        if (mapLiquidVisibility.value(state) == VisibilityState::Visible) {
            view->showColumn(2);
        } else {
            view->hideColumn(2);
        }
    }
    if (visibilityController && mapPressureVisibility.contains(state)) {
        if (mapPressureVisibility.value(state) == VisibilityState::Visible) {
            view->showColumn(1);
        } else {
            view->hideColumn(1);
        }
    }
}

void PQTView::onPressureSourceChanged(const QString & value)
{
    for (int r = 0; r < model->rowCount() - 1; ++r) {
        QModelIndex itemIndex = model->index(r, 1);
        model->setData(itemIndex, value);
    }
}

void PQTView::onModelItemChanged(QStandardItem *item)
{
    int col = item->column();
    int row = item->row();
    if (col == 0 && row == model->rowCount()-1) {
        QModelIndex presIndex = model->index(row, 1);
        QModelIndex liqIndex = model->index(row, 2);
        model->setData(presIndex, pressureSource->CurrentText());
        model->setData(liqIndex, liquidSource->CurrentText());
        QList<QStandardItem*> row;
        for (int c = 0; c < model->columnCount(); ++c) {
            QStandardItem *item = new QStandardItem();
            row.append(item);
        }
        model->appendRow(row);
    }
    if (col == 0) {
        QModelIndex presIndex = model->index(row, 1);
        QModelIndex liqIndex = model->index(row, 2);
        model->setData(presIndex, pressureSource->CurrentText());
        model->setData(liqIndex, liquidSource->CurrentText());
    }
}

void PQTView::onLiquidSourceChanged(const QString & value)
{
    for (int r = 0; r < model->rowCount() - 1; ++r) {
        QModelIndex itemIndex = model->index(r, 2);
        model->setData(itemIndex, value);
    }
}


