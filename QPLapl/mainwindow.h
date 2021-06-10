#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QTableView>
#include <QStandardItemModel>
#include <QSpacerItem>
#include <QFileDialog>
#include <QTextStream>
#include "picmanager.h"
#include "noteditabledelegate.h"
#include "pqgraphwindow.h"
//#include "qgrid1d.h"
#include "gridplot.h"
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
    void RunPQCalc();
    void RunGridCalc();
public slots:
    void SavePQData();
    void SaveGridData();
private:
    Ui::MainWindow *ui;
    PQGraphWindow *graphWin;
    GridPlot *gridWin;
    PicManager *picMan;
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
    void AlignLineInputs(QVBoxLayout *vLayout);
private slots:
    void setupWellController();
    //test functions
    void CatchComboSendText(const QString& t) {
        qDebug() << "CatchComboSendText: " << t;
    }
};


#endif // MAINWINDOW_H
