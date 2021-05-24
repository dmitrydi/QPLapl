#include "wellmanager.h"
#include <QDebug>

WellManager::WellManager(QObject *parent) : QObject(parent),
  well(nullptr)
{}

QVector<DataPoint> ZipToDatapointVector(const std::vector<double>& ts, const std::vector<double>& vals) {
    if (ts.size() != vals.size())
        throw std::invalid_argument("dimentions of vectors must be equal\n");
    size_t n = ts.size();
    QVector<DataPoint> res(n);
    for (size_t i = 0; i < n; ++i) {
        res[i] = {ts[i], vals[i]};
    }
    return res;
}

QVector<DataPoint> WellManager::PQCalc() {
    if (well)
        delete well;
    CreateWell();
    std::vector<double> ts = pk.GetTPQ();
    std::vector<double> tds = pk.GetTds(ts);
    std::vector<double> values(tds.size());
    switch (*pk.clcm) {
    case CALCMODE::LIQCONST:
        well->pwd_parallel(tds, values, 4);
        break;
    case CALCMODE::PRESCONST:
        well->qwd_parallel(tds, values, 4);
    }

    return ZipToDatapointVector(ts, pk.GetPQ(values));
}

QList<Matrix3DV> WellManager::GridCalc() const {
    //return {};
}



void WellManager::CreateWell() {
    switch (*pk.wt) {
    case WellType::Fracture:
        switch (*pk.drng) {
        case DrainageArea::Rectangular:
            switch (*pk.bnd) {
            case Boundary::NNNN:
                well = new Rectangular::Fracture(pk.boundary(), pk.xwd(), pk.xed(), pk.ywd(), pk.yed(), pk.fcd());
                break;
            default:
                throw std::logic_error("Boundary other than NNNN not implemented\n");
            }
            break;
        default:
            throw std::logic_error("Drainage Area other than Rectangular is not implemented\n");
        }
        break;
    default:
        throw std::logic_error("Welltype oter than Fracture is not implemented\n");
    }
}

void WellManager::setWellType(const QString& sval) {
    pk.wt = mapStrToGWellType.value(sval);
}
void WellManager::setBoundary(const QString& sval) {
    pk.bnd = mapStrToGBoundary.value(sval);
}
void WellManager::setDrainageArea(const QString& sval) {
    pk.drng = mapStrToGDrainageArea.value(sval);
}
void WellManager::setUnitSystem(const QString& sval) {
    pk.usys = mapStrToUnitSystem.value(sval);
}
void WellManager::setCalcMode(const QString& sval) {
    pk.clcm = mapStrToCalcMode.value(sval);
}
void WellManager::setXe(const QString& sval) {
    pk.xe = sval.toDouble();
}
void WellManager::setXw(const QString& sval) {
    pk.xw = sval.toDouble();
}
void WellManager::setYe(const QString& sval) {
    pk.ye = sval.toDouble();
}
void WellManager::setYw(const QString& sval) {
    pk.yw = sval.toDouble();
}
void WellManager::setZw(const QString& sval) {
    pk.zw = sval.toDouble();
}
void WellManager::setRe(const QString& sval) {
    pk.re = sval.toDouble();
}
void WellManager::setRw(const QString& sval) {
    pk.rw = sval.toDouble();
}
void WellManager::setFcd(const QString& sval) {
    pk.Fcd = sval.toDouble();
}
void WellManager::setH(const QString& sval) {
    pk.h = sval.toDouble();
}
void WellManager::setLh(const QString& sval) {
    pk.lh = 0.5*sval.toDouble();
}

void WellManager::setXf(const QString& sval) {
    pk.xf = sval.toDouble();
}

void WellManager::setNFrac(const QString& sval) {
    pk.nfrac = sval.toInt();
}
void WellManager::setTSchedule(const QStandardItemModel* m) {
    pk.tpqmodel = m;
}

void WellManager::setQ(const QString& sval) {
    pk.qwell = sval.toDouble();
}
void WellManager::setP(const QString& sval) {
    pk.pwell = sval.toDouble();
}
void WellManager::setPinit(const QString& sval) {
    pk.pinit = sval.toDouble();
}

void WellManager::setK(const QString& sval) {
    pk.perm = sval.toDouble();
}
void WellManager::setFi(const QString& sval) {
    pk.fi = sval.toDouble();
}
void WellManager::setMu(const QString& sval) {
    pk.mu = sval.toDouble();
}
void WellManager::setBoil(const QString& sval) {
    pk.boil = sval.toDouble();
}
void WellManager::setCt(const QString& sval) {
    pk.ct = sval.toDouble();
}

//-------------

double ParamKeeper::xed() const {
    CHECK_OPT_PARAM(xe, "Xe is not set\n");
    return *xe/LRef();
}
double ParamKeeper::xwd() const {
    CHECK_OPT_PARAM(xw, "Xw is not set\n");
    return *xw/LRef();
}
double ParamKeeper::yed() const {
    CHECK_OPT_PARAM(ye, "Ye is not set\n");
    return *ye/LRef();
}
double ParamKeeper::ywd() const {
    CHECK_OPT_PARAM(yw, "Yw is not set\n");
    return *yw/LRef();
}
double ParamKeeper::zwd() const {
    CHECK_OPT_PARAM(h, "H is not set\n");
    CHECK_OPT_PARAM(zw, "Zw is not set\n");
    CHECK_OPT_PARAM(rw, "Rw is not set\n");
    return (*zw + *rw)/(*h);
}
double ParamKeeper::red() const {
    CHECK_OPT_PARAM(re, "Re is not set\n");
    return *re/LRef();
}
double ParamKeeper::ld() const {
    return 1.;
}
double ParamKeeper::hd() const {
    CHECK_OPT_PARAM(h, "H is not set\n");
    return *h/LRef();
}

double ParamKeeper::fcd() const {
    CHECK_OPT(Fcd);
    return *Fcd;
}

Boundary ParamKeeper::boundary() const {
    CHECK_OPT_PARAM(bnd, "Boundary is not set\n");
    return *bnd;
}

std::vector<double> ParamKeeper::GetTPQ() const {
    CHECK_OPT(tpqmodel);
    std::vector<double> ts;
    ts.reserve(tpqmodel->rowCount());
    for (int r = 0; r < tpqmodel->rowCount(); ++r) {
        auto index = tpqmodel->index(r, 0);
        double val = tpqmodel->data(index).toDouble();
        if (val > std::numeric_limits<double>::min())
            ts.push_back(val);
    }
    return ts;
}

std::vector<double> ParamKeeper::GetTds(const std::vector<double>& ts) const {
    std::vector<double> tds;
    CHECK_OPT_PARAM(usys, "Unit system is not set\n");
    CHECK_OPT(perm);
    CHECK_OPT(fi);
    CHECK_OPT(mu);
    CHECK_OPT(ct);
    CHECK_OPT(tpqmodel);
    double dimt;
    switch (*usys) {
    case UNITSYSTEM::OILFIELD:
        dimt = 0.00036*(*perm)/(*fi)/(*mu)/(*ct)/LRef()/LRef();
        break;
    default:
        throw std::logic_error("Unit system is not implemented\n");
    }
    tds.reserve(tpqmodel->rowCount());
    for (const auto& t: ts) {
        tds.push_back(dimt*t);
    }
    return tds;
}

std::vector<double> ParamKeeper::GetP(const std::vector<double>& pds) const {
    CHECK_OPT_PARAM(usys, "Unit system is not set\n");
    CHECK_OPT(perm);
    CHECK_OPT(h);
    CHECK_OPT(qwell);
    CHECK_OPT(boil);
    CHECK_OPT(mu);
    CHECK_OPT(pinit);
    std::vector<double> ps;
    double dimp;
    switch (*usys) {
    case UNITSYSTEM::OILFIELD:
        dimp = 18.42*(*qwell)*(*mu)*(*boil)/(*perm)/(*h);
        ps.reserve(pds.size());
        for (double pd: pds) {
            ps.push_back(*pinit - dimp*pd);
        }
        break;
    default:
        throw std::logic_error("Unit system is not implemented\n");
    }
    return ps;
}
std::vector<double> ParamKeeper::GetQ(const std::vector<double>& qds) const {
    CHECK_OPT_PARAM(usys, "Unit system is not set\n");
    CHECK_OPT(perm);
    CHECK_OPT(h);
    CHECK_OPT(pwell);
    CHECK_OPT(boil);
    CHECK_OPT(mu);
    CHECK_OPT(pinit);
    std::vector<double> qs;
    double dimq;
    switch (*usys) {
    case UNITSYSTEM::OILFIELD:
        dimq = (*pinit - *pwell)*(*perm)*(*h)/(18.42*(*qwell)*(*mu)*(*boil));
        qs.reserve(qds.size());
        for (double qd: qds) {
            qs.push_back(dimq*qd);
        }
        break;
    default:
        throw std::logic_error("Unit system is not implemented\n");
    }
    return qs;
}

std::vector<double> ParamKeeper::GetPQ(const std::vector<double>& vals) const {
    switch (*clcm) {
    case CALCMODE::LIQCONST:
        return GetP(vals);
        break;
    case CALCMODE::PRESCONST:
        return GetQ(vals);
        break;
    }
}

double ParamKeeper::LRef() const {
    CHECK_OPT_PARAM(wt, "Well type not set\n");
    switch (*wt) {
    case WellType::Fracture:
        CHECK_OPT_PARAM(xf, "Fracture length is not set\n");
        return *xf;
    case WellType::Horizontal:
        CHECK_OPT_PARAM(lh, "Horizontal well length is not set\n");
        return *lh;
    case WellType::MultiFractured:
        CHECK_OPT_PARAM(xf, "Fracture length is not set\n");
        return *xf;
    case WellType::Vertical:
        CHECK_OPT_PARAM(rw, "Well radius is not set\n");
        return *rw;
    }
}
