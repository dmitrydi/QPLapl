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
    nLeftInput->AddVisibilityController(regimeInput);
    nLeftInput->AddVisibilityMap(WellRegimes::Maps::liqrateVisibility);
    ui->layoutGridSetup->addWidget(nLeftInput);

    nRightInput = new TextComboLineInput("N right", {"Lin", "Log"}, welltypeInput, WellTypes::Maps::nRightVisibility);
    nRightInput->AddVisibilityController(areashapeInput);
    nRightInput->AddVisibilityMap(AreaShapes::Maps::nRightVisibility);
    nRightInput->AddVisibilityController(regimeInput);
    nRightInput->AddVisibilityMap(WellRegimes::Maps::liqrateVisibility);
    ui->layoutGridSetup->addWidget(nRightInput);

    nBottomInput = new TextComboLineInput("N bottom", {"Lin", "Log"}, welltypeInput, WellTypes::Maps::nBottomVisibility);
    nBottomInput->AddVisibilityController(areashapeInput);
    nBottomInput->AddVisibilityMap(AreaShapes::Maps::nBottomVisibility);
    nBottomInput->AddVisibilityController(regimeInput);
    nBottomInput->AddVisibilityMap(WellRegimes::Maps::liqrateVisibility);
    ui->layoutGridSetup->addWidget(nBottomInput);

    nTopInput = new TextComboLineInput("N top", {"Lin", "Log"}, welltypeInput, WellTypes::Maps::nTopVisibility);
    nTopInput->AddVisibilityController(areashapeInput);
    nTopInput->AddVisibilityMap(AreaShapes::Maps::nTopVisibility);
    nTopInput->AddVisibilityController(regimeInput);
    nTopInput->AddVisibilityMap(WellRegimes::Maps::liqrateVisibility);
    ui->layoutGridSetup->addWidget(nTopInput);

    nWellInput = new TextComboLineInput("N Well", {"Lin", "Log"}, welltypeInput, WellTypes::Maps::nWellVisibility);
    nWellInput->AddVisibilityController(areashapeInput);
    nWellInput->AddVisibilityMap(AreaShapes::Maps::nWellVisibility);
    nWellInput->AddVisibilityController(regimeInput);
    nWellInput->AddVisibilityMap(WellRegimes::Maps::liqrateVisibility);
    ui->layoutGridSetup->addWidget(nWellInput);

    nzBottomInput = new TextComboLineInput("Nz Bottom", {"Lin", "Log"}, welltypeInput, WellTypes::Maps::nzBottomVisibility);
    nzBottomInput->AddVisibilityController(areashapeInput);
    nzBottomInput->AddVisibilityMap(AreaShapes::Maps::nzBottomVisibility);
    nzBottomInput->AddVisibilityController(regimeInput);
    nzBottomInput->AddVisibilityMap(WellRegimes::Maps::liqrateVisibility);
    ui->layoutGridSetup->addWidget(nzBottomInput);

    nzTopInput = new TextComboLineInput("Nz Top", {"Lin", "Log"}, welltypeInput, WellTypes::Maps::nzTopVisibility);
    nzTopInput->AddVisibilityController(areashapeInput);
    nzTopInput->AddVisibilityMap(AreaShapes::Maps::nzTopVisibility);
    nzTopInput->AddVisibilityController(regimeInput);
    nzTopInput->AddVisibilityMap(WellRegimes::Maps::liqrateVisibility);
    ui->layoutGridSetup->addWidget(nzTopInput);

    nBetweenInput = new TextComboLineInput("N between", {"Lin", "Log"}, welltypeInput, WellTypes::Maps::nBetweenVisibility);
    nBetweenInput->AddVisibilityController(areashapeInput);
    nBetweenInput->AddVisibilityMap(AreaShapes::Maps::nBetweenVisibility);
    nBetweenInput->AddVisibilityController(regimeInput);
    nBetweenInput->AddVisibilityMap(WellRegimes::Maps::liqrateVisibility);
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

    // grid check box
    gridCheckBox = new CheckBoxHidable("Calculate grid", regimeInput, WellRegimes::Maps::liqrateVisibility);
    ui->checkBoxLayout->addWidget(gridCheckBox);
    /*
     *     TextComboLineInput *nRightInput;
    TextComboLineInput *nBottomInput;
    TextComboLineInput *nTopInput;
    TextComboLineInput *nWellInput;
    TextComboLineInput *nzBottomInput;
    TextComboLineInput *nzTopInput;
    TextComboLineInput *nBetweenInput;
     */


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
    gridSchedView->hide();
    // check box connections
    connect(gridCheckBox, &AbstractControlledHidable::WidgetVisibilityChanged, gridSchedView, &QWidget::setVisible);
    gridSchedView->SetVisible(gridCheckBox->IsChecked());
    ui->layoutGridSched->update();
    connect(gridCheckBox, &CheckBoxHidable::SetChecked, gridSchedView, &QWidget::setVisible);
    connect(gridCheckBox, &CheckBoxHidable::SetChecked, nLeftInput, &QWidget::setVisible);
    connect(gridCheckBox, &CheckBoxHidable::SetChecked, nRightInput, &QWidget::setVisible);
    connect(gridCheckBox, &CheckBoxHidable::SetChecked, nBottomInput, &QWidget::setVisible);
    connect(gridCheckBox, &CheckBoxHidable::SetChecked, nTopInput, &QWidget::setVisible);
    connect(gridCheckBox, &CheckBoxHidable::SetChecked, nWellInput, &QWidget::setVisible);
    connect(gridCheckBox, &CheckBoxHidable::SetChecked, nzBottomInput, &QWidget::setVisible);
    connect(gridCheckBox, &CheckBoxHidable::SetChecked, nzTopInput, &QWidget::setVisible);
    connect(gridCheckBox, &CheckBoxHidable::SetChecked, nBetweenInput, &QWidget::setVisible);
    nLeftInput->setVisible(gridCheckBox->IsChecked());
    nRightInput->setVisible(gridCheckBox->IsChecked());
    nBottomInput->setVisible(gridCheckBox->IsChecked());
    nTopInput->setVisible(gridCheckBox->IsChecked());
    nWellInput->setVisible(gridCheckBox->IsChecked());
    nzBottomInput->setVisible(gridCheckBox->IsChecked());
    nzTopInput->setVisible(gridCheckBox->IsChecked());
    nBetweenInput->setVisible(gridCheckBox->IsChecked());

}

MainWindow::~MainWindow()
{
    delete ui;
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
