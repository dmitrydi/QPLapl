#ifndef QGRID1D_H
#define QGRID1D_H

#include <QWidget>
#include <cmath>
#include <QString>
#include <vector>
#include <QLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QStringList>
#include "auxillary.h"
#include <algorithm>
#include <stdexcept>
#include <utility>
#include <optional>

enum QGridType {
    Lin,
    Log
};

enum LogDirection {
    MinToMax,
    MaxToMin
};

struct GridSettings {
    QGridType gType;
    int nSegments;
    std::optional<LogDirection> direction;
};

const QStringList gridType = {"Lin", "Log"};

std::vector<double> LinLogGrid(double xmin, double xmax, GridSettings settings, double factor = 1.1);


class QGrid1D : public QWidget
{
    Q_OBJECT
public:
    explicit QGrid1D(const QString& varTitle, LogDirection direction, QWidget *parent = nullptr);
    GridSettings GetGridSettings() const;
//private:
    LogDirection direction;
    QString varTitle;
    QHBoxLayout *hLayout;
    QLabel *titleLabel;
    QLineEdit *lineEdit;
    QComboBox *LinLogBox;

signals:

};

#endif // QGRID1D_H
