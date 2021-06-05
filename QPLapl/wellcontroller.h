#ifndef WELLCONTROLLER_H
#define WELLCONTROLLER_H

#include <QObject>
#include <optional>
#include <QString>
#include "interfacemaps.h"
#include "gwell.h"
#include "qgrid1d.h"

class WellController : public QObject
{
    Q_OBJECT
public:
    explicit WellController(QObject *parent = nullptr);
    // main functions
    void CalculatePQ();
    void CalculateGrid();
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
    void setNLeft(const QString& n, const QString& gridType);
    void setNRight(const QString& n, const QString& gridType);
    void setNBottom(const QString& n, const QString& gridType);
    void setNTop(const QString& n, const QString& gridType);
    void setNWell(const QString& n, const QString& gridType);
    void setNzBottom(const QString& n, const QString& gridType);
    void setNzTop(const QString& n, const QString& gridType);
    void setNBetween(const QString& n, const QString& gridType);
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
    double lref() const;
    //getters
    const std::vector<double>& getTValues() const;
    const std::vector<double>& getPQValues() const;
    const std::vector<double>& getTValuesDimentionless() const;
    const std::vector<double>& getPQValuesDimentionless() const;
    const std::vector<double>& getTGrid() const;
    const std::vector<double>& getTGridDimentionless() const;
    QList<Matrix3DV>& getGrid() const;
    QList<Matrix3DV>& getGridDimentionless() const;
private:
    std::vector<double> ts, ps, qs;
    std::vector<double> tds, pds, qds;
    std::vector<double> tsGrid;
    std::vector<double> tdsGrid;
    QList<Matrix3DV> gridP;
    QList<Matrix3DV> gridPDimentionless;
    std::optional<double> xe, xw, ye, yw, zw, re, rw, Fcd, xf, lh, h;
    std::optional<double> perm, fi, mu, boil, ct;
    std::optional<double> pWell, qWell, pInit;
    std::optional<int> nFrac;
    std::optional<WellType> wellType;
    std::optional<Boundary> boundaryConditions;
    std::optional<DrainageArea> areaShape;
    std::optional<QString> unitSystem;
    std::optional<QString> calcMode;
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
    std::vector<double> LinLogGrid(double xmin, double xmax, GridSetup gSetup, double factor = 1.1);
signals:

};

#endif // WELLCONTROLLER_H
