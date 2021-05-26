#ifndef TABLEWELLSCHEDULE_H
#define TABLEWELLSCHEDULE_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QStandardItemModel>
#include <QTableView>
#include <QLayout>
#include <QPushButton>
#include <QDebug>
#include "unicombobox.h"
#include "labelmapper.h"
#include "noteditabledelegate.h"

class TableWellSchedule : public QWidget
{
    Q_OBJECT
public:
    explicit TableWellSchedule(UniComboBox *unitsSrc
                               , UniComboBox *calcModeSrc
                               , QLineEdit *liqRateSrc
                               , QLineEdit *wellPresSrc
                               , QWidget *parent = nullptr);
    QStandardItemModel* getModel() const;

signals:
    void ButtonCalculatePressed();
    void ButtonShowPressed();
    void ButtonSavePressed();
private:
    UniComboBox *unitsSource;
    UniComboBox *calcModeSource;
    QLineEdit *liquidRateSource;
    QLineEdit *wellPressureSource;
    QStandardItemModel *tableModel;
    QTableView *tableView;
    QVBoxLayout *vLayout;
    QHBoxLayout *hLayout;
    QPushButton *buttonCalculate;
    QPushButton *buttonShow;
    QPushButton *buttonSave;
    void SetTable();
    void SetButtons();
    void SetLayouts();
    //--table setup utilities
    void SetTableHeaders(const QString& units);
    void SetTableColumnsVisibility(const QString& calcmode);
    void MaybeAddRow(QStandardItem* item);
    void UpdateTableValues(const QString& liquidVal, const QString& pressureVal);
};

#endif // TABLEWELLSCHEDULE_H


// column count
// row count
// units source
// calc mode source
// liquid rate source
// wellbore pressure source
