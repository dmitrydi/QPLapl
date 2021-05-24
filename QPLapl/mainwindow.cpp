#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , graphWin(new PQGraphWindow(this))
{
    ui->setupUi(this);
    wellManager = new WellManager(this);
    ui->tabWidget->setCurrentIndex(0);
    setupUComboUnits();
    setupUComboWellType();
    setupUComboAeaShape();
    setupUComboBoundaryConditions();
    setupUComboCalcMode();
    setupPayUnitsLabels();
    setupWellParamsLabels();
    setupDrainageAreaLabels();
    setupPicMan();
    setupCalcModeLabels();
    setupTableSchedule();

    connect(ui->pushPQ, &QPushButton::released, this, &MainWindow::runWellManagerPQ);
    connect(this, &MainWindow::PQDataCalculated, graphWin, &PQGraphWindow::FillData);
    connect(ui->pushShowGraph, &QPushButton::released, graphWin, &PQGraphWindow::ShowGraph);
}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::runWellManagerPQ() {
    setWellManagerData();
    PQTData = wellManager->PQCalc();
    emit PQDataCalculated(PQTData);
}

//---------------------------------------
void MainWindow::setupUComboUnits() {
    uComboUnits = new UniComboBox(this);
    uComboUnits->addItems(unitSystemsList);
    ui->gridLayoutUnits->setAlignment(Qt::AlignTop);
    ui->gridLayoutUnits->addWidget(uComboUnits);
}

void MainWindow::setupUComboWellType() {
    uComboWellType = new UniComboBox(this);
    uComboWellType->addItems(wellTypeList);
    ui->gridLayoutWellProps->addWidget(uComboWellType
                                       , uComboWellTypeIndex.row
                                       , uComboWellTypeIndex.col);
}
void MainWindow::setupUComboAeaShape() {
    uComboAreaShape = new UniComboBox(this);
    uComboAreaShape->addItems(areaShapeList);
    ui->gridLayoutDarinageProps->addWidget(uComboAreaShape
                                           , uComboAreaShapeIndex.row
                                           , uComboAreaShapeIndex.col);
}
void MainWindow::setupUComboBoundaryConditions() {
    uComboBoundaryConditions = new UniComboBox(this);
    uComboBoundaryConditions->addItems(boundaryConditionsList);
    ui->gridLayoutDarinageProps->addWidget(uComboBoundaryConditions
                                           , uComboBoundaryCondIndex.row
                                           , uComboBoundaryCondIndex.col);
}

void MainWindow::setupUComboCalcMode() {
    uComboCalcMode = new UniComboBox(this);
    uComboCalcMode->addItems(calcModeList);
    ui->gridLayoutCalcMode->addWidget(uComboCalcMode
                                      , uComboCalcModeIndex.row
                                      , uComboCalcModeIndex.col);
}

void MainWindow::_setupPayUnits(const QString& units) {
    ui->labelPermUnits->setText(unitsMapper
                                .value(mapStrToUnitSystem.value(units))
                                .value(PARAMS::K));
    ui->labelPayZoneUnits->setText(unitsMapper
                                   .value(mapStrToUnitSystem.value(units))
                                   .value(PARAMS::H));
    ui->labelPoroUnits->setText(unitsMapper
                                .value(mapStrToUnitSystem.value(units))
                                .value(PARAMS::PHI));
    ui->labelViscUnits->setText(unitsMapper
                                .value(mapStrToUnitSystem.value(units))
                                .value(PARAMS::MU));
    ui->labelCtUnits->setText(unitsMapper
                              .value(mapStrToUnitSystem.value(units))
                              .value(PARAMS::CT));
    ui->labelFVFUnits->setText(unitsMapper
                               .value(mapStrToUnitSystem.value(units))
                               .value(PARAMS::BOIL));
    ui->labelPinitUnits->setText(unitsMapper
                                 .value(mapStrToUnitSystem.value(units))
                                 .value(PARAMS::PINIT));
}

void MainWindow::setupPayUnitsLabels() {
    _setupPayUnits(uComboUnits->text());
    connect(uComboUnits, &UniComboBox::textChanged,
            this, &MainWindow::_setupPayUnits);
}

void MainWindow::_setupWellParamsVisibility(const QString& wellType) {
    if (wellType == "Fracture") {
        _setXfLineVisible(true);
        _setFcdLineVisible(true);
        _setLhLineVisible(false);
        _setNumFracLineVisible(false);
        _setRwLineVisible(false);
    } else if (wellType == "Multifractured") {
        _setXfLineVisible(true);
        _setFcdLineVisible(true);
        _setLhLineVisible(true);
        _setNumFracLineVisible(true);
        _setRwLineVisible(false);
    } else if (wellType == "Vertical") {
        _setXfLineVisible(false);
        _setFcdLineVisible(false);
        _setLhLineVisible(false);
        _setNumFracLineVisible(false);
        _setRwLineVisible(true);
    } else if (wellType == "Horizontal") {
        _setXfLineVisible(false);
        _setFcdLineVisible(false);
        _setLhLineVisible(true);
        _setNumFracLineVisible(false);
        _setRwLineVisible(true);
    }
}

void MainWindow::_setupWellParamsUnits(const QString& units) {
    ui->labelFracXfUnits->setText(unitsMapper
                                  .value(mapStrToUnitSystem.value(units))
                                  .value(PARAMS::XF));
    ui->labelFracFcdUnits->setText(unitsMapper
                                   .value(mapStrToUnitSystem.value(units))
                                   .value(PARAMS::FCD));
    ui->labelHorizontalWellLengthUnits->setText(unitsMapper
                                                .value(mapStrToUnitSystem.value(units))
                                                .value(PARAMS::LH));
    ui->labelNumOfFracturesUnits->setText(unitsMapper
                                          .value(mapStrToUnitSystem.value(units))
                                          .value(PARAMS::NFRAC));
    ui->labelWellRadiusUnits->setText(unitsMapper
                                      .value(mapStrToUnitSystem.value(units))
                                      .value(PARAMS::RW));
}

void MainWindow::setupWellParamsLabels(){
    _setupWellParamsVisibility(uComboWellType->text());
    _setupWellParamsUnits(uComboUnits->text());
    connect(uComboWellType, &UniComboBox::textChanged,
            this, &MainWindow::_setupWellParamsVisibility);
    connect(uComboUnits, &UniComboBox::textChanged,
            this, &MainWindow::_setupWellParamsUnits);
}

void MainWindow::_setupDrainageAreaVisibility(const QString& area, const QString& welltype) {
    _setXeLineVisible(mapStrToAreaShape.value(area) == AREASHAPE::RECTANGULAR);
    _setXwLineVisible(mapStrToAreaShape.value(area) == AREASHAPE::RECTANGULAR);
    _setYeLineVisible(mapStrToAreaShape.value(area) == AREASHAPE::RECTANGULAR);
    _setYwLineVisible(mapStrToAreaShape.value(area) == AREASHAPE::RECTANGULAR);
    _setZwLineVisible(mapStrToWellType.value(welltype) == WELLTYPE::HORIZONTAL);
    _setReLineVisible(mapStrToAreaShape.value(area) == AREASHAPE::CIRCLED);
}

void MainWindow::_setupDrainageAreaUnits(const QString& units) {
    ui->labelXeUnits->setText(unitsMapper.value(mapStrToUnitSystem.value(units)).value(PARAMS::XE));
    ui->labelXwUnits->setText(unitsMapper.value(mapStrToUnitSystem.value(units)).value(PARAMS::XW));
    ui->labelYeUnits->setText(unitsMapper.value(mapStrToUnitSystem.value(units)).value(PARAMS::YE));
    ui->labelYwUnits->setText(unitsMapper.value(mapStrToUnitSystem.value(units)).value(PARAMS::YW));
    ui->labelZwUnits->setText(unitsMapper.value(mapStrToUnitSystem.value(units)).value(PARAMS::ZW));
    ui->labelReUnits->setText(unitsMapper.value(mapStrToUnitSystem.value(units)).value(PARAMS::RE));
}

void MainWindow::setupDrainageAreaLabels(){
    _setupDrainageAreaVisibility(uComboAreaShape->text(), uComboWellType->text());
    _setupDrainageAreaUnits(uComboUnits->text());
    connect(uComboAreaShape, &UniComboBox::textChanged, [this](const QString& newArea) {
        _setupDrainageAreaVisibility(newArea, this->uComboWellType->text());
    });
    connect(uComboWellType, &UniComboBox::textChanged, [this](const QString& newWellType){
        _setupDrainageAreaVisibility(this->uComboAreaShape->text(), newWellType);
    });
}


void MainWindow::setupPicMan() {
    picMan = new PicManager(ui->labelWellScheme);
    picMan->setWellType(uComboWellType->text());
    picMan->setDrainageShape(uComboAreaShape->text());
    ui->labelWellScheme->setPixmap(picMan->getPixmap());
    connect(uComboWellType, SIGNAL(textChanged(const QString&)),
            picMan, SLOT(setWellType(const QString&)));
    connect(uComboAreaShape, SIGNAL(currentTextChanged(const QString&)),
            picMan, SLOT(setDrainageShape(const QString&)));
    connect(picMan, SIGNAL(sendPixmap(const QPixmap&)),
            ui->labelWellScheme, SLOT(setPixmap(const QPixmap&)));
}

void MainWindow::_setXfLineVisible(bool visible) {
    ui->labelFracXf->setVisible(visible);
    ui->labelFracXfUnits->setVisible(visible);
    ui->lineEditFracXf->setVisible(visible);
}

void MainWindow::_setFcdLineVisible(bool visible) {
    ui->labelFracCond->setVisible(visible);
    ui->lineEditFracFcd->setVisible(visible);
    ui->labelFracFcdUnits->setVisible(visible);
}

void MainWindow::_setLhLineVisible(bool visible) {
    ui->labelHorizontalWellLength->setVisible(visible);
    ui->lineEditHorizontalWellLength->setVisible(visible);
    ui->labelHorizontalWellLengthUnits->setVisible(visible);
}

void MainWindow::_setNumFracLineVisible(bool visible) {
    ui->labelNumOfFractures->setVisible(visible);
    ui->lineEditNumOfFractures->setVisible(visible);
    ui->labelNumOfFracturesUnits->setVisible(visible);
}

void MainWindow::_setRwLineVisible(bool visible) {
    ui->labelWellRadius->setVisible(visible);
    ui->lineEditWellRadius->setVisible(visible);
    ui->labelWellRadiusUnits->setVisible(visible);
}

void MainWindow::_setXeLineVisible(bool visible){
    ui->labelXe->setVisible(visible);
    ui->lineEditXe->setVisible(visible);
    ui->labelXeUnits->setVisible(visible);
}
void MainWindow::_setXwLineVisible(bool visible){
    ui->labelXw->setVisible(visible);
    ui->lineEditXw->setVisible(visible);
    ui->labelXwUnits->setVisible(visible);
}
void MainWindow::_setYeLineVisible(bool visible){
    ui->labelYe->setVisible(visible);
    ui->lineEditYe->setVisible(visible);
    ui->labelYeUnits->setVisible(visible);
}
void MainWindow::_setYwLineVisible(bool visible){
    ui->labelYw->setVisible(visible);
    ui->labelYwUnits->setVisible(visible);
    ui->lineEditYw->setVisible(visible);
}
void MainWindow::_setZwLineVisible(bool visible){
    ui->labelZw->setVisible(visible);
    ui->labelZwUnits->setVisible(visible);
    ui->lineEditZw->setVisible(visible);
}
void MainWindow::_setReLineVisible(bool visible){
    ui->labelRe->setVisible(visible);
    ui->labelReUnits->setVisible(visible);
    ui->lineEditRe->setVisible(visible);
}

void MainWindow::_setLiqRateLineVisible(bool visible) {
    ui->labelLiquidRate->setVisible(visible);
    ui->lineEditLiquidRate->setVisible(visible);
    ui->labelLiquidRateUnits->setVisible(visible);
}
void MainWindow::_setWellPresLineVisible(bool visible) {
    ui->labelWellborePressure->setVisible(visible);
    ui->labelWellborePressureUnits->setVisible(visible);
    ui->lineEditWellborePressure->setVisible(visible);
}
void MainWindow::_setupeCalcModeUnits(const QString& units) {
    ui->labelLiquidRateUnits->setText(unitsMapper.value(mapStrToUnitSystem.value(units)).value(PARAMS::LIQRATE));
    ui->labelWellborePressureUnits->setText(unitsMapper.value(mapStrToUnitSystem.value(units)).value(PARAMS::WELLPRES));
}
void MainWindow::_setupCalcModeVisibility(const QString& calcmode) {
    _setLiqRateLineVisible(mapStrToCalcMode.value(calcmode) == CALCMODE::LIQCONST);
    _setWellPresLineVisible(mapStrToCalcMode.value(calcmode) == CALCMODE::PRESCONST);
}
void MainWindow::setupCalcModeLabels() {
    _setupCalcModeVisibility(uComboCalcMode->text());
    _setupeCalcModeUnits(uComboUnits->text());
    connect(uComboCalcMode, &UniComboBox::textChanged,
            this, &MainWindow::_setupCalcModeVisibility);
    connect(uComboUnits, &UniComboBox::textChanged,
            this, &MainWindow::_setupeCalcModeUnits);
}


void MainWindow::_setTableScheduleHeaders(const QString& units) {
    tableScheduleModel->setHeaderData(0, Qt::Horizontal, "Time, " + unitsMapper
                                      .value(mapStrToUnitSystem.value(units))
                                      .value(PARAMS::TIME), Qt::EditRole);
    tableScheduleModel->setHeaderData(1, Qt::Horizontal, "Well rate, " + unitsMapper
                                      .value(mapStrToUnitSystem.value(units))
                                      .value(PARAMS::LIQRATE), Qt::DisplayRole);
    tableScheduleModel->setHeaderData(2, Qt::Horizontal, "Wellbore pressure, " + unitsMapper
                                      .value(mapStrToUnitSystem.value(units))
                                      .value(PARAMS::WELLPRES), Qt::DisplayRole);
}

void MainWindow::initTableScheduleModel() {
    tableScheduleModel = new QStandardItemModel(this);
    tableScheduleModel->setRowCount(tableScheduleModeInitRows);
    tableScheduleModel->setColumnCount(tableScheduleModelColumns);
    _setTableScheduleHeaders(uComboUnits->text());
    connect(uComboUnits, &UniComboBox::textChanged,
            this, &MainWindow::_setTableScheduleHeaders);
    connect(tableScheduleModel, &QStandardItemModel::itemChanged,
            this, &MainWindow::timeDataChanged);

    connect(uComboCalcMode, &UniComboBox::textChanged,
            [this](const QString& calcmode){
        this->setLiqAndPres(calcmode, ui->lineEditLiquidRate->text(), ui->lineEditWellborePressure->text());
    });

    connect(ui->lineEditLiquidRate, &QLineEdit::textChanged,
            [this](const QString& liqrate){
        this->setLiqAndPres(this->uComboCalcMode->text(), liqrate, this->ui->lineEditWellborePressure->text());
    });
    connect(ui->lineEditWellborePressure, &QLineEdit::textChanged,
            [this](const QString& wellpres){
        this->setLiqAndPres(this->uComboCalcMode->text(), this->ui->lineEditLiquidRate->text(), wellpres);
    });
}

void MainWindow::maybeAddTimeRow(QStandardItem* item) {
    int col = item->column();
    int row = item->row();
    if (col == 0 && (row == (tableScheduleModel->rowCount() - 1))) {
        QModelIndex liqIndex = tableScheduleModel->index(row, 1);
        QModelIndex presIndex = tableScheduleModel->index(row, 2);
        tableScheduleModel->setData(liqIndex, ui->lineEditLiquidRate->text());
        tableScheduleModel->setData(presIndex, ui->lineEditWellborePressure->text());
        QList<QStandardItem*> row;
        for (int c = 0; c < tableScheduleModel->columnCount(); ++c) {
            QStandardItem *item = new QStandardItem();
            row.append(item);
        }
        tableScheduleModel->appendRow(row);
    }
}

void MainWindow::setLiqAndPres(const QString& calcMode, const QString& liqvalue, const QString& presvalue) {
    if (calcMode == "Constant Liquid Rate") {
        for (int row = 0; row < tableScheduleModel->rowCount()-1; ++row) {
            QModelIndex itemIndex = tableScheduleModel->index(row, strToTableScheduleColumn.value(calcMode));
            tableScheduleModel->setData(itemIndex, liqvalue);
        }
    } else if (calcMode == "Constant Wellbore Pressure") {
        for (int row = 0; row < tableScheduleModel->rowCount()-1; ++row) {
            QModelIndex itemIndex = tableScheduleModel->index(row, strToTableScheduleColumn.value(calcMode));
            tableScheduleModel->setData(itemIndex, presvalue);
        }
    }

}

void MainWindow::timeDataChanged(QStandardItem* item) {
    maybeAddTimeRow(item);
}

void MainWindow::_setTableColsVisibility(const QString& calcmode) {
    ui->tableWellSchedule->hideColumn(1);
    ui->tableWellSchedule->hideColumn(2);
    ui->tableWellSchedule->showColumn(strToTableScheduleColumn.value(calcmode));
}

void MainWindow::setupTableSchedule() {
    initTableScheduleModel();
    ui->tableWellSchedule->setModel(tableScheduleModel);
    for(int c = 0; c < ui->tableWellSchedule->model()->columnCount(); c++)
    {
        if(c != 0)
            ui->tableWellSchedule->setItemDelegateForColumn(c, new NotEditableDelegate(ui->tableWellSchedule));
    }
    _setTableColsVisibility(uComboCalcMode->text());
    connect(uComboCalcMode, &UniComboBox::textChanged,
            this, &MainWindow::_setTableColsVisibility);
}

void MainWindow::setWellManagerData() {
    wellManager->setUnitSystem(uComboUnits->text());
    wellManager->setBoundary(uComboBoundaryConditions->text());
    wellManager->setDrainageArea(uComboAreaShape->text());
    wellManager->setWellType(uComboWellType->text());
    wellManager->setCalcMode(uComboCalcMode->text());
    wellManager->setXe(ui->lineEditXe->text());
    wellManager->setXw(ui->lineEditXw->text());
    wellManager->setYe(ui->lineEditYe->text());
    wellManager->setYw(ui->lineEditYw->text());
    wellManager->setZw(ui->lineEditZw->text());
    wellManager->setRw(ui->lineEditWellRadius->text());
    wellManager->setRe(ui->lineEditRe->text());
    wellManager->setFcd(ui->lineEditFracFcd->text());
    wellManager->setH(ui->lineEditH->text());
    wellManager->setLh(ui->lineEditHorizontalWellLength->text());
    wellManager->setXf(ui->lineEditFracXf->text());
    wellManager->setNFrac(ui->lineEditNumOfFractures->text());
    wellManager->setTSchedule(tableScheduleModel);
    wellManager->setP(ui->lineEditWellborePressure->text());
    wellManager->setQ(ui->lineEditLiquidRate->text());
    wellManager->setPinit(ui->lineEditPi->text());
    wellManager->setK(ui->lineEditK->text());
    wellManager->setFi(ui->lineEditFi->text());
    wellManager->setMu(ui->lineEditMu->text());
    wellManager->setBoil(ui->lineEditBoil->text());
    wellManager->setCt(ui->lineEditCt->text());
}
