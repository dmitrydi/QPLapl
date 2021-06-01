#ifndef INTERFACEMAPS_H
#define INTERFACEMAPS_H

#include <QHash>
#include <QString>
#include <QList>

enum class VisibilityState {
    Visible,
    Invisible
};

namespace WellRegimes {
const QList<QString> WellRegimes = {"Constant Liquid Rate", "Constant Wellbore Pressure"};
namespace Maps {
const QHash<QString, VisibilityState> liqrateVisibility = {
    {"Constant Liquid Rate", VisibilityState::Visible},
    {"Constant Wellbore Pressure", VisibilityState::Invisible}
};

const QHash<QString, VisibilityState> wellpresVisibility = {
    {"Constant Liquid Rate", VisibilityState::Invisible},
    {"Constant Wellbore Pressure", VisibilityState::Visible}
};
}
}

namespace BoundaryTypes  {
const QList<QString> BoundaryTypes = {"Impearmable", "Constant Pressure"};
}

namespace WellTypes {
const QList<QString> WellTypes = {"Fractured", "Horizontal", "Multifractured", "Vertical"};

namespace Maps {
const QHash<QString, VisibilityState> xfVisibility = {
    {"Fractured", VisibilityState::Visible},
    {"Horizontal", VisibilityState::Invisible},
    {"Multifractured", VisibilityState::Visible},
    {"Vertical", VisibilityState::Invisible}
};
const QHash<QString, VisibilityState> fcdVisibility = {
    {"Fractured", VisibilityState::Visible},
    {"Horizontal", VisibilityState::Invisible},
    {"Multifractured", VisibilityState::Visible},
    {"Vertical", VisibilityState::Invisible}
};
const QHash<QString, VisibilityState> lhVisibility = {
    {"Fractured", VisibilityState::Invisible},
    {"Horizontal", VisibilityState::Visible},
    {"Multifractured", VisibilityState::Visible},
    {"Vertical", VisibilityState::Invisible}
};
const QHash<QString, VisibilityState> nfracVisibility = {
    {"Fractured", VisibilityState::Invisible},
    {"Horizontal", VisibilityState::Invisible},
    {"Multifractured", VisibilityState::Visible},
    {"Vertical", VisibilityState::Invisible}
};
const QHash<QString, VisibilityState> rwVisibility = {
    {"Fractured", VisibilityState::Invisible},
    {"Horizontal", VisibilityState::Visible},
    {"Multifractured", VisibilityState::Invisible},
    {"Vertical", VisibilityState::Visible}
};

const QHash<QString, VisibilityState> zwVisibility = {
    {"Fractured", VisibilityState::Invisible},
    {"Horizontal", VisibilityState::Visible},
    {"Multifractured", VisibilityState::Invisible},
    {"Vertical", VisibilityState::Invisible}
};
}
}

namespace AreaShapes {
const QList<QString> AreaShapes = {"Rectangular", "Infinite", "Cricled"};

namespace Maps {
const QHash<QString, VisibilityState> xeVisibility = {
    {"Rectangular", VisibilityState::Visible},
    {"Infinite", VisibilityState::Invisible},
    {"Cricled", VisibilityState::Invisible},
};
const QHash<QString, VisibilityState> xwVisibility = {
    {"Rectangular", VisibilityState::Visible},
    {"Infinite", VisibilityState::Invisible},
    {"Cricled", VisibilityState::Invisible},
};
const QHash<QString, VisibilityState> yeVisibility = {
    {"Rectangular", VisibilityState::Visible},
    {"Infinite", VisibilityState::Invisible},
    {"Cricled", VisibilityState::Invisible},
};
const QHash<QString, VisibilityState> ywVisibility = {
    {"Rectangular", VisibilityState::Visible},
    {"Infinite", VisibilityState::Invisible},
    {"Cricled", VisibilityState::Invisible},
};
const QHash<QString, VisibilityState> reVisibility = {
    {"Rectangular", VisibilityState::Invisible},
    {"Infinite", VisibilityState::Invisible},
    {"Cricled", VisibilityState::Visible},
};


}
}

namespace Units {
const QList<QString> Units = {"Oilfield", "US", "SI", "Custom"};

namespace Maps {
const QHash<QString, QString> Distance = {
    {"Oilfield", "m"},
    {"US", "ft"},
    {"SI", "m"}
};
const QHash<QString, QString> Pressure = {
    {"Oilfield", "atm"},
    {"US", "psi"},
    {"SI", "Pa"}
};
const QHash<QString, QString> LiquidRate = {
    {"Oilfield", "m3/day"},
    {"US", "scfd"},
    {"SI", "m3/s"}
};
const QHash<QString, QString> Count = {
    {"Oilfield", "pcs."},
    {"US", "pcs."},
    {"SI", "pcs."}
};
const QHash<QString, QString> Time = {
    {"Oilfield", "day"},
    {"US", "day"},
    {"SI", "s"}
};
const QHash<QString, QString> DimentionlessConductivity = {
    {"Oilfield", "dimentionless"},
    {"US", "dimentionless"},
    {"SI", "dimentionless"}
};
const QHash<QString, QString> Permeability = {
    {"Oilfield", "md"},
    {"US", "md"},
    {"SI", "m2"}
};
const QHash<QString, QString> PayZone = {
    {"Oilfield", "m"},
    {"US", "ft"},
    {"SI", "m"}
};
const QHash<QString, QString> Porosity = {
    {"Oilfield", "fraction"},
    {"US", "fraction"},
    {"SI", "fraction"}
};
const QHash<QString, QString> Viscosity = {
    {"Oilfield", "cP"},
    {"US", "cP"},
    {"SI", "Pa*s"}
};
const QHash<QString, QString> FVF = {
    {"Oilfield", "sm3/m3"},
    {"US", "scf/cf"},
    {"SI", "m3/m3"}
};
const QHash<QString, QString> Ct = {
    {"Oilfield", "1/atm"},
    {"US", "1/psi"},
    {"SI", "1/Pa"}
};
const QHash<QString, QString> Pinit = {
    {"Oilfield", "atm"},
    {"US", "psi"},
    {"SI", "Pa"}
};

} // Maps
} // Units


#endif // INTERFACEMAPS_H
