#include "xymodel.h"

XYModel::XYModel(Matrix3DV* grid,
                 size_t zInd,
                 QObject *parent): QAbstractTableModel(parent),
                                   grid_ptr(grid),
                                   zInd(zInd),
                                   minVal(grid_ptr->GetSliceOverZAxis(zInd).GetMinVal()),
                                   maxVal(grid_ptr->GetSliceOverZAxis(zInd).GetMaxVal()) {

}

int XYModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return grid_ptr->GetSizeOnAxis(MatrixAxis::X);
}

int XYModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return grid_ptr->GetSizeOnAxis(MatrixAxis::Y);
}

QVariant XYModel::data(const QModelIndex &index, int role) const {
    switch (role) {
    case Qt::DisplayRole:
        return QString("%1").arg(grid_ptr->operator()(index.row(),
                                                      index.column(),
                                                      zInd).val);
    case Qt::BackgroundRole:
    {
        double val = grid_ptr->operator()(index.row(),
                                          index.column(),
                                          zInd).val;
        int r = static_cast<int>(255.*scale(val, minVal, maxVal));
        int g = static_cast<int>(255.*(1-scale(val, minVal, maxVal)));
        QColor color(r,g,0);
        return QBrush(color);
    }
    case Qt::SizeHintRole:
        return QSize(1000, 20);
    }
    return QVariant();
}

double XYModel::scale(double val, double min, double max) const {
    return (val-min)/(max-min);
}
