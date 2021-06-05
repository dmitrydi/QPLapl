#include "wellcontroller.h"

WellController::WellController(QObject *parent) : QObject(parent)
{

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
    if (us_str.isEmpty())
        return;
    unitSystem = us_str;
}

void WellController::setCalcmode(const QString &cm_str)
{
    if (cm_str.isEmpty())
        return;
    calcMode = cm_str;
}

void WellController::setTimeSchedule(const std::vector<double> &ts_)
{
    ts.reserve(ts_.size());
    for (const auto& t: ts_) {
        if (t > 0.0)
            ts.push_back(t);
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

std::vector<double> WellController::LinLogGrid(double xmin, double xmax, WellController::GridSetup gSetup, double factor)
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


