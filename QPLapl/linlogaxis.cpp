#include "linlogaxis.h"

using namespace QtCharts;

LinLogAxis::LinLogAxis(CustomAxisType t, QObject *parent):
    QObject(parent)
  , axisType(t)
  , linAxis(new QValueAxis(this))
  , logAxis(new QLogValueAxis(this))
{

}


QAbstractAxis* LinLogAxis::get() {
    if (axisType == CustomAxisType::Lin) {
        return linAxis;
    } else if (axisType == CustomAxisType::Log) {
        return logAxis;
    } else {
        return nullptr;
    }
}
