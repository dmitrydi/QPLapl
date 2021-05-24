#ifndef UNICOMBOBOX_H
#define UNICOMBOBOX_H

#include <QObject>
#include <QWidget>
#include <QString>
#include <QDebug>
#include <QComboBox>

class UniComboBox: public QComboBox
{
    Q_OBJECT
public:
    UniComboBox(QWidget *parent = nullptr);
    QString text() const;

signals:
    void textChanged(const QString&);
};

#endif // UNICOMBOBOX_H
