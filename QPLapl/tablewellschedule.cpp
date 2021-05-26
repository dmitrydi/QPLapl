#include "tablewellschedule.h"

TableWellSchedule::TableWellSchedule(UniComboBox *unitsSrc
                                    , UniComboBox *calcModeSrc
                                    , QLineEdit *liqRateSrc
                                    , QLineEdit *wellPresSrc
                                    , QWidget *parent):
    QWidget(parent)
    , unitsSource(unitsSrc)
    , calcModeSource(calcModeSrc)
    , liquidRateSource(liqRateSrc)
    , wellPressureSource(wellPresSrc)
{
    SetTable();
    SetLayouts();
    SetButtons();
}

void TableWellSchedule::SetTable() {
    tableModel = new QStandardItemModel(this);
    tableModel->setRowCount(tableScheduleModeInitRows);
    tableModel->setColumnCount(tableScheduleModelColumns);
    tableView = new QTableView(this);
    tableView->setModel(tableModel);
    for(int c = 1; c < tableView->model()->columnCount(); c++)
            tableView->setItemDelegateForColumn(c, new NotEditableDelegate(tableView));
    SetTableHeaders(unitsSource->currentText());
    connect(unitsSource, &UniComboBox::textChanged, this, &TableWellSchedule::SetTableHeaders);
    SetTableColumnsVisibility(calcModeSource->currentText());
    connect(calcModeSource, &UniComboBox::textChanged, this, &TableWellSchedule::SetTableColumnsVisibility);
    connect(tableModel, &QStandardItemModel::itemChanged, this, &TableWellSchedule::MaybeAddRow);
    connect(liquidRateSource, &QLineEdit::textChanged, [this](const QString& liquid){
        this->UpdateTableValues(liquid, wellPressureSource->text());
    });
    connect(wellPressureSource, &QLineEdit::textChanged, [this](const QString& pressure){
        this->UpdateTableValues(liquidRateSource->text(), pressure);
    });
}

void TableWellSchedule::SetLayouts() {
    vLayout = new QVBoxLayout(this);
    hLayout = new QHBoxLayout();
    vLayout->addWidget(tableView);
    vLayout->addLayout(hLayout);
}

void TableWellSchedule::SetButtons() {
    buttonCalculate = new QPushButton();
    buttonSave = new QPushButton();
    buttonShow = new QPushButton();
    buttonCalculate->setText("Calculate");
    buttonSave->setText("Save");
    buttonShow->setText("Show");
    hLayout->addWidget(buttonCalculate);
    hLayout->addWidget(buttonShow);
    hLayout->addWidget(buttonSave);
    connect(buttonCalculate, &QPushButton::released, this, &TableWellSchedule::ButtonCalculatePressed);
    connect(buttonShow, &QPushButton::released, this, &TableWellSchedule::ButtonShowPressed);
    connect(buttonSave, &QPushButton::released, this, &TableWellSchedule::ButtonSavePressed);
}

void TableWellSchedule::SetTableHeaders(const QString& units) {
    tableModel->setHeaderData(0, Qt::Horizontal, "Time, " + unitsMapper
                                      .value(mapStrToUnitSystem.value(units))
                                      .value(PARAMS::TIME), Qt::EditRole);
    tableModel->setHeaderData(1, Qt::Horizontal, "Well rate, " + unitsMapper
                                      .value(mapStrToUnitSystem.value(units))
                                      .value(PARAMS::LIQRATE), Qt::DisplayRole);
    tableModel->setHeaderData(2, Qt::Horizontal, "Wellbore pressure, " + unitsMapper
                                      .value(mapStrToUnitSystem.value(units))
                                      .value(PARAMS::WELLPRES), Qt::DisplayRole);
}

void TableWellSchedule::SetTableColumnsVisibility(const QString& calcmode) {
    tableView->hideColumn(1);
    tableView->hideColumn(2);
    tableView->showColumn(strToTableScheduleColumn.value(calcmode));
}

void TableWellSchedule::MaybeAddRow(QStandardItem* item) {
    int col = item->column();
    int row = item->row();
    if (col == 0 && (row == (tableModel->rowCount() - 1))) {
        QModelIndex liqIndex = tableModel->index(row, 1);
        QModelIndex presIndex = tableModel->index(row, 2);
        tableModel->setData(liqIndex,liquidRateSource->text());
        tableModel->setData(presIndex, wellPressureSource->text());
        QList<QStandardItem*> row;
        for (int c = 0; c < tableModel->columnCount(); ++c) {
            QStandardItem *item = new QStandardItem();
            row.append(item);
        }
        tableModel->appendRow(row);
    }
}

void TableWellSchedule::UpdateTableValues(const QString& liquidVal, const QString& pressureVal) {
    const QString calcMode = calcModeSource->currentText();
    if (calcMode == "Constant Liquid Rate") {
        for (int row = 0; row < tableModel->rowCount()-1; ++row) {
            QModelIndex itemIndex = tableModel->index(row, strToTableScheduleColumn.value(calcMode));
            tableModel->setData(itemIndex, liquidVal);
        }
    } else if (calcMode == "Constant Wellbore Pressure") {
        for (int row = 0; row < tableModel->rowCount()-1; ++row) {
            QModelIndex itemIndex = tableModel->index(row, strToTableScheduleColumn.value(calcMode));
            tableModel->setData(itemIndex, pressureVal);
        }
    }
}

QStandardItemModel* TableWellSchedule::getModel() const {
    return tableModel;
}

