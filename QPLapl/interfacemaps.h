#ifndef INTERFACEMAPS_H
#define INTERFACEMAPS_H

#include <QHash>
#include <QString>
#include <QList>

enum class VisibilityState {
    Visible,
    Invisible
};

namespace Units {
const QList<QString> Units = {"Oilfield", "US", "SI", "Custom"};

namespace Maps {
const QHash<QString, QString> Permeability = {
    {"Oilfield", "md"},
    {"US", "md"},
    {"SI", "m2"}
};

} // Maps
} // Units


#endif // INTERFACEMAPS_H
