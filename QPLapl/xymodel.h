#ifndef XYMODEL_H
#define XYMODEL_H
#include <QObject>
#include <QAbstractTableModel>
#include <QColor>
#include <QBrush>
#include "matrix3dv.h"


class XYModel: public QAbstractTableModel
{
    Q_OBJECT
public:
    XYModel(Matrix3DV* grid, size_t zInd = 0, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
private:
    Matrix3DV *grid_ptr;
    size_t zInd;
    double minVal;
    double maxVal;
    double scale(double val, double min, double max) const;
};

#endif // XYMODEL_H
