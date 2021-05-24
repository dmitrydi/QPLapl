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
#include "mainwindowdata.h"
#include "pqgraphwindow.h"

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

public slots:

private:
    friend class MainWindowData;
    Ui::MainWindow *ui;
    PQGraphWindow *graphWin;
    UniComboBox *uComboUnits;
    UniComboBox *uComboWellType;
    UniComboBox *uComboAreaShape;
    UniComboBox *uComboBoundaryConditions;
    UniComboBox *uComboCalcMode;
    PicManager *picMan;
    QStandardItemModel *tableScheduleModel;
    MainWindowData *dataContainer;
    WellManager *wellManager;
    QVector<DataPoint> PQTData;
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
    void _setTableScheduleHeaders(const QString& units);
    void initTableScheduleModel();
    void maybeAddTimeRow(QStandardItem* item);
    void setLiqAndPres(const QString& calcMode, const QString& liqvalue, const QString& presvalue) ;
    //--- table schedule functions
    void _setTableColsVisibility(const QString& calcmode);
    void setupTableSchedule();
    //-- Interaction with WellManager
    void setWellManagerData();
    void runWellManagerPQ();

    void refreshAndRunDialog();
private slots:
    void timeDataChanged(QStandardItem* item);

};
#endif // MAINWINDOW_H
