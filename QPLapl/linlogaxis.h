#ifndef LINLOGAXIS_H
#define LINLOGAXIS_H

#include <QtCharts/QAbstractAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLogValueAxis>
#include <QWidget>
#include <QObject>

enum class CustomAxisType {
    Log,
    Lin
};

class LinLogAxis: public QObject
{
    Q_OBJECT
public:
    LinLogAxis(CustomAxisType t = CustomAxisType::Lin, QObject *parent = nullptr);
    QtCharts::QAbstractAxis* get();
private:
    CustomAxisType axisType;
    QtCharts::QValueAxis* linAxis;
    QtCharts::QLogValueAxis* logAxis;
};

#endif // LINLOGAXIS_H
