#include "wellmanager.h"
#include <QDebug>

WellManager::WellManager(QObject *parent) : QObject(parent),
  well(nullptr)
{}

WellManager::~WellManager() {
    if (well) delete well;
}

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
    std::vector<double> ts = pk.GetTs(pk.tpqmodel);
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

QList<Matrix3DV> WellManager::GridCalc() {
    //return {};
    QList<Matrix3DV> ans;
    if (well)
        delete well;
    CreateWell();
    std::vector<double> ts = pk.GetTs(pk.tgridmodel);
    std::vector<double> tds = pk.GetTds(ts);
    std::vector<double> xds = pk.GetGridX();
    std::vector<double> yds = pk.GetGridY();
    std::vector<double> zds = pk.GetGridZ();
    qDebug() << tds;
    qDebug() << xds;
    qDebug() << yds;
    qDebug() << zds;
    for (const auto& td: tds) {
        qDebug() << td;
        ans.push_back(well->pd_m_parallel(td, 4, xds, yds, zds));
        std::cerr << ans[ans.size()-1];
    }
    //pk.TransformToDimentionGrid(ans);
    return ans;
}

void WellManager::PrintParams(QTextStream& ts) const {
    PrintFluidRock(ts);
    PrintWell(ts);
    PrintDrainage(ts);
    PrintBoundary(ts);
    PrintPQHeader(ts);
}

void WellManager::CreateWell() {
    if (well) delete well;
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

void WellManager::setGridSchedule(const QStandardItemModel *m) {
    pk.tgridmodel = m;
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

std::vector<double> ParamKeeper::GetTs(const QStandardItemModel *tpqmodel) const {
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

void WellManager::PrintFluidRock(QTextStream& ts) const {
    CHECK_OPT(pk.usys);
    CHECK_OPT(pk.perm);
    CHECK_OPT(pk.h);
    CHECK_OPT(pk.fi);
    CHECK_OPT(pk.mu);
    CHECK_OPT(pk.ct);
    CHECK_OPT(pk.pinit);
    CHECK_OPT(pk.boil);
    QString permUnits;
    QString hUnits;
    QString poroUnits;
    QString muUnits;
    QString ctUnits;
    QString pinitUnits;
    QString boilUnits;
    switch (*pk.usys) {
    case UNITSYSTEM::OILFIELD:
        permUnits = " md";
        hUnits = " m";
        poroUnits = " fraction";
        muUnits = " cP";
        ctUnits = " 1/atm";
        pinitUnits = " atm";
        boilUnits = " std.m3/m3";
        break;
    case UNITSYSTEM::SI:
        permUnits = " m2";
        hUnits = " m";
        poroUnits = " fraction";
        muUnits = " Pa*s";
        ctUnits = " 1/Pa";
        pinitUnits = " Pa";
        boilUnits = " std.m3/m3";
        break;
    case UNITSYSTEM::US:
        permUnits = " md";
        hUnits = " ft";
        poroUnits = " fraction";
        muUnits = " cP";
        ctUnits = " 1/psi";
        pinitUnits = " psi";
        boilUnits = " std.bbl/bbl";
        break;
    }
    ts << "====Fluid and Rock====\n";
    ts << "perm " << *pk.perm << permUnits << "\n";
    ts << "net pay " << *pk.h << hUnits << "\n";
    ts << "poro " << *pk.fi << poroUnits << "\n";
    ts << "viscosity " << *pk.mu << muUnits << "\n";
    ts << "compressibility " << *pk.ct << ctUnits << "\n";
    ts << "formation volume factor " << *pk.boil << boilUnits << "\n";
    ts << "initial formation pressure " << *pk.pinit << pinitUnits << "\n";
};

void WellManager::PrintWell(QTextStream& ts) const {
    CHECK_OPT(pk.usys);
    CHECK_OPT(pk.wt);
    QString lunits;
    switch (*pk.usys) {
    case UNITSYSTEM::OILFIELD:
        lunits = " m";
        break;
    case UNITSYSTEM::SI:
        lunits = " m";
        break;
    case UNITSYSTEM::US:
        lunits = " ft";
        break;
    }
    ts << "====Well====\n";
    switch (*pk.wt) {
    case WellType::Fracture:
        CHECK_OPT(pk.xf);
        CHECK_OPT(pk.Fcd);
        ts << "Fracture\n";
        ts << "xf " << *pk.xf << lunits << "\n";
        ts << "Fcd " << *pk.Fcd << "\n";
        break;
    case WellType::MultiFractured:
        CHECK_OPT(pk.xf);
        CHECK_OPT(pk.Fcd);
        CHECK_OPT(pk.lh);
        CHECK_OPT(pk.nfrac);
        ts << "Multifracture\n";
        ts << "xf " << *pk.xf << lunits << "\n";
        ts << "Fcd " << *pk.Fcd << "\n";
        ts << "Nfrac " << *pk.nfrac << "\n";
        ts << "Lh " << (*pk.lh)*2 << lunits << "\n";
        break;
    case WellType::Horizontal:
        CHECK_OPT(pk.lh);
        CHECK_OPT(pk.rw);
        CHECK_OPT(pk.zw);
        ts << "Horizontal\n";
        ts << "Lh " << (*pk.lh)*2 << lunits << "\n";
        ts << "zw " << *pk.zw << lunits << "\n";
        ts << "rw " << *pk.rw << lunits << "\n";
        break;
    case WellType::Vertical:
        CHECK_OPT(pk.rw);
        ts << "Vertical\n";
        ts << "rw " << *pk.rw << lunits << "\n";
        break;
    }
}
void WellManager::PrintDrainage(QTextStream& ts) const {
    CHECK_OPT(pk.usys);
    CHECK_OPT(pk.drng);
    QString lunits;
    switch (*pk.usys) {
    case UNITSYSTEM::OILFIELD:
        lunits = " m";
        break;
    case UNITSYSTEM::SI:
        lunits = " m";
        break;
    case UNITSYSTEM::US:
        lunits = " ft";
        break;
    }
    ts << "====Drainage Area====\n";
    switch (*pk.drng) {
    case DrainageArea::Rectangular:
        CHECK_OPT(pk.xe);
        CHECK_OPT(pk.ye);
        CHECK_OPT(pk.xw);
        CHECK_OPT(pk.yw);
        ts << "Rectangle\n";
        ts << "xe " << *pk.xe << lunits << "\n";
        ts << "ye " << *pk.ye << lunits << "\n";
        ts << "xw " << *pk.xw << lunits << "\n";
        ts << "yw " << *pk.yw << lunits << "\n";
        break;
    case DrainageArea::Circle:
        CHECK_OPT(pk.re);
        ts << "Circle\n";
        ts << "re " << *pk.re << lunits << "\n";
        break;
    case DrainageArea::Infinite:
        ts << "Infinite\n";
        break;
    }
}
void WellManager::PrintBoundary(QTextStream& ts) const {
    CHECK_OPT(pk.bnd);
    ts << "====Boundary====\n";
    switch (*pk.bnd) {
    case Boundary::NNNN:
        ts << "Impearmable\n";
        break;
    case Boundary::CCCC:
        ts << "Constant pressure\n";
        break;
    }
}
void WellManager::PrintPQHeader(QTextStream& ts) const {
    CHECK_OPT(pk.usys);
    CHECK_OPT(pk.clcm);
    QString qunits, punits, tunits;
    switch (*pk.usys) {
    case UNITSYSTEM::OILFIELD:
        qunits = " m3/day";
        punits = " atm";
        tunits = " hr";
        break;
    case UNITSYSTEM::SI:
        qunits = " m3/s";
        punits = " Pa";
        tunits = " s";
        break;
    case UNITSYSTEM::US:
        qunits = " bbl/day";
        punits = " psi";
        tunits = " hr";
        break;
    }
    ts << "====Well results====\n";
    switch (*pk.clcm) {
    case CALCMODE::LIQCONST:
        CHECK_OPT(pk.qwell);
        ts << "Liquid rate " << *pk.qwell << qunits << "\n";
        ts << "Time," << tunits << " " << "Wellbore Pressure, " << punits << "\n";
        break;
    case CALCMODE::PRESCONST:
        CHECK_OPT(pk.pwell);
        ts << "Wellbore pressure " << *pk.pwell << punits << "\n";
        ts << "Time," << tunits << " " << "Well Rate, " << qunits << "\n";
        break;
    }
}

void WellManager::SetNxLeft(const GridSettings settings) {
    pk.nxLeft = settings;
}
void WellManager::SetNxWell(const GridSettings settings) {
    pk.nxWell = settings;
}
void WellManager::SetNxRight(const GridSettings settings) {
    pk.nxRight = settings;
}
void WellManager::SetNyBottom(const GridSettings settings) {
    pk.nyBottom = settings;
}
void WellManager::SetNyTop(const GridSettings settings) {
    pk.nyTop = settings;
}
void WellManager::SetNyWell(const GridSettings settings) {
    pk.nyWell = settings;
}
void WellManager::SetNzBottom(const GridSettings settings) {
    pk.nzBottom = settings;
}
void WellManager::SetNzTop(const GridSettings settings) {
    pk.nzTop = settings;
}

void WellManager::SetNxBetween(const GridSettings settings) {
    pk.nxBetween = settings;
}
void WellManager::SetNyBetween(const GridSettings settings) {
    pk.nyBetween = settings;
}

std::vector<double> ParamKeeper::GetGridX() const {
    CHECK_OPT(wt);
    CHECK_OPT(drng);
    std::vector<double> ans;
    std::vector<double> left, halfwell, right;
    switch (*drng) {
    case DrainageArea::Rectangular:
        if (*wt == WellType::Fracture || *wt == WellType::Horizontal) {
            CHECK_OPT(nxLeft);
            CHECK_OPT(nxWell);
            CHECK_OPT(nxRight);
            left = LinLogGrid(0., xwd()-ld(), *nxLeft);
            halfwell = LinLogGrid(xwd()-ld(), xwd(), *nxWell);
            right = LinLogGrid(xwd() + ld(), xed(), *nxRight);
            std::copy(left.begin(), left.end(), std::back_inserter(ans));
            std::copy(halfwell.begin(), halfwell.end(), std::back_inserter(ans));
            halfwell = LinLogGrid(xwd(), xwd() + ld(), *nxWell);
            std::copy(halfwell.begin(), halfwell.end(), std::back_inserter(ans));
            std::copy(right.begin(), right.end(), std::back_inserter(ans));
        } else {
            throw std::logic_error("not implemented\n");
        }
        break;
    case DrainageArea::Circle:
        throw std::logic_error("not implemented\n");
    case DrainageArea::Infinite:
        throw std::logic_error("not implemented\n");
    default:
        throw std::invalid_argument("ParamKeeper::GetGridX: unknown drainage area\n");
    }
    return ans;
}
std::vector<double> ParamKeeper::GetGridY() const {
    CHECK_OPT(wt);
    CHECK_OPT(drng);
    std::vector<double> ans;
    std::vector<double> bottom, top;
    switch (*drng) {
    case DrainageArea::Rectangular:
        if (*wt == WellType::Fracture || *wt == WellType::Horizontal) {
             CHECK_OPT(nyBottom);
             CHECK_OPT(nyTop);
             bottom = LinLogGrid(0., ywd(), *nyBottom);
             top = LinLogGrid(ywd(), yed(), *nyTop);
             std::copy(bottom.begin(), bottom.end(), std::back_inserter(ans));
             std::copy(top.begin(), top.end(), std::back_inserter(ans));
        } else {
            throw std::logic_error("not implemented\n");
        }
        break;
    case DrainageArea::Circle:
        throw std::logic_error("not implemented\n");
    case DrainageArea::Infinite:
        throw std::logic_error("not implemented\n");
    default:
        throw std::invalid_argument("ParamKeeper::GetGridX: unknown drainage area\n");
    }
    return ans;
}
std::vector<double> ParamKeeper::GetGridZ() const {
    CHECK_OPT(wt);
    if (*wt == WellType::Fracture || *wt == WellType::Vertical || *wt == WellType::MultiFractured) {
        return {0.};
    } else {
        throw std::logic_error("not implemented\n");
    }
}

void ParamKeeper::TransformToDimentionGrid(QList<Matrix3DV>& gridList) {
    for (auto& grid: gridList) {
        for (auto& pt: grid) {
            pt.x *= LRef();
            pt.y *= LRef();
            if (*wt == WellType::Horizontal) pt.z *= (*h);
            double dimp = 18.42*(*qwell)*(*mu)*(*boil)/(*perm)/(*h);
            pt.val = (*pinit - dimp*pt.val);

        }
    }
}

