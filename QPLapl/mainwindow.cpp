#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , graphWin(new PQGraphWindow(this))
    , gridWin(new GridPlot(this))
    , wellController(new WellController(this))
{
    ui->setupUi(this);
    wellManager = new WellManager(this);
    ui->tabWidget->setCurrentIndex(0);

    // setup layoutUnits
    unitsInput = new ComboLineinput("Units");
    unitsInput->AddComboItems(Units::Units);
    ui->layoutUnits->addWidget(unitsInput);
    ui->layoutUnits->addStretch(1);

    // setup layoutFulidRock
    ui->layoutFluidRock->setContentsMargins(0,0,0,0);
    permInput = new TextLineInput("Permeability", unitsInput, Units::Maps::Permeability);
    ui->layoutFluidRock->addWidget(permInput);
    permInput->SetDefaultValue("1");

    payInput = new TextLineInput("Pay zone", unitsInput, Units::Maps::PayZone);
    ui->layoutFluidRock->addWidget(payInput);
    payInput->SetDefaultValue("10");

    poroInput = new TextLineInput("Porosity", unitsInput, Units::Maps::Porosity);
    ui->layoutFluidRock->addWidget(poroInput);
    poroInput->SetDefaultValue("0.1");

    viscInput = new TextLineInput("Viscosity", unitsInput, Units::Maps::Viscosity);
    ui->layoutFluidRock->addWidget(viscInput);
    viscInput->SetDefaultValue("1");

    ctInput = new TextLineInput("Total Compressibility", unitsInput, Units::Maps::Ct);
    ui->layoutFluidRock->addWidget(ctInput);
    ctInput->SetDefaultValue("0.0001");

    fvfInput = new TextLineInput("Formation Volume Factor", unitsInput, Units::Maps::FVF);
    ui->layoutFluidRock->addWidget(fvfInput);
    fvfInput->SetDefaultValue("1");

    pinitInput = new TextLineInput("Initial Formation Pressure", unitsInput, Units::Maps::Pinit);
    ui->layoutFluidRock->addWidget(pinitInput);
    pinitInput->SetDefaultValue("250");

    ui->layoutFluidRock->addStretch(1);

    AlignLineInputs(ui->layoutFluidRock);

    // setup layoutWellProps
    welltypeInput = new ComboLineinput("Well Type");
    welltypeInput->AddComboItems(WellTypes::WellTypes);
    ui->layoutWellProps->addWidget(welltypeInput);

    xfInput = new TextLineInput("Fracture Half-Length", unitsInput, Units::Maps::Distance, welltypeInput, WellTypes::Maps::xfVisibility);
    ui->layoutWellProps->addWidget(xfInput);
    xfInput->SetDefaultValue("100");

    fcdInput = new TextLineInput("Fracture Conductivity", unitsInput, Units::Maps::DimentionlessConductivity, welltypeInput, WellTypes::Maps::fcdVisibility);
    ui->layoutWellProps->addWidget(fcdInput);
    fcdInput->SetDefaultValue("10");

    lhInput = new TextLineInput("Horizontal Well Length", unitsInput, Units::Maps::Distance, welltypeInput, WellTypes::Maps::lhVisibility);
    ui->layoutWellProps->addWidget(lhInput);
    lhInput->SetDefaultValue("100");

    nfracInput = new TextLineInput("Number of Fractures", unitsInput, Units::Maps::Count, welltypeInput, WellTypes::Maps::nfracVisibility);
    ui->layoutWellProps->addWidget(nfracInput);
    nfracInput->SetDefaultValue("1");

    rwInput = new TextLineInput("Well Radius", unitsInput, Units::Maps::Distance, welltypeInput, WellTypes::Maps::rwVisibility);
    ui->layoutWellProps->addWidget(rwInput);
    rwInput->SetDefaultValue("0.1");

    ui->layoutWellProps->addStretch(1);

    AlignLineInputs(ui->layoutWellProps);

    // setup layoutDrainageProps
    areashapeInput = new ComboLineinput("Area Shape");
    areashapeInput->AddComboItems(AreaShapes::AreaShapes);
    ui->layoutDrainageProps->addWidget(areashapeInput);

    xeInput = new TextLineInput("Xe", unitsInput, Units::Maps::Distance, areashapeInput, AreaShapes::Maps::xeVisibility);
    ui->layoutDrainageProps->addWidget(xeInput);
    xeInput->SetDefaultValue("2000");

    yeInput = new TextLineInput("Ye", unitsInput, Units::Maps::Distance, areashapeInput, AreaShapes::Maps::xeVisibility);
    ui->layoutDrainageProps->addWidget(yeInput);
    yeInput->SetDefaultValue("2000");

    xwInput = new TextLineInput("Xw", unitsInput, Units::Maps::Distance, areashapeInput, AreaShapes::Maps::xwVisibility);
    ui->layoutDrainageProps->addWidget(xwInput);
    xwInput->SetDefaultValue("1000");

    ywInput = new TextLineInput("Yw", unitsInput, Units::Maps::Distance, areashapeInput, AreaShapes::Maps::ywVisibility);
    ui->layoutDrainageProps->addWidget(ywInput);
    ywInput->SetDefaultValue("1000");

    zwInput = new TextLineInput("Zw", unitsInput, Units::Maps::Distance, welltypeInput, WellTypes::Maps::zwVisibility);
    ui->layoutDrainageProps->addWidget(zwInput);
    zwInput->SetDefaultValue("5");

    reInput = new TextLineInput("Re", unitsInput, Units::Maps::Distance, areashapeInput, AreaShapes::Maps::reVisibility);
    ui->layoutDrainageProps->addWidget(reInput);
    reInput->SetDefaultValue("1000");

    boundaryInput = new ComboLineinput("Boundary Conditions");
    boundaryInput->AddComboItems(BoundaryTypes::BoundaryTypes);
    ui->layoutDrainageProps->addWidget(boundaryInput);

    ui->layoutDrainageProps->addStretch(1);

    AlignLineInputs(ui->layoutDrainageProps);

    // setup layoutRegime
    regimeInput = new ComboLineinput("Regime");
    regimeInput->AddComboItems(WellRegimes::WellRegimes);
    ui->layoutRegime->addWidget(regimeInput);

    liqrateInput = new TextLineInput("Liquid Rate", unitsInput, Units::Maps::LiquidRate, regimeInput, WellRegimes::Maps::liqrateVisibility);
    ui->layoutRegime->addWidget(liqrateInput);

    wellpresInput = new TextLineInput("Wellbore Pressure", unitsInput, Units::Maps::Pressure, regimeInput, WellRegimes::Maps::wellpresVisibility);
    ui->layoutRegime->addWidget(wellpresInput);

    ui->layoutRegime->addStretch(1);

    AlignLineInputs(ui->layoutRegime);

    // setup Pic Manager
    picMan = new PicManager(ui->wellScheme);
    picMan -> setWellType(welltypeInput->CurrentText());
    picMan->setDrainageShape(areashapeInput->CurrentText());
    ui->wellScheme->setPixmap(picMan->getPixmap());
    connect(welltypeInput, &ComboLineinput::SendText, picMan, &PicManager::setWellType);
    connect(areashapeInput, &ComboLineinput::SendText, picMan, &PicManager::setDrainageShape);
    connect(picMan, &PicManager::sendPixmap, ui->wellScheme, &QLabel::setPixmap);
    connect(welltypeInput, &ComboLineinput::SendText, this, &MainWindow::CatchComboSendText);

    // setup layoutGridSetup
    nLeftInput = new TextComboLineInput("N left", {"Lin", "Log"}, welltypeInput, WellTypes::Maps::nLeftVisibility);
    nLeftInput->AddVisibilityController(areashapeInput);
    nLeftInput->AddVisibilityMap(AreaShapes::Maps::nLeftVisibility);
    ui->layoutGridSetup->addWidget(nLeftInput);

    nRightInput = new TextComboLineInput("N right", {"Lin", "Log"}, welltypeInput, WellTypes::Maps::nRightVisibility);
    nRightInput->AddVisibilityController(areashapeInput);
    nRightInput->AddVisibilityMap(AreaShapes::Maps::nRightVisibility);
    ui->layoutGridSetup->addWidget(nRightInput);

    nBottomInput = new TextComboLineInput("N bottom", {"Lin", "Log"}, welltypeInput, WellTypes::Maps::nBottomVisibility);
    nBottomInput->AddVisibilityController(areashapeInput);
    nBottomInput->AddVisibilityMap(AreaShapes::Maps::nBottomVisibility);
    ui->layoutGridSetup->addWidget(nBottomInput);

    nTopInput = new TextComboLineInput("N top", {"Lin", "Log"}, welltypeInput, WellTypes::Maps::nTopVisibility);
    nTopInput->AddVisibilityController(areashapeInput);
    nTopInput->AddVisibilityMap(AreaShapes::Maps::nTopVisibility);
    ui->layoutGridSetup->addWidget(nTopInput);

    nWellInput = new TextComboLineInput("N Well", {"Lin", "Log"}, welltypeInput, WellTypes::Maps::nWellVisibility);
    nWellInput->AddVisibilityController(areashapeInput);
    nWellInput->AddVisibilityMap(AreaShapes::Maps::nWellVisibility);
    ui->layoutGridSetup->addWidget(nWellInput);

    nzBottomInput = new TextComboLineInput("Nz Bottom", {"Lin", "Log"}, welltypeInput, WellTypes::Maps::nzBottomVisibility);
    nzBottomInput->AddVisibilityController(areashapeInput);
    nzBottomInput->AddVisibilityMap(AreaShapes::Maps::nzBottomVisibility);
    ui->layoutGridSetup->addWidget(nzBottomInput);

    nzTopInput = new TextComboLineInput("Nz Top", {"Lin", "Log"}, welltypeInput, WellTypes::Maps::nzTopVisibility);
    nzTopInput->AddVisibilityController(areashapeInput);
    nzTopInput->AddVisibilityMap(AreaShapes::Maps::nzTopVisibility);
    ui->layoutGridSetup->addWidget(nzTopInput);

    nBetweenInput = new TextComboLineInput("N between", {"Lin", "Log"}, welltypeInput, WellTypes::Maps::nBetweenVisibility);
    nBetweenInput->AddVisibilityController(areashapeInput);
    nBetweenInput->AddVisibilityMap(AreaShapes::Maps::nBetweenVisibility);
    ui->layoutGridSetup->addWidget(nBetweenInput);

    ui->layoutGridSetup->addStretch(1);

    // setup Well Schedule
    wellSchedView = new PQTView(
                liqrateInput,
                wellpresInput,
                unitsInput,
                Units::Maps::TimeSmall,
                Units::Maps::LiquidRate,
                Units::Maps::Pressure,
                regimeInput,
                WellRegimes::Maps::liqrateVisibility,
                WellRegimes::Maps::wellpresVisibility);
    ui->layoutWellSched->addWidget(wellSchedView);
    gridSchedView = new PQTView(
                liqrateInput,
                wellpresInput,
                unitsInput,
                Units::Maps::TimeSmall,
                Units::Maps::LiquidRate,
                Units::Maps::Pressure,
                regimeInput,
                WellRegimes::Maps::liqrateVisibility,
                WellRegimes::Maps::wellpresVisibility
                );
    ui->layoutGridSched->addWidget(gridSchedView);

    // Interface connections
    // PQT connections
    connect(wellSchedView, &PQTView::CalcButtonPressed, this, &MainWindow::setupWellController);
    connect(this, &MainWindow::RunPQCalc, wellController, &WellController::CalculatePQ);
    connect(wellController, &WellController::GraphDataReady, graphWin, &PQGraphWindow::FillData);
    connect(wellSchedView, &PQTView::ShowButtonPressed, graphWin, &PQGraphWindow::ShowGraph);
    connect(wellSchedView, &PQTView::SaveButtonPressed, this, &MainWindow::SavePQData);
    connect(graphWin, &PQGraphWindow::SaveData, this, &MainWindow::SavePQData);
    // Grid Connections
    connect(gridSchedView, &PQTView::CalcButtonPressed, this, &MainWindow::setupWellController);
    connect(this, &MainWindow::RunGridCalc, wellController, &WellController::CalculateGrid);
    connect(wellController, &WellController::GridReady, gridWin, &GridPlot::FillData);
    connect(gridSchedView, &PQTView::ShowButtonPressed, gridWin, &GridPlot::ShowGraph);
    connect(gridSchedView, &PQTView::SaveButtonPressed, this, &MainWindow::SaveGridData);
    connect(gridWin, &GridPlot::SaveData, this, &MainWindow::SaveGridData);



    //connect(wellSchedView, &PQTView::CalcButtonPressed, this, &MainWindow::TestPQTGetters);

    /*
     *     // new objects

    // tab Schedule

    //

    */
    /*
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
    SetupPQSchedule();
    SetupGridSchedule();
    SetupGrids();
    connect(this, &MainWindow::PQDataCalculated, graphWin, &PQGraphWindow::FillData);
    connect(graphWin, &PQGraphWindow::SaveData, this, &MainWindow::SavePQData);

    connect(this, &MainWindow::GridCalculated, gridWin, &GridPlot::FillData);
    connect(gridWin, &GridPlot::SaveData, this, &MainWindow::SaveGridData);
    QList<QGrid1D*> gridWidgets = this->findChildren<QGrid1D*>();
    if (!gridWidgets.isEmpty()) {
        for (int i = 0; i < gridWidgets.size(); ++i) {
            auto curWidget = gridWidgets.at(i);
            qDebug() << curWidget->titleLabel->text() << " " << curWidget->titleLabel->sizeHint() << " " << curWidget->titleLabel->sizePolicy() ;
        }
    } else {
        qDebug() << "No gridWidgets in MianWindow";
    }
    testAbstractLineInput();
    */
}

void MainWindow::SetupPQSchedule() {
    PQSchedule = new TableWellSchedule(uComboUnits, uComboCalcMode, ui->lineEditLiquidRate, ui->lineEditWellborePressure);
    ui->PQSchedLayout->addWidget(PQSchedule);
    connect(PQSchedule, &TableWellSchedule::ButtonCalculatePressed, this, &MainWindow::runWellManagerPQ);
    connect(PQSchedule, &TableWellSchedule::ButtonShowPressed, graphWin, &PQGraphWindow::ShowGraph);
    connect(PQSchedule, &TableWellSchedule::ButtonSavePressed, this, &MainWindow::SavePQData);
}
void MainWindow::SetupGridSchedule() {
    GridSchedule = new TableWellSchedule(uComboUnits, uComboCalcMode, ui->lineEditLiquidRate, ui->lineEditWellborePressure);
    ui->GridSchedLayout->addWidget(GridSchedule);
    connect(GridSchedule, &TableWellSchedule::ButtonCalculatePressed, this, &MainWindow::runWellManagerGrid);
    connect(GridSchedule, &TableWellSchedule::ButtonShowPressed, gridWin, &GridPlot::ShowGraph);
    connect(GridSchedule, &TableWellSchedule::ButtonSavePressed, this, &MainWindow::SaveGridData);
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

void MainWindow::runWellManagerGrid() {
    setWellManagerData();
    GridData = wellManager->GridCalc();
    emit GridCalculated(GridData);
}

void MainWindow::SetupGrids()
{
    gNxLeft = new QGrid1D("Nx left", LogDirection::MaxToMin);
    gNxWell = new QGrid1D("Nx well", LogDirection::MinToMax);
    gNxRight = new QGrid1D("Nx right", LogDirection::MinToMax);
    gNyBottom = new QGrid1D("Ny bottom", LogDirection::MaxToMin);
    gNyTop = new QGrid1D("Ny top", LogDirection::MinToMax);
    gNyWell = new QGrid1D("Ny well", LogDirection::MinToMax);
    gNzBottom = new QGrid1D("Nz bottom", LogDirection::MaxToMin);
    gNzTop = new QGrid1D("Nz top", LogDirection::MinToMax);
    gNxBetween = new QGrid1D("Nx between", LogDirection::MinToMax);
    gNyBetween = new QGrid1D("Ny between", LogDirection::MinToMax);

    ui->gridLayout->setSpacing(0);
    ui->gridLayout->setContentsMargins(0,0,0,0);

    ui->gridLayout->addWidget(gNxLeft);
    ui->gridLayout->addWidget(gNxWell);
    ui->gridLayout->addWidget(gNxRight);
    ui->gridLayout->addWidget(gNyBottom);
    ui->gridLayout->addWidget(gNyTop);
    ui->gridLayout->addWidget(gNzBottom);
    ui->gridLayout->addWidget(gNzTop);
    ui->gridLayout->addWidget(gNxBetween);
    ui->gridLayout->addWidget(gNyBetween);
    ui->gridLayout->insertStretch(-1,1);
    gNyWell->hide();

}

void MainWindow::testAbstractLineInput()
{
    comboLine = new ComboLineinput("Unit System");
    ui->testLayoutController->addWidget(comboLine);
    comboLine->AddComboItems(Units::Units);

    comboVisible = new ComboLineinput("Visibility");
    comboVisible->AddComboItem("Visible");
    comboVisible->AddComboItem("Invisible");
    QHash<QString, VisibilityState> mapVisible = {{"Visible", VisibilityState::Visible}, {"Invisible", VisibilityState::Invisible}};
    ui->testLayoutController->addWidget(comboVisible);

    textLine = new TextLineInput("perm", comboLine, Units::Maps::Permeability, comboVisible, mapVisible);
    textLine->SetDefaultUnits("custom");
    textLine->SetTitleMinWidth(10);
    textLine->SetUnitsMinWidth(40);
    ui->testLayoutReciever->addWidget(textLine);


}

void MainWindow::setupWellController()
{
    wellController->setXe(xeInput->CurrentText());
    qDebug() << "setXe OK";
    wellController->setXw(xwInput->CurrentText());
    qDebug() << "setXw OK";
    wellController->setYe(yeInput->CurrentText());
    wellController->setYw(ywInput->CurrentText());
    wellController->setZw(zwInput->CurrentText());
    wellController->setRe(reInput->CurrentText());
    wellController->setRw(rwInput->CurrentText());
    wellController->setFcd(fcdInput->CurrentText());
    wellController->setXf(xfInput->CurrentText());
    wellController->setLh(lhInput->CurrentText());
    wellController->setH(payInput->CurrentText());
    wellController->setK(permInput->CurrentText());
    wellController->setFi(poroInput->CurrentText());
    wellController->setMu(viscInput->CurrentText());
    wellController->setBoil(fvfInput->CurrentText());
    wellController->setCt(ctInput->CurrentText());
    wellController->setPwell(wellpresInput->CurrentText());
    wellController->setQwell(liqrateInput->CurrentText());
    wellController->setPinit(pinitInput->CurrentText());
    wellController->setNfrac(nfracInput->CurrentText());
    wellController->setWelltype(welltypeInput->CurrentText());
    wellController->setBoundaryConditions(boundaryInput->CurrentText());
    wellController->setAreaShape(areashapeInput->CurrentText());
    wellController->setUnitsystem(unitsInput->CurrentText());
    wellController->setCalcmode(regimeInput->CurrentText());
    qDebug() << "setCalc mode OK";
    wellController->setTimeSchedule(wellSchedView->GetTValues());
    qDebug() << "setTimeSchedule OK";
    wellController->setTimeShcheduleGrid(gridSchedView->GetTValues());
    qDebug() << "setTimeShcheduleGrid OK";
    wellController->setNLeft(nLeftInput->CurrentText(), nLeftInput->ComboText());
    wellController->setNRight(nRightInput->CurrentText(), nRightInput->ComboText());
    wellController->setNBottom(nBottomInput->CurrentText(), nBottomInput->ComboText());
    wellController->setNTop(nTopInput->CurrentText(), nTopInput->ComboText());
    wellController->setNWell(nWellInput->CurrentText(), nWellInput->ComboText());
    wellController->setNzBottom(nzBottomInput->CurrentText(), nzBottomInput->ComboText());
    wellController->setNzTop(nzTopInput->CurrentText(), nzTopInput->ComboText());
    wellController->setNBetween(nBetweenInput->CurrentText(), nBetweenInput->ComboText());
    qDebug() << "setNBetween OK";
    PQTView* snd = qobject_cast<PQTView*>(sender());
    qDebug() << "sender OK";
    if (snd == wellSchedView)
        emit RunPQCalc();
    else if (snd == gridSchedView)
        emit RunGridCalc();
    else
        qDebug() << "MainWindow::setupWellController(): unknown sender";

}

void MainWindow::AlignLineInputs(QVBoxLayout *vLayout)
{
    int maxTitleWidth = 0, maxUnitsWidth = 0;
    for (int i = 0; i < vLayout->count(); ++i) {
        QWidget *w = vLayout->itemAt(i)->widget();
        AbstractControlledHidable *h = dynamic_cast<AbstractControlledHidable*>(w);
        if (h) {
            if (h->GetTitleWidth() > maxTitleWidth)
                maxTitleWidth = h->GetTitleWidth();
            if (h->GetUnitsWidth() > maxUnitsWidth)
                maxUnitsWidth = h->GetUnitsWidth();
        }
    }
    for (int i = 0; i < vLayout->count(); ++i) {
        QWidget *w = vLayout->itemAt(i)->widget();
        AbstractControlledHidable *h = dynamic_cast<AbstractControlledHidable*>(w);
        if (h) {
            h->SetTitleMinWidth(maxTitleWidth);
            h->SetUnitsMinWidth(maxUnitsWidth);
        }
    }
}

void MainWindow::SavePQData()
{
    QString filePath = QFileDialog::getSaveFileName(this, "data.txt");
    if (filePath.isEmpty()) return;
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        wellController->SavePQT(stream);
    }
    file.close();
}

void MainWindow::SaveGridData() {
    QString filePath = QFileDialog::getSaveFileName(this, "grid_data.txt");
    if (filePath.isEmpty()) return;
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        wellController->SaveGrid(stream);
    }
    file.close();
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
    wellManager->setTSchedule(PQSchedule->getModel());
    wellManager->setGridSchedule(GridSchedule->getModel());
    wellManager->setP(ui->lineEditWellborePressure->text());
    wellManager->setQ(ui->lineEditLiquidRate->text());
    wellManager->setPinit(ui->lineEditPi->text());
    wellManager->setK(ui->lineEditK->text());
    wellManager->setFi(ui->lineEditFi->text());
    wellManager->setMu(ui->lineEditMu->text());
    wellManager->setBoil(ui->lineEditBoil->text());
    wellManager->setCt(ui->lineEditCt->text());
    // setup grid
    wellManager->SetNxLeft(gNxLeft->GetGridSettings());
    wellManager->SetNxWell(gNxWell->GetGridSettings());
    wellManager->SetNxRight(gNxRight->GetGridSettings());
    wellManager->SetNyBottom(gNyBottom->GetGridSettings());
    wellManager->SetNyTop(gNyTop->GetGridSettings());
    wellManager->SetNyWell(gNyWell->GetGridSettings());
    wellManager->SetNzBottom(gNzBottom->GetGridSettings());
    wellManager->SetNzTop(gNzTop->GetGridSettings());
    wellManager->SetNxBetween(gNxBetween->GetGridSettings());
    wellManager->SetNyBetween(gNyBetween->GetGridSettings());
}
