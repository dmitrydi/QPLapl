#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QTableView>
#include <QStandardItemModel>
#include "unicombobox.h"
#include "labelmapper.h"
#include "picmanager.h"
#include "noteditabledelegate.h"
#include "wellmanager.h"
#include "pqgraphwindow.h"
#include <QFileDialog>
#include <QTextStream>
#include "tablewellschedule.h"
#include "qgrid1d.h"
#include "gridplot.h"
#include <QSpacerItem>
#include "abstractlineinput.h"
#include "interfacemaps.h"
#include "pqtview.h"
#include "wellcontroller.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void PQDataCalculated(const QVector<DataPoint>&);
    void GridCalculated(const QList<Matrix3DV>&);
public slots:
    void SavePQData();
    void SaveGridData();
private:


    friend class MainWindowData;
    Ui::MainWindow *ui;
    PQGraphWindow *graphWin;
    GridPlot *gridWin;
    UniComboBox *uComboUnits;
    UniComboBox *uComboWellType;
    UniComboBox *uComboAreaShape;
    UniComboBox *uComboBoundaryConditions;
    UniComboBox *uComboCalcMode;
    PicManager *picMan;
    WellManager *wellManager;
    QVector<DataPoint> PQTData;
    QList<Matrix3DV> GridData;
    void setupUComboUnits();
    void setupUComboWellType();
    void setupUComboAeaShape();
    void setupUComboBoundaryConditions();
    void setupUComboCalcMode();
    void _setupPayUnits(const QString&);
    void setupPayUnitsLabels();
    void setupWellParamsLabels();
    void _setupWellParamsUnits(const QString&);
    void setupDrainageAreaLabels();
    void _setupWellParamsVisibility(const QString& wellType);
    void _setXfLineVisible(bool visible);
    void _setFcdLineVisible(bool visible);
    void _setLhLineVisible(bool visible);
    void _setNumFracLineVisible(bool visible);
    void _setRwLineVisible(bool visible);
    void _setupDrainageAreaVisibility(const QString& area, const QString& welltype);
    void _setupDrainageAreaUnits(const QString& units);
    void _setXeLineVisible(bool visible);
    void _setXwLineVisible(bool visible);
    void _setYeLineVisible(bool visible);
    void _setYwLineVisible(bool visible);
    void _setZwLineVisible(bool visible);
    void _setReLineVisible(bool visible);
    void _setLiqRateLineVisible(bool visible);
    void _setWellPresLineVisible(bool visible);
    void _setupeCalcModeUnits(const QString& units);
    void _setupCalcModeVisibility(const QString& calcmode);
    void setupPicMan();
    void setupCalcModeLabels();
    // table schedule model
    TableWellSchedule *PQSchedule;
    TableWellSchedule *GridSchedule;
    void SetupPQSchedule();
    void SetupGridSchedule();
    //-- Interaction with WellManager
    void setWellManagerData();
    void runWellManagerPQ();
    void runWellManagerGrid();
    void refreshAndRunDialog();
    //-- Spatial Calc
    QGrid1D *testGrid;
    void SetupGrids();
    QGrid1D *gNxLeft, *gNxWell, *gNxRight;
    QGrid1D *gNyBottom, *gNyTop, *gNyWell;
    QGrid1D *gNzBottom, *gNzTop;
    QGrid1D *gNxBetween, *gNyBetween;
    void testAbstractLineInput();
    TextLineInput *textLine;
    ComboLineinput *comboLine;
    ComboLineinput *comboVisible;

    // new objects
    WellController *wellController;
    // tab Units
    ComboLineinput *unitsInput;
    // tab Fluid and Rock
    TextLineInput *permInput;
    TextLineInput *payInput;
    TextLineInput *poroInput;
    TextLineInput *viscInput;
    TextLineInput *ctInput;
    TextLineInput *fvfInput;
    TextLineInput *pinitInput;
    // tab Well and Drainage Area
    ComboLineinput *welltypeInput;
    TextLineInput *xfInput;
    TextLineInput *fcdInput;
    TextLineInput *lhInput;
    TextLineInput *nfracInput;
    TextLineInput *rwInput;
    ComboLineinput *areashapeInput;
    TextLineInput *xeInput;
    TextLineInput *yeInput;
    TextLineInput *xwInput;
    TextLineInput *ywInput;
    TextLineInput *zwInput;
    TextLineInput *reInput;
    ComboLineinput *boundaryInput;
    // tab Schedule
    ComboLineinput *regimeInput;
    TextLineInput *liqrateInput;
    TextLineInput *wellpresInput;

    PQTView *wellSchedView;
    PQTView *gridSchedView;

    TextComboLineInput *nLeftInput;
    TextComboLineInput *nRightInput;
    TextComboLineInput *nBottomInput;
    TextComboLineInput *nTopInput;
    TextComboLineInput *nWellInput;
    TextComboLineInput *nzBottomInput;
    TextComboLineInput *nzTopInput;
    TextComboLineInput *nBetweenInput;
    //

    //test functions
    void TestPQTGetters() {
        auto Ts = wellSchedView->GetTValues();
        auto Ps = wellSchedView->GetPValues();
        auto Qs = wellSchedView->GetQValues();
        for (size_t i = 0; i < Ts.size(); ++i) {
            qDebug() << Ts[i] << " " << Ps[i] << " " <<Qs[i];
        }
    }

    void AlignLineInputs(QVBoxLayout *vLayout);
private slots:
    void setupWellController();
signals:
    void RunPQCalc();
    void RunGridCalc();


};


#endif // MAINWINDOW_H
