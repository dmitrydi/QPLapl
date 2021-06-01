#ifndef ABSTRACTLINEINPUT_H
#define ABSTRACTLINEINPUT_H

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QHBoxLayout>
#include <QHash>
#include <QLineEdit>
#include <QComboBox>
#include <QDoubleValidator>
#include "interfacemaps.h"

class AbstractControlledHidable: public QWidget
{
    Q_OBJECT
public:
    AbstractControlledHidable(AbstractControlledHidable const *visibilityController = nullptr,
                         const QHash<QString, VisibilityState>& mapVisibilityController = {},
                         QWidget *parent = nullptr);
    void AddVisibilityDependency(const QString& controllerState, VisibilityState visState);
    void SetVisibilityController(AbstractControlledHidable const *visibilityController);
    virtual const QString CurrentText() const = 0;
    virtual ~AbstractControlledHidable() = default;
protected:
    AbstractControlledHidable const *visibilityController;
    QHash<QString, VisibilityState> mapVisibilityController;
public slots:
    void onVisibilityControllerChanged(const QString& value);
signals:
    void VisibilityControllerChanged(const QString&);

};

class AbstractLineInput : public AbstractControlledHidable
{
    Q_OBJECT
public:
    explicit AbstractLineInput(const QString& title,
                               AbstractLineInput const *unitsController = nullptr,
                               const QHash<QString,QString>& mapUnitsController = {},
                               AbstractLineInput const *visibilityController = nullptr,
                               const QHash<QString, VisibilityState>& mapVisibilityController = {},
                               QWidget *parent = nullptr);
    void SetUnitsController(AbstractLineInput const *unitsController);
    void SetDefaultUnits(const QString& defaultUnits);
    void SetTitleMinWidth(int width);
    void SetUnitsMinWidth(int width);
    virtual ~AbstractLineInput() = default;
public slots:
    void onUnitsControllerChanged(const QString& value);
    void onVisibilityControllerChanged(const QString& value);
protected:
    QLabel *title_ptr;
    QLabel *units_ptr;
    QWidget *blankWidget;
    QString default_units;
    QHBoxLayout *mainLayout;
    AbstractLineInput const *unitsController;
    QHash<QString, QString> mapUnitsController;
public:
    void AddUnitsDependency(const QString& controllerState, const QString unitsToShow);
signals:
    void SendText(const QString&);
    void UnitsControllerChanged(const QString&);
};


class TextLineInput: public AbstractLineInput {
    Q_OBJECT
public:
    TextLineInput(const QString& title,
                  AbstractLineInput const *unitsController = nullptr,
                  const QHash<QString,QString>& mapUnitsController = {},
                  AbstractLineInput const *visibilityController = nullptr,
                  const QHash<QString, VisibilityState>& mapVisibilityController = {},
                  QWidget *parent = nullptr);
    const QString CurrentText() const override;
    ~TextLineInput() = default;
private:
    QLineEdit *line_edit;
    QString init_units;
    QDoubleValidator *validator;
signals:
    void SendText(const QString&);
    void UnitsControllerChanged(const QString&);
    void VisibilityControllerChanged(const QString&);
};

class ComboLineinput: public AbstractLineInput {
    Q_OBJECT
public:
    ComboLineinput(const QString& title,
                   AbstractLineInput const *unitsController = nullptr,
                   const QHash<QString,QString>& mapUnitsController = {},
                   AbstractLineInput const *visibilityController = nullptr,
                   const QHash<QString, VisibilityState>& mapVisibilityController = {},
                   QWidget *parent = nullptr);
    const QString CurrentText() const override;
    void AddComboItem(const QString&);
    void AddComboItems(const QList<QString>&);
    ~ComboLineinput() = default;
private:
    QComboBox *combo_box;
signals:
    void SendText(const QString&);
    void UnitsControllerChanged(const QString&);
    void VisibilityControllerChanged(const QString&);
};

class TextComboLineInput: public AbstractControlledHidable {
    Q_OBJECT
public:
    TextComboLineInput(const QString& title,
                      const QList<QString>& comboItems = {},
                      AbstractLineInput const *visibilityController = nullptr,
                      const QHash<QString, VisibilityState>& mapVisibilityController = {},
                      QWidget *parent = nullptr);
    const QString CurrentText() const override;
    const QString ComboText() const;
    void AddComboItem(const QString&);
    void SetValidator(const QValidator* validator);
    void SetTitleMinWidth(int width);
    void SetUnitsMinWidth(int width);
private:
    QLabel *title_ptr;
    QList<QString> comboItems;
    QComboBox *box_ptr;
    QHBoxLayout *layout_ptr;
    QLineEdit *line_ptr;
};

#endif // ABSTRACTLINEINPUT_H
