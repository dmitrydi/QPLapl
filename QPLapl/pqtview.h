#ifndef PQTVIEW_H
#define PQTVIEW_H

#include <QWidget>
#include "abstractlineinput.h"
#include <vector>
#include <QStandardItemModel>
#include <QTableView>
#include <QBoxLayout>
#include <QPushButton>
#include "interfacemaps.h"

class PQTView : public QWidget
{
    Q_OBJECT
public:
    explicit PQTView(
            const AbstractLineInput *liquidSource,
            const AbstractLineInput *pressureSource,
            const AbstractLineInput *unitsController = nullptr,
            const QHash<QString, QString>& mapTimeUnits = {},
            const QHash<QString, QString>& mapLiquidUnits = {},
            const QHash<QString, QString>& mapPressureUnits = {},
            const AbstractLineInput *visibilityController = nullptr,
            const QHash<QString, VisibilityState>& mapLiquidVisibility = {},
            const QHash<QString, VisibilityState>& mapPressureVisibility = {},
            const AbstractControlledHidable *globalVisController = nullptr,
            QWidget *parent = nullptr
            );
    std::vector<double> GetTValues() const;
    std::vector<double> GetPValues() const;
    std::vector<double> GetQValues() const;
    void CheckGlobalControllerState();
    void SetVisible(bool);
private:
    const AbstractLineInput *liquidSource;
    const AbstractLineInput *pressureSource;
    const AbstractLineInput *unitsController;
    QHash<QString, QString> mapTimeUnits;
    QHash<QString, QString> mapLiquidUnits;
    QHash<QString, QString> mapPressureUnits;

    const AbstractLineInput *visibilityController;
    QHash<QString, VisibilityState> mapLiquidVisibility;
    QHash<QString, VisibilityState> mapPressureVisibility;

    const AbstractControlledHidable *globalController;

    QStandardItemModel *model;
    QTableView *view;

    QVBoxLayout *vLayout;
    QHBoxLayout *hLayout;
    QPushButton *buttonCalculate;
    QPushButton *buttonShow;
    QPushButton *buttonSave;
    QString GetCurrentUnitsByController(const QHash<QString, QString>& mapUnits) const;
    void SetCurrentColumnVisibility();
    std::vector<double> GetColumnValues(int column) const;
public slots:
    void onUnitsControllerChanged(const QString&);
    void onVisibilityControllerChanged(const QString&);
    void onLiquidSourceChanged(const QString&);
    void onPressureSourceChanged(const QString&);
private slots:
    void onModelItemChanged(QStandardItem*);
signals:
    void CalcButtonPressed();
    void ShowButtonPressed();
    void SaveButtonPressed();
};

#endif // PQTVIEW_H
