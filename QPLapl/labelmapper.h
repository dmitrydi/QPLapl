#ifndef LABELMAPPER_H
#define LABELMAPPER_H

#include <QHash>
#include <QString>
#include <QStringList>
#include <QtCore/QtGlobal>
using qhash_result_t = size_t;

enum class PARAMS {
    K,
    H,
    PHI,
    MU,
    CT,
    BOIL,
    PINIT,
    FCD,
    XF,
    LH,
    NFRAC,
    RW,
    XE,
    XW,
    YE,
    YW,
    ZW,
    RE,
    LIQRATE,
    WELLPRES,
    TIME
};

enum class UNITSYSTEM {
    OILFIELD,
    US,
    SI
};

enum class WELLTYPE {
    FRACTURE,
    MULTIFRACTURED,
    VERTICAL,
    HORIZONTAL
};

enum class AREASHAPE {
    RECTANGULAR,
    INFINITE,
    CIRCLED
};

enum class BOUNDARYCOND {
    INMPEARMABLE,
    CONSTANTPRES
};

enum class CALCMODE {
    LIQCONST,
    PRESCONST
};

inline uint qHash(const PARAMS key, uint seed)
{
    return qHash(static_cast<int>(key), seed);
}

inline uint qHash(const UNITSYSTEM key, uint seed) {
    return qHash(static_cast<int>(key), seed);
}

const QHash<QString, UNITSYSTEM> mapStrToUnitSystem = {
    {"Oilfield", UNITSYSTEM::OILFIELD},
    {"US", UNITSYSTEM::US},
    {"SI", UNITSYSTEM::SI}
};

const QHash<QString, WELLTYPE> mapStrToWellType = {
    {"Fracture", WELLTYPE::FRACTURE},
    {"Multifractured", WELLTYPE::MULTIFRACTURED},
    {"Vertical", WELLTYPE::VERTICAL},
    {"Horizontal", WELLTYPE::HORIZONTAL}
};

const QHash<QString, AREASHAPE> mapStrToAreaShape = {
    {"Rectangular", AREASHAPE::RECTANGULAR},
    {"Infinite", AREASHAPE::INFINITE},
    {"Circled", AREASHAPE::CIRCLED}
};

const QHash<QString, BOUNDARYCOND> mapStrToBoundaryCond = {
    {"Impearmable", BOUNDARYCOND::INMPEARMABLE},
    {"Constant Pressure", BOUNDARYCOND::CONSTANTPRES}
};

const QHash<QString, CALCMODE> mapStrToCalcMode = {
   {"Constant Liquid Rate", CALCMODE::LIQCONST},
    {"Constant Wellbore Pressure", CALCMODE::PRESCONST}
};

const QStringList unitSystemsList = {"Oilfield", "US", "SI"};
const QStringList wellTypeList = {"Fracture", "Multifractured", "Vertical", "Horizontal"};
const QStringList areaShapeList = {"Rectangular", "Infinite", "Circled"};
const QStringList boundaryConditionsList = {"Impearmable", "Constant Pressure"};
const QStringList calcModeList = {"Constant Liquid Rate", "Constant Wellbore Pressure"};

struct _index {
    int row;
    int col;
};

const _index uComboWellTypeIndex = {0, 1};
const _index uComboAreaShapeIndex = {0, 1};
const _index uComboBoundaryCondIndex = {7, 1};
const _index uComboCalcModeIndex = {0, 1};

typedef QHash<PARAMS, QString> UnitsMap;
typedef QHash<PARAMS, QString> OilfieldUnitsMap;
typedef QHash<PARAMS, QString> USUnitsMap;
typedef QHash<PARAMS, QString> SIUnitsMap;

const OilfieldUnitsMap oilfiledUnitsMapper = {
    {PARAMS::K, "md"},
    {PARAMS::H, "m"},
    {PARAMS::PHI, "fraction"},
    {PARAMS::MU, "cP"},
    {PARAMS::CT, "1/atm"},
    {PARAMS::BOIL, "m3/m3"},
    {PARAMS::PINIT, "atm"},
    {PARAMS::FCD, "dimentionless"},
    {PARAMS::XF, "m"},
    {PARAMS::LH, "m"},
    {PARAMS::NFRAC, "number"},
    {PARAMS::RW, "m"},
    {PARAMS::XE, "m"},
    {PARAMS::XW, "m"},
    {PARAMS::YE, "m"},
    {PARAMS::YW, "m"},
    {PARAMS::ZW, "m"},
    {PARAMS::RE, "m"},
    {PARAMS::LIQRATE, "m3/day"},
    {PARAMS::WELLPRES, "atm"},
    {PARAMS::TIME, "hr"}
};

const USUnitsMap usUnitsMapper = {
    {PARAMS::K, "md"},
    {PARAMS::H, "ft"},
    {PARAMS::PHI, "fraction"},
    {PARAMS::MU, "cP"},
    {PARAMS::CT, "1/psi"},
    {PARAMS::BOIL, "scf/scf"},
    {PARAMS::PINIT, "psi"},
    {PARAMS::FCD, "dimentionless"},
    {PARAMS::XF, "ft"},
    {PARAMS::LH, "ft"},
    {PARAMS::NFRAC, "number"},
    {PARAMS::RW, "ft"},
    {PARAMS::XE, "ft"},
    {PARAMS::XW, "ft"},
    {PARAMS::YE, "ft"},
    {PARAMS::YW, "ft"},
    {PARAMS::ZW, "ft"},
    {PARAMS::RE, "ft"},
    {PARAMS::LIQRATE, "scf/day"},
    {PARAMS::WELLPRES, "psi"},
    {PARAMS::TIME, "hr"}
};

const SIUnitsMap siUnitsMapper = {
    {PARAMS::K, "m2"},
    {PARAMS::H, "m"},
    {PARAMS::PHI, "fraction"},
    {PARAMS::MU, "Pa*s"},
    {PARAMS::CT, "1/Pa"},
    {PARAMS::BOIL, "m3/m3"},
    {PARAMS::PINIT, "Pa"},
    {PARAMS::FCD, "dimentionless"},
    {PARAMS::XF, "m"},
    {PARAMS::LH, "m"},
    {PARAMS::NFRAC, "number"},
    {PARAMS::RW, "m"},
    {PARAMS::XE, "m"},
    {PARAMS::XW, "m"},
    {PARAMS::YE, "m"},
    {PARAMS::YW, "m"},
    {PARAMS::ZW, "m"},
    {PARAMS::RE, "m"},
    {PARAMS::LIQRATE, "m3/s"},
    {PARAMS::WELLPRES, "Pa"},
    {PARAMS::TIME, "s"}
};

const QHash<UNITSYSTEM, UnitsMap> unitsMapper = {
    {UNITSYSTEM::OILFIELD, oilfiledUnitsMapper},
    {UNITSYSTEM::US, usUnitsMapper},
    {UNITSYSTEM::SI, siUnitsMapper}
};

const int tableScheduleModeInitRows = 1;
const int tableScheduleModelColumns = 3;


const QHash<QString, int> strToTableScheduleColumn = {
   {"Constant Liquid Rate", 1},
    {"Constant Wellbore Pressure", 2}
};

#endif // LABELMAPPER_H
