#ifndef WELLMANAGER_H
#define WELLMANAGER_H

#include <sstream>
#include <QObject>
#include <utility>
#include <QList>
#include <QVector>
#include <QStandardItemModel>
#include <QHash>
#include "gwell.h"
#include "labelmapper.h"
#include <optional>
#include <stdexcept>

#define CHECK_OPT_PARAM(opt_param, text) {  \
    if(!(opt_param)) {                      \
        throw std::logic_error(text);       \
    }                                       \
}

#define CHECK_OPT(opt) {                               \
    if (!opt) {                                      \
        std::ostringstream os;                         \
        os << #opt << " is not set\n";                 \
        throw std::logic_error(os.str());              \
    }                                                  \
}



const QHash<QString, WellType> mapStrToGWellType = {
    {"Fracture", WellType::Fracture},
    {"Horizontal", WellType::Horizontal},
    {"Multifractured", WellType::MultiFractured},
    {"Vertical", WellType::Vertical}
};


const QHash<QString, Boundary> mapStrToGBoundary = {
    {"Constant Pressure", Boundary::CCCC},
    {"Impearmable", Boundary::NNNN}
};

const QHash<QString, DrainageArea> mapStrToGDrainageArea = {
    {"Rectangular", DrainageArea::Rectangular},
    {"Circled", DrainageArea::Circle},
    {"Infinite", DrainageArea::Infinite}
};

using DataPoint = std::pair<double, double>;

class ParamKeeper {
public:
    ParamKeeper() {};
    std::optional<double> xe, xw, ye, yw, zw, re, rw, Fcd, xf, lh, h;
    std::optional<double> perm, fi, mu, boil, ct;
    std::optional<double> pwell, qwell, pinit;
    std::optional<int> nfrac;
    std::optional<WellType> wt;
    std::optional<Boundary> bnd;
    std::optional<DrainageArea> drng;
    std::optional<UNITSYSTEM> usys;
    std::optional<CALCMODE> clcm;
    const QStandardItemModel *tpqmodel;
    double xed() const;
    double xwd() const;
    double yed() const;
    double ywd() const;
    double zwd() const;
    double red() const;
    double ld() const;
    double hd() const;
    double fcd() const;
    Boundary boundary() const;
    std::vector<double> GetTPQ() const;
    std::vector<double> GetTds(const std::vector<double>& ts) const;
    std::vector<double> GetPQ(const std::vector<double>& vals) const;
    std::vector<double> GetP(const std::vector<double>& pds) const;
    std::vector<double> GetQ(const std::vector<double>& qds) const;
private:
    double LRef() const;
};

class WellManager : public QObject
{
    Q_OBJECT
public:

    explicit WellManager(QObject *parent = nullptr);
    QVector<DataPoint> PQCalc(); // to be modified to take a QAbstractItemModel*
    QList<Matrix3DV> GridCalc() const; // to be modified to take a QAbstractItemModel*
    void setWellType(const QString&);
    void setBoundary(const QString&);
    void setDrainageArea(const QString&);
    void setUnitSystem(const QString&);
    void setCalcMode(const QString&);
    void setXe(const QString&);
    void setXw(const QString&);
    void setYe(const QString&);
    void setYw(const QString&);
    void setZw(const QString&);
    void setRe(const QString&);
    void setRw(const QString&);
    void setFcd(const QString&);
    void setH(const QString&);
    void setXf(const QString&);
    void setLh(const QString&);
    void setNFrac(const QString&);
    void setTSchedule(const QStandardItemModel*);
    void setQ(const QString&);
    void setP(const QString&);
    void setPinit(const QString&);
    ///
    void setK(const QString&);
    void setFi(const QString&);
    void setMu(const QString&);
    void setBoil(const QString&);
    void setCt(const QString&);

signals:
    void WellIsSet(bool);

private:
    mutable LaplWell *well;
    ParamKeeper pk;
    void CreateWell();
};

#endif // WELLMANAGER_H
