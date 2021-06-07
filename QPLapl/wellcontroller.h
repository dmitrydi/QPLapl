#ifndef WELLCONTROLLER_H
#define WELLCONTROLLER_H

#include <QObject>
#include <optional>
#include <QString>
#include <sstream>
#include <stdexcept>
#include "interfacemaps.h"
#include "gwell.h"
#include "qgrid1d.h"
#include <memory>
#include <QVector>

#define CH_OPT_MSG(opt_param, text) {          \
    if(!(opt_param)) {                      \
        throw std::logic_error(text);       \
    }                                       \
}

#define CH_OPT(opt) {                               \
    if (!opt) {                                      \
        std::ostringstream os;                         \
        os << #opt << " is not set\n";                 \
        throw std::logic_error(os.str());              \
    }                                                  \
}


class WellController : public QObject
{
    Q_OBJECT
public slots:
    void CalculatePQ();
    void CalculateGrid();
    void SavePQT();
    void SaveGrid();
public:
    explicit WellController(QObject *parent = nullptr);
    // setters
    void setXe(const QString& xe_str);
    void setXw(const QString& xw_str);
    void setYe(const QString& ye_str);
    void setYw(const QString& yw_str);
    void setZw(const QString& zw_str);
    void setRe(const QString& re_str);
    void setRw(const QString& rw_str);
    void setFcd(const QString& fcd_str);
    void setXf(const QString& xf_str);
    void setLh(const QString& lh_str);
    void setH(const QString& h_str);
    void setK(const QString& k_str);
    void setFi(const QString& fi_str);
    void setMu(const QString& mu_str);
    void setBoil(const QString& boil_str);
    void setCt(const QString& ct_str);
    void setPwell(const QString& pwell_str);
    void setQwell(const QString& qwell_str);
    void setPinit(const QString& pinit_str);
    void setNfrac(const QString& nfrac_str);
    void setWelltype(const QString& welltype_str);
    void setBoundaryConditions(const QString& bnd_str);
    void setAreaShape(const QString& ash_str);
    void setUnitsystem(const QString& us_str);
    void setCalcmode(const QString& cm_str);
    void setTimeSchedule(const std::vector<double>& ts);
    void setTimeShcheduleGrid(const std::vector<double>& ts);
    void setNLeft(const QString& n, const QString& gridType);
    void setNRight(const QString& n, const QString& gridType);
    void setNBottom(const QString& n, const QString& gridType);
    void setNTop(const QString& n, const QString& gridType);
    void setNWell(const QString& n, const QString& gridType);
    void setNzBottom(const QString& n, const QString& gridType);
    void setNzTop(const QString& n, const QString& gridType);
    void setNBetween(const QString& n, const QString& gridType);
    //getters
    double lref() const;
    double xed() const;
    double xwd() const;
    double yed() const;
    double ywd() const;
    double zwd() const;
    double red() const;
    double ld() const;
    double hd() const;
    double fcd() const;
    WellType welltype() const;
    Boundary boundary() const;
    DrainageArea areashape() const;
    const std::vector<double>& getTValues() const;
    const std::vector<double>& getPQValues() const;
    const std::vector<double>& getTValuesDimentionless() const;
    const std::vector<double>& getPQValuesDimentionless() const;
    const std::vector<double>& getTGrid() const;
    const std::vector<double>& getTGridDimentionless() const;
    const QList<Matrix3DV>& getGrid() const;
    const QList<Matrix3DV>& getGridDimentionless() const;
    static constexpr double LogGridFactor = 1.1;
private:
    enum class CalcMode {
        ConstP,
        ConstQ
    };
    const QHash<QString, CalcMode> mapStrCalcMode = {
        {"Constant Liquid Rate", CalcMode::ConstQ},
        {"Constant Wellbore Pressure", CalcMode::ConstP}
    };
    enum class UnitSystem {
        Oilfield,
        US,
        SI,
        Custom
    };
    const QHash<QString, UnitSystem> mapStrUnitSystem = {
        {"Oilfield", UnitSystem::Oilfield},
        {"US", UnitSystem::US},
        {"SI", UnitSystem::SI},
        {"Custom", UnitSystem::Custom}
    };
    std::vector<double> ts, ps, qs;
    std::vector<double> tds, pds, qds;
    std::vector<double> tsGrid;
    std::vector<double> tdsGrid;
    QVector<std::pair<double, double>> TP;
    QVector<std::pair<double, double>> TQ;
    QList<Matrix3DV> gridP;
    QList<Matrix3DV> gridPDimentionless;
    std::optional<double> xe, xw, ye, yw, zw, re, rw, Fcd, xf, lh, h;
    std::optional<double> perm, fi, mu, boil, ct;
    std::optional<double> pWell, qWell, pInit;
    std::optional<int> nFrac;
    std::optional<MultifracOrientation> mFracOrientation;
    std::optional<WellType> wellType;
    std::optional<Boundary> boundaryConditions;
    std::optional<DrainageArea> areaShape;
    std::optional<UnitSystem> unitSystem;
    std::optional<CalcMode> calcMode;
    double dimT() const;
    double dimP() const;
    double dimQ() const;
    std::unique_ptr<LaplWell> makeWell() const;
    std::vector<double> ConvertPd_P(const std::vector<double>& pds) const;
    std::vector<double> ConvertQd_Q(const std::vector<double>& qds) const;
    std::vector<double> ConvertTd_T(const std::vector<double>& tds) const;
    std::vector<double> ConvertP_Pd(const std::vector<double>& ps) const;
    std::vector<double> ConvertQ_Qd(const std::vector<double>& qs) const;
    std::vector<double> ConvertT_Td(const std::vector<double>& ts) const;
    std::vector<double> ConvertVector(const std::vector<double> & vec, double mult) const;
    QList<Matrix3DV> ConvertGrid(const QList<Matrix3DV>& grid, double xmult, double ymult, double zmult, double valmult) const;
    // grid objects
    enum class GridType {
        Lin,
        Log
    };
    enum class GrowthDir {
        MinToMax, // | |  |   |    |
        MaxToMin  // |    |   |  | |
    };
    struct GridSetup {
        WellController::GridType gType;
        int nSegments;
        std::optional<WellController::GrowthDir> logGrowth;
    };
    std::optional<GridSetup> gsLeft, gsWellLeft, gsWellRight, gsRight;
    std::optional<GridSetup> gsBottom, gsTop;
    std::optional<GridSetup> gszBottom, gszTop;
    std::optional<GridSetup> gsBetweenLeft, gsBetweenRight;
    std::vector<double> makeGrid(
            const std::vector<std::pair<double, double>>& gridpoints
          , const std::vector<const GridSetup*>& gridsetups
            ) const;
    std::vector<double> makeXGrid() const;
    std::vector<double> makeXGridFracture() const;
    std::vector<double> makeXGridHorizontal() const;
    std::vector<double> makeXGridMultifracture() const;
    std::vector<double> makeXGridVertical() const;
    std::vector<double> makeYGrid() const;
    std::vector<double> makeYGridFracture() const;
    std::vector<double> makeYGridHorizontal() const;
    std::vector<double> makeYGridMultifracture() const;
    std::vector<double> makeYGridVertical() const;
    std::vector<double> makeZGrid() const;
    std::vector<double> makeZGridFracture() const;
    std::vector<double> makeZGridHorizontal() const;
    std::vector<double> makeZGridMultifracture() const;
    std::vector<double> makeZGridVertical() const;
    // utility
    std::vector<double> LinLogGrid(double xmin, double xmax, GridSetup gSetup, double factor = 1.1) const;
    QVector<std::pair<double, double>> zipStdVectors(const std::vector<double>& vfirst, const std::vector<double>& vsecond);
signals:
    void TPQReady(std::pair<const std::vector<double>&, const std::vector<double>&>);
    void TPQDimentionlessReady(std::pair<const std::vector<double>&, const std::vector<double>&>);
    void TGridReady(const std::vector<double>&);
    void TGridDimentionlessReady(const std::vector<double>&);
    void GridReady(const QList<Matrix3DV>&);
    void GridDimentionlessReady(const QList<Matrix3DV>&);
    void GraphDataReady(const QVector<std::pair<double, double>>&);
};

#endif // WELLCONTROLLER_H
