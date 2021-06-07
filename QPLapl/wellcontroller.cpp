#include "wellcontroller.h"
#include <QDebug>

WellController::WellController(QObject *parent) : QObject(parent)
{
}

std::unique_ptr<LaplWell> WellController::makeWell() const
{
    CH_OPT(wellType);
    CH_OPT(areaShape);
    switch (*areaShape) {
    case DrainageArea::Rectangular:
        switch (*wellType) {
        case WellType::Fracture:
            return std::make_unique<Rectangular::Fracture>(*boundaryConditions, xwd(), xed(), ywd(), yed(), fcd());
        default:
            throw std::logic_error("not implemented well type\n");
        }
    default:
        throw std::logic_error("not implemented area shape\n");
    }
}

void WellController::CalculatePQ()
{
    qDebug() << "Start CalculatePQ";
    CH_OPT(calcMode);
    std::unique_ptr<LaplWell> well = makeWell();
    qDebug() << "Make well OK";
    tds = ConvertT_Td(ts);
    qDebug() << "tds OK size " << tds;
    switch (*calcMode) {
    case CalcMode::ConstQ:
        pds.resize(tds.size());
        well->pwd_parallel(tds, pds, 1);
        qDebug() << "pds OK " << pds;
        ps = ConvertPd_P(pds);
        emit TPQReady(std::make_pair<const std::vector<double>&, const std::vector<double>&>(ts, ps));
        emit TPQDimentionlessReady(std::make_pair<const std::vector<double>&, const std::vector<double>&>(tds, pds));
        TP = zipStdVectors(ts, ps);
        emit GraphDataReady(TP);
        break;
    case CalcMode::ConstP:
        qds.resize(tds.size());
        well->qwd_parallel(tds, qds, 4);
        qs = ConvertQd_Q(qds);
        emit TPQReady(std::make_pair<const std::vector<double>&, const std::vector<double>&>(ts, qs));
        emit TPQDimentionlessReady(std::make_pair<const std::vector<double>&, const std::vector<double>&>(tds, qds));
        TQ = zipStdVectors(ts, qs);
        emit GraphDataReady(TQ);
        break;
    }
}

void WellController::CalculateGrid()
{
    qDebug() << "WellController::CalculateGrid() called";
    std::unique_ptr<LaplWell> well = makeWell();
    tdsGrid = ConvertT_Td(tsGrid);
    std::vector<double> xdGrid = makeXGrid();
    std::vector<double> ydGrid = makeYGrid();
    std::vector<double> zdGrid = makeZGrid();
    for (const auto t: tdsGrid) {
        auto m = well->pd_m_parallel(t, 4, xdGrid, ydGrid, zdGrid);
        gridPDimentionless.append(std::move(m));
    };
    gridP = ConvertGrid(gridPDimentionless, lref(), lref(), *h, dimP());
    emit TGridReady(tsGrid);
    emit TGridDimentionlessReady(tdsGrid);
    emit GridReady(gridP);
    emit GridDimentionlessReady(gridPDimentionless);
}

void WellController::SavePQT()
{
    throw std::logic_error("not implemented\n");
}

void WellController::SaveGrid()
{
    throw std::logic_error("not implemented\n");
}

void WellController::setXe(const QString &xe_str)
{
    if (xe_str.isEmpty())
        return;
    xe = xe_str.toDouble();
}

void WellController::setXw(const QString &xw_str)
{
    if (xw_str.isEmpty())
        return;
    xw = xw_str.toDouble();
}

void WellController::setYe(const QString &ye_str)
{
    if (ye_str.isEmpty())
        return;
    ye = ye_str.toDouble();
}

void WellController::setYw(const QString &yw_str)
{
    if (yw_str.isEmpty())
        return;
    yw = yw_str.toDouble();
}

void WellController::setZw(const QString &zw_str)
{
    if (zw_str.isEmpty())
        return;
    zw = zw_str.toDouble();
}

void WellController::setRe(const QString &re_str)
{
    if (re_str.isEmpty())
        return;
    re = re_str.toDouble();
}

void WellController::setRw(const QString &rw_str)
{
    if (rw_str.isEmpty())
        return;
    rw = rw_str.toDouble();
}

void WellController::setFcd(const QString &fcd_str)
{
    if (fcd_str.isEmpty())
        return;
    Fcd = fcd_str.toDouble();
}

void WellController::setXf(const QString &xf_str)
{
    if (xf_str.isEmpty())
        return;
    xf = xf_str.toDouble();
}

void WellController::setLh(const QString &lh_str)
{
    if (lh_str.isEmpty())
        return;
    lh = 0.5*lh_str.toDouble();
}

void WellController::setH(const QString &h_str)
{
    if (h_str.isEmpty())
        return;
    h = h_str.toDouble();
}

void WellController::setK(const QString &k_str)
{
    if (k_str.isEmpty())
        return;
    perm = k_str.toDouble();
}

void WellController::setFi(const QString &fi_str)
{
    if (fi_str.isEmpty())
        return;
    fi = fi_str.toDouble();
}

void WellController::setMu(const QString &mu_str)
{
    if (mu_str.isEmpty())
        return;
    mu = mu_str.toDouble();
}

void WellController::setBoil(const QString &boil_str)
{
    if (boil_str.isEmpty())
        return;
    boil = boil_str.toDouble();
}

void WellController::setCt(const QString &ct_str)
{
    if (ct_str.isEmpty())
        return;
    ct = ct_str.toDouble();
}

void WellController::setPwell(const QString &pwell_str)
{
    if (pwell_str.isEmpty())
        return;
    pWell = pwell_str.toDouble();
}

void WellController::setQwell(const QString &qwell_str)
{
    if (qwell_str.isEmpty())
        return;
    qWell = qwell_str.toDouble();
}

void WellController::setPinit(const QString &pinit_str)
{
    if (pinit_str.isEmpty())
        return;
    pInit = pinit_str.toDouble();
}

void WellController::setNfrac(const QString &nfrac_str)
{
    if (nfrac_str.isEmpty())
        return;
    nFrac = nfrac_str.toInt();
}

void WellController::setWelltype(const QString &welltype_str)
{
    if (welltype_str.isEmpty())
        return;
    if (welltype_str == "Fractured") {
        wellType = WellType::Fracture;
    } else if (welltype_str == "Horizontal") {
        wellType = WellType::Horizontal;
    } else if (welltype_str == "Multifractured") {
        wellType = WellType::MultiFractured;
    } else if (welltype_str == "Vertical") {
        wellType = WellType::Vertical;
    } else {
        throw std::invalid_argument("WellController::setWelltype: unknown well type\n");
    }
}

void WellController::setBoundaryConditions(const QString &bnd_str)
{
    if (bnd_str.isEmpty())
        return;
    if (bnd_str == "Impearmable") {
        boundaryConditions = Boundary::NNNN;
    } else if (bnd_str == "Constant Pressure") {
        boundaryConditions = Boundary::CCCC;
    } else {
        throw std::invalid_argument("WellController::setBoundaryConditions: unknown boundary type\n");
    }
}

void WellController::setAreaShape(const QString &ash_str)
{
    if (ash_str.isEmpty())
        return;
    if (ash_str == "Rectangular") {
        areaShape = DrainageArea::Rectangular;
    } else if (ash_str == "Infinite") {
        areaShape = DrainageArea::Infinite;
    } else if (ash_str == "Cricled") {
        areaShape = DrainageArea::Circle;
    } else {
        throw std::invalid_argument("WellController::setAreaShape: unknown area shape\n");
    }
}

void WellController::setUnitsystem(const QString &us_str)
{
    if (mapStrUnitSystem.contains(us_str))
        unitSystem = mapStrUnitSystem.value(us_str);
}

void WellController::setCalcmode(const QString &cm_str)
{
    if (mapStrCalcMode.contains(cm_str))
        calcMode = mapStrCalcMode.value(cm_str);
}

void WellController::setTimeSchedule(const std::vector<double> &ts_)
{
    ts.clear();
    ts.reserve(ts_.size());
    for (const auto& t: ts_) {
        if (t > 0.0)
            ts.push_back(t);
    }
}

void WellController::setTimeShcheduleGrid(const std::vector<double> &ts_)
{
    tsGrid.clear();
    tsGrid.reserve(ts_.size());
    for (const auto& t: ts_) {
        if (t>0.0)
            tsGrid.push_back(t);
    }
}

void WellController::setNLeft(const QString &n_str, const QString &gridType_str)
{
    if (n_str.isEmpty() || gridType_str.isEmpty())
        return;
    int n = n_str.toInt();
    Q_ASSERT(n>0);
    if (gridType_str == "Lin") {
        gsLeft = {GridType::Lin, n, std::nullopt};
    } else if (gridType_str == "Log") {
        gsLeft = {GridType::Log, n, GrowthDir::MaxToMin};
    } else {
        throw std::invalid_argument("WellController::setNLeft: unknown grid type\n");
    }
}

void WellController::setNRight(const QString &n_str, const QString &gridType_str)
{
    if (n_str.isEmpty() || gridType_str.isEmpty())
        return;
    int n = n_str.toInt();
    Q_ASSERT(n>0);
    if (gridType_str == "Lin") {
        gsRight = {GridType::Lin, n, std::nullopt};
    } else if (gridType_str == "Log") {
        gsRight = {GridType::Log, n, GrowthDir::MinToMax};
    } else {
        throw std::invalid_argument("WellController::setNRight: unknown grid type\n");
    }
}

void WellController::setNBottom(const QString &n_str, const QString &gridType_str)
{
    if (n_str.isEmpty() || gridType_str.isEmpty())
        return;
    int n = n_str.toInt();
    Q_ASSERT(n>0);
    if (gridType_str == "Lin") {
        gsBottom = {GridType::Lin, n, std::nullopt};
    } else if (gridType_str == "Log") {
        gsBottom = {GridType::Log, n, GrowthDir::MaxToMin};
    } else {
        throw std::invalid_argument("WellController::setNBottom: unknown grid type\n");
    }
}

void WellController::setNTop(const QString &n_str, const QString &gridType_str)
{
    if (n_str.isEmpty() || gridType_str.isEmpty())
        return;
    int n = n_str.toInt();
    Q_ASSERT(n>0);
    if (gridType_str == "Lin") {
        gsTop = {GridType::Lin, n, std::nullopt};
    } else if (gridType_str == "Log") {
        gsTop = {GridType::Log, n, GrowthDir::MinToMax};
    } else {
        throw std::invalid_argument("WellController::setNTop: unknown grid type\n");
    }
}

void WellController::setNWell(const QString &n_str, const QString &gridType_str)
{
    if (n_str.isEmpty() || gridType_str.isEmpty())
        return;
    int n = n_str.toInt();
    Q_ASSERT(n>0);
    if (gridType_str == "Lin") {
        gsWellLeft = {GridType::Lin, n, std::nullopt};
        gsWellRight = gsWellLeft;
    } else if (gridType_str == "Log") {
        gsWellLeft = {GridType::Log, n, GrowthDir::MinToMax};
        gsWellRight = {GridType::Log, n, GrowthDir::MaxToMin};
    } else {
        throw std::invalid_argument("WellController::setNWell: unknown grid type\n");
    }
}

void WellController::setNzBottom(const QString &n_str, const QString &gridType_str)
{
    if (n_str.isEmpty() || gridType_str.isEmpty())
        return;
    int n = n_str.toInt();
    Q_ASSERT(n>0);
    if (gridType_str == "Lin") {
        gszBottom = {GridType::Lin, n, std::nullopt};
    } else if (gridType_str == "Log") {
        gszBottom = {GridType::Log, n, GrowthDir::MaxToMin};
    } else {
        throw std::invalid_argument("WellController::setNzBottom: unknown grid type\n");
    }
}

void WellController::setNzTop(const QString &n_str, const QString &gridType_str)
{
    if (n_str.isEmpty() || gridType_str.isEmpty())
        return;
    int n = n_str.toInt();
    Q_ASSERT(n>0);
    if (gridType_str == "Lin") {
        gszTop = {GridType::Lin, n, std::nullopt};
    } else if (gridType_str == "Log") {
        gszTop = {GridType::Log, n, GrowthDir::MinToMax};
    } else {
        throw std::invalid_argument("WellController::setNzTop: unknown grid type\n");
    }
}

void WellController::setNBetween(const QString &n_str, const QString &gridType_str)
{
    if (n_str.isEmpty() || gridType_str.isEmpty())
        return;
    int n = n_str.toInt();
    Q_ASSERT(n>0);
    if (gridType_str == "Lin") {
        gsBetweenLeft = {GridType::Lin, n, std::nullopt};
        gsBetweenRight = gsBetweenLeft;
    } else if (gridType_str == "Log") {
        gsBetweenLeft = {GridType::Log, n, GrowthDir::MinToMax};
        gsBetweenRight = {GridType::Log, n, GrowthDir::MaxToMin};
    } else {
        throw std::invalid_argument("WellController::setNBetween: unknown grid type\n");
    }
}


double WellController::lref() const
{
    CH_OPT_MSG(wellType, "Well type is not set\n");
    switch (*wellType) {
    case WellType::Fracture:
        CH_OPT_MSG(xf, "Fracture length is not set\n");
        return *xf;
    case WellType::MultiFractured:
        CH_OPT_MSG(xf, "Fracture length is not set\n");
        return *xf;
    case WellType::Horizontal:
        CH_OPT_MSG(lh, "Horizontal well length is not set\n");
        return *lh;
    case WellType::Vertical:
        CH_OPT_MSG(rw, "Well radius is not set\n");
        return *rw;
    }
}

double WellController::xed() const
{
    CH_OPT_MSG(xe, "Xe is not set\n");
    return *xe/lref();
}

double WellController::xwd() const
{
    CH_OPT_MSG(xw, "Xw is not set\n");
    return *xw/lref();
}

double WellController::yed() const
{
    CH_OPT_MSG(ye, "Ye is not set\n");
    return *ye/lref();
}

double WellController::ywd() const
{
    CH_OPT_MSG(yw, "Yw is not set\n");
    return *yw/lref();
}

double WellController::zwd() const
{
    CH_OPT_MSG(h, "H is not set\n");
    CH_OPT_MSG(zw, "Zw is not set\n");
    CH_OPT_MSG(rw, "Rw is not set\n");
    return ((*zw)+(*rw))/(*h);
}

double WellController::red() const
{
    CH_OPT_MSG(re, "Re is not set\n");
    return *re/lref();
}

double WellController::ld() const
{
    return 1.;
}

double WellController::hd() const
{
    CH_OPT_MSG(h, "H is not set\n");
    return *h/lref();
}

double WellController::fcd() const
{
    CH_OPT_MSG(Fcd, "Fcd is not set\n");
    return *Fcd;
}

WellType WellController::welltype() const
{
    CH_OPT_MSG(wellType, "Welltype is not set\n");
    return *wellType;
}

Boundary WellController::boundary() const
{
    CH_OPT_MSG(boundaryConditions, "Boundary conditions is not set\n");
    return *boundaryConditions;
}

DrainageArea WellController::areashape() const
{
    CH_OPT_MSG(areaShape, "Area shape is not set\n");
    return *areaShape;
}

const std::vector<double> &WellController::getTValues() const
{
    return ts;
}

const std::vector<double> &WellController::getPQValues() const
{
    CH_OPT_MSG(calcMode, "Calc mode is not set\n");
    switch (*calcMode) {
    case CalcMode::ConstP:
        return qs;
    case CalcMode::ConstQ:
        return ps;
    }
}

const std::vector<double> &WellController::getTValuesDimentionless() const
{
    return tds;
}

const std::vector<double> &WellController::getPQValuesDimentionless() const
{
    CH_OPT_MSG(calcMode, "Calc mode is not set\n");
    switch (*calcMode) {
    case CalcMode::ConstP:
        return qds;
    case CalcMode::ConstQ:
        return pds;
    }
}

const std::vector<double> &WellController::getTGrid() const
{
    return tsGrid;
}

const std::vector<double> &WellController::getTGridDimentionless() const
{
    return tdsGrid;
}

const QList<Matrix3DV> &WellController::getGrid() const
{
    return gridP;
}

const QList<Matrix3DV> &WellController::getGridDimentionless() const
{
    return gridPDimentionless;
}

double WellController::dimT() const
{
    CH_OPT(unitSystem);
    CH_OPT(perm);
    CH_OPT(fi);
    CH_OPT(mu);
    CH_OPT(ct);
    switch (*unitSystem) {
    case UnitSystem::Oilfield:
        return 0.00036*(*perm)/((*fi)*(*mu)*(*ct)*lref()*lref());
    default:
        throw std::logic_error("unit system is not implemented\n");
    }
}

double WellController::dimP() const
{
    CH_OPT(unitSystem);
    CH_OPT(qWell);
    CH_OPT(mu);
    CH_OPT(boil);
    CH_OPT(perm);
    CH_OPT(h);
    switch (*unitSystem) {
    case UnitSystem::Oilfield:
        return 18.42*(*qWell)*(*mu)*(*boil)/(*perm)/(*h);
    default:
        throw std::logic_error("unit system is not implemented\n");
    }
}

double WellController::dimQ() const
{
    CH_OPT(unitSystem);
    CH_OPT(perm);
    CH_OPT(pInit);
    CH_OPT(pWell);
    CH_OPT(h);
    CH_OPT(mu);
    CH_OPT(boil);
    switch (*unitSystem) {
    case UnitSystem::Oilfield:
        return (*pInit - *pWell)*(*perm)*(*h)/(18.42*(*mu)*(*boil));
    default:
        throw std::logic_error("unit system is not implemented\n");
    }
}

std::vector<double> WellController::ConvertPd_P(const std::vector<double> &pds) const
{
    CH_OPT(pInit);
    auto ans = ConvertVector(pds, dimP());
    for (auto& p: ans)
        p = *pInit - p;
    return ans;
}

std::vector<double> WellController::ConvertQd_Q(const std::vector<double> &qds) const
{
    return ConvertVector(qds, dimQ());
}

std::vector<double> WellController::ConvertTd_T(const std::vector<double> &tds) const
{
    return ConvertVector(tds, 1./dimT());
}

std::vector<double> WellController::ConvertP_Pd(const std::vector<double> &ps) const
{
    std::vector<double> ans;
    ans.reserve(ps.size());
    for (const auto& p: ps) {
        ans.push_back(*pInit-p);
    }
    return ConvertVector(ans, 1./dimP());
}

std::vector<double> WellController::ConvertQ_Qd(const std::vector<double> &qs) const
{
    return ConvertVector(qs, 1./dimQ());
}

std::vector<double> WellController::ConvertT_Td(const std::vector<double> &ts) const
{
    return ConvertVector(ts, dimT());
}

std::vector<double> WellController::ConvertVector(const std::vector<double> &vec, double mult) const
{
    std::vector<double> ans;
    ans.reserve(vec.size());
    for (const auto& v: vec) {
        ans.push_back(mult*v);
    }
    return ans;
}

QList<Matrix3DV> WellController::ConvertGrid(const QList<Matrix3DV> &grid, double xmult, double ymult, double zmult, double valmult) const
{
    QList<Matrix3DV> ans;
    for (const auto& _: grid) {
        Matrix3DV m(_);
        for (auto it = m.begin(); it != m.end(); ++it) {
            it->x *= xmult;
            it->y *= ymult;
            it->z *= zmult;
            it->val *= valmult;
        }
        ans.append(std::move(m));
    }
    return ans;
}

std::vector<double> WellController::makeGrid(const std::vector<std::pair<double, double> > &gridpoints
                                           , const std::vector<const WellController::GridSetup *> &gridsetups) const
{
    std::vector<double> xgrid;
    Q_ASSERT(gridpoints.size() == gridsetups.size());
    for (size_t i = 0; i < gridpoints.size(); ++i) {
        auto g = LinLogGrid(gridpoints[i].first, gridpoints[i].second, *gridsetups[i], LogGridFactor);
        xgrid.insert(xgrid.end(), std::make_move_iterator(g.begin()), std::make_move_iterator(g.end()));
    }
    auto it = std::unique(xgrid.begin(), xgrid.end()); // remove duplicate points at ends of gridpoints[]
    xgrid.erase(it, xgrid.end());
    return xgrid;
}

std::vector<double> WellController::makeXGrid() const
{
    CH_OPT(wellType);
    switch (*wellType) {
    case WellType::Fracture:
        return makeXGridFracture();
    case WellType::Horizontal:
        return makeXGridHorizontal();
    case WellType::MultiFractured:
        return makeXGridMultifracture();
    case WellType::Vertical:
        return makeXGridVertical();
    default:
        throw std::invalid_argument("unknown well type\n");
    }
}

std::vector<double> WellController::makeXGridFracture() const
{
    CH_OPT(areaShape);
    std::vector<std::pair<double, double>> gridpoints;
    std::vector<const GridSetup*> gridsetups;
    switch (*areaShape) {
    case DrainageArea::Rectangular:
        CH_OPT(gsLeft);
        CH_OPT(gsWellLeft);
        CH_OPT(gsWellRight);
        CH_OPT(gsRight);
        gridpoints.push_back({0., xwd()-1.});
        gridpoints.push_back({xwd()-1., xwd()});
        gridpoints.push_back({xwd(), xwd()+1.});
        gridpoints.push_back({xwd()+1., xed()});
        gridsetups.push_back(&(*gsLeft));
        gridsetups.push_back(&(*gsWellLeft));
        gridsetups.push_back(&(*gsWellRight));
        gridsetups.push_back(&(*gsRight));
        return makeGrid(gridpoints, gridsetups);
    default:
        throw std::logic_error("area shape not implemented\n");
    }
}

std::vector<double> WellController::makeXGridHorizontal() const
{
    return WellController::makeXGridFracture();
}

std::vector<double> WellController::makeXGridMultifracture() const
{
    CH_OPT(areaShape);
    std::vector<std::pair<double, double>> gridpoints;
    std::vector<const GridSetup*> gridsetups;
    switch (*areaShape) {
    case DrainageArea::Rectangular:
        CH_OPT(gsLeft);
        CH_OPT(gsWellLeft);
        CH_OPT(gsWellRight);
        CH_OPT(gsRight);
        CH_OPT(gsBetweenLeft);
        CH_OPT(gsBetweenRight);
        CH_OPT(mFracOrientation);
        CH_OPT(nFrac);
        if (*mFracOrientation == MultifracOrientation::Normal) {
            return makeXGridFracture();
        } else {
            throw std::logic_error("parallel orientation not implemented\n");
        }
    default:
        throw std::logic_error("area shape not implemented\n");
    }
}

std::vector<double> WellController::makeXGridVertical() const
{
    CH_OPT(areaShape);
    std::vector<std::pair<double, double>> gridpoints;
    std::vector<const GridSetup*> gridsetups;
    switch (*areaShape) {
    case DrainageArea::Rectangular:
        CH_OPT(gsLeft);
        CH_OPT(gsRight);
        gridpoints.push_back({0., xwd()-1.});
        gridpoints.push_back({xwd()+1., xed()});
        gridsetups.push_back(&(*gsLeft));
        gridsetups.push_back(&(*gsRight));
        return makeGrid(gridpoints, gridsetups);
    default:
        throw std::logic_error("area shape not implemented\n");
    }
}

std::vector<double> WellController::makeYGrid() const
{
    CH_OPT(wellType);
    switch (*wellType) {
    case WellType::Fracture:
        return makeYGridFracture();
    case WellType::Horizontal:
        return makeYGridHorizontal();
    case WellType::MultiFractured:
        return makeYGridMultifracture();
    case WellType::Vertical:
        return makeYGridVertical();
    default:
        throw std::invalid_argument("unknown well type\n");
    }
}

std::vector<double> WellController::makeYGridFracture() const
{
    CH_OPT(areaShape);
    std::vector<std::pair<double, double>> gridpoints;
    std::vector<const GridSetup*> gridsetups;
    switch (*areaShape) {
    case DrainageArea::Rectangular:
        CH_OPT(gsBottom);
        CH_OPT(gsTop);
        gridpoints.push_back({0., ywd()});
        gridpoints.push_back({ywd(), yed()});
        gridsetups.push_back(&(*gsBottom));
        gridsetups.push_back(&(*gsTop));
        return makeGrid(gridpoints, gridsetups);
    default:
        throw std::logic_error("area shape not implemented\n");
    }
}

std::vector<double> WellController::makeYGridHorizontal() const
{
    return WellController::makeYGridFracture();
}

std::vector<double> WellController::makeYGridMultifracture() const
{
    std::vector<std::pair<double, double>> gridpoints;
    std::vector<const GridSetup*> gridsetups;
    CH_OPT(areaShape);
    if (*areaShape == DrainageArea::Rectangular) {
        CH_OPT(nFrac);
        CH_OPT(lh);
        CH_OPT(gsBottom);
        CH_OPT(gsTop);
        CH_OPT(gsBetweenLeft);
        CH_OPT(gsBetweenRight);
        if (*mFracOrientation == MultifracOrientation::Normal) {
            double lhd = *lh/lref();
            double dfracd2 = (*lh)/(*nFrac-1);
            double ydstart = ywd() - lhd;
            double ydcur = ydstart;
            double ydend = ywd() + lhd;
            gridpoints.push_back({0., ydstart});
            gridsetups.push_back(&(*gsBottom));
            for (int i = 0; i < 2*(*nFrac-1); ++i) {
                gridpoints.push_back({ydcur, ydcur+dfracd2});
                ydcur += dfracd2;
            }
            for (int i = 0; i < (*nFrac-1); ++i) {
                gridsetups.push_back(&(*gsBetweenLeft));
                gridsetups.push_back(&(*gsBetweenRight));
            }
            gridpoints.push_back({ydend, yed()});
            gridsetups.push_back(&(*gsTop));
            return makeGrid(gridpoints, gridsetups);
        } else {
            throw std::logic_error("parallel orientation is not implemented\n");
        }
    } else {
        throw std::logic_error("area shape is not implemented\n");
    }
}

std::vector<double> WellController::makeYGridVertical() const
{
    CH_OPT(areaShape);
    std::vector<std::pair<double, double>> gridpoints;
    std::vector<const GridSetup*> gridsetups;
    switch (*areaShape) {
    case DrainageArea::Rectangular:
        CH_OPT(gsBottom);
        CH_OPT(gsTop);
        gridpoints.push_back({0., ywd()-1});
        gridpoints.push_back({ywd()+1, yed()});
        gridsetups.push_back(&(*gsBottom));
        gridsetups.push_back(&(*gsTop));
        return makeGrid(gridpoints, gridsetups);
    default:
        throw std::logic_error("area shape not implemented\n");
    }
}

std::vector<double> WellController::makeZGrid() const
{
    CH_OPT(wellType);
    switch (*wellType) {
    case WellType::Fracture:
        return makeZGridFracture();
    case WellType::Horizontal:
        return makeZGridHorizontal();
    case WellType::MultiFractured:
        return makeZGridMultifracture();
    case WellType::Vertical:
        return makeZGridVertical();
    default:
        throw std::invalid_argument("unknown well type\n");
    }
}

std::vector<double> WellController::makeZGridFracture() const
{
    return {0.};
}

std::vector<double> WellController::makeZGridHorizontal() const
{
    std::vector<std::pair<double, double>> gridpoints;
    std::vector<const GridSetup*> gridsetups;
    CH_OPT(rw);
    CH_OPT(h);
    double rwd = (*rw)/(*h);
    gridpoints.push_back({0., zwd() - 2*rwd}); // zwd() = (zw+rw)/h;
    gridpoints.push_back({zwd(), 1.});
    gridsetups.push_back(&(*gszBottom));
    gridsetups.push_back(&(*gszTop));
    return makeGrid(gridpoints, gridsetups);
}

std::vector<double> WellController::makeZGridMultifracture() const
{
    return {0.};
}

std::vector<double> WellController::makeZGridVertical() const
{
    return {0.};
}

std::vector<double> WellController::LinLogGrid(double xmin, double xmax, WellController::GridSetup gSetup, double factor) const
{
    int nseg = gSetup.nSegments;
    if (gSetup.gType == GridType::Lin) {
        return LinSpaced(xmin, xmax, nseg + 1);
    } else {
        double dist, d1;
        dist = std::abs(xmax-xmin);
        if ( factor != 1.) {
            double fact_n = std::pow(factor, nseg);
            d1 = dist*(1. - factor)/(1. - fact_n);
        } else {
            d1 = dist/nseg;
        }
        std::vector<double> ans(nseg+1);
        if (*(gSetup.logGrowth) == GrowthDir::MaxToMin) {
            ans[0] = xmin;
            for (int i = 1; i <= nseg; ++i) {
                ans[i] = ans[i-1] + d1;
                d1 *= factor;
            }
        } else {
            ans[nseg] = xmax;
            for (int i = nseg - 1; i >= 0; --i) {
                ans[i] = ans[i+1] - d1;
                d1 *= factor;
            }
        }
        return ans;
    }
}

QVector<std::pair<double, double> > WellController::zipStdVectors(const std::vector<double> &vfirst, const std::vector<double> &vsecond)
{
    Q_ASSERT(vfirst.size() == vsecond.size());
    size_t n = vfirst.size();
    QVector<std::pair<double, double>> ans;
    ans.reserve(n);
    for (size_t i = 0; i < n; ++i) {
        ans.push_back({vfirst[i], vsecond[i]});
    }
    return ans;
}



