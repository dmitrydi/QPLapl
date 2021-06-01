#include "abstractlineinput.h"
#include <QDebug>
#include <QLocale>

AbstractControlledHidable::AbstractControlledHidable(const AbstractControlledHidable *visibilityController,
                                                     const QHash<QString, VisibilityState> &mapVisibilityController,
                                                     QWidget *parent):
    QWidget(parent)
  , visibilityController(visibilityController)
  , mapVisibilityController(mapVisibilityController)
{
    if (visibilityController) {
        connect(visibilityController, &AbstractControlledHidable::VisibilityControllerChanged, this, &AbstractControlledHidable::onVisibilityControllerChanged);
    }
    if (mapVisibilityController.isEmpty()) {
        this->setVisible(true);
    } else {
        onVisibilityControllerChanged(visibilityController->CurrentText());
    }
}

void AbstractControlledHidable::AddVisibilityDependency(const QString &controllerState, VisibilityState visState)
{
    mapVisibilityController[controllerState] = visState;
}

void AbstractControlledHidable::SetVisibilityController(const AbstractControlledHidable *visibilityController_)
{
    if (visibilityController)
        disconnect(visibilityController);
    visibilityController = visibilityController_;
    connect(visibilityController, &AbstractControlledHidable::VisibilityControllerChanged, this, &AbstractControlledHidable::onVisibilityControllerChanged);
}

void AbstractControlledHidable::onVisibilityControllerChanged(const QString &value)
{
    if (mapVisibilityController.contains(value))
        this->setVisible(mapVisibilityController.value(value) == VisibilityState::Visible);
    else
        this->setVisible((true));
}


AbstractLineInput::AbstractLineInput(const QString& title,
                                     AbstractLineInput const *unitsController_,
                                     const QHash<QString, QString>& mapUnitsController,
                                     AbstractLineInput const *visibilityController_,
                                     const QHash<QString, VisibilityState>& mapVisibilityController,
                                     QWidget *parent) :
    AbstractControlledHidable(visibilityController_, mapVisibilityController, parent)
  , title_ptr(new QLabel(title))
  , units_ptr(new QLabel)
  , blankWidget(new QWidget)
  , default_units("")
  , mainLayout(new QHBoxLayout)
  , unitsController(unitsController_)
  , mapUnitsController(mapUnitsController)
{
    this->setLayout(mainLayout);
    mainLayout->addWidget(title_ptr);
    mainLayout->addWidget(blankWidget);
    mainLayout->addWidget(units_ptr);

    if (unitsController) {
        connect(unitsController, &AbstractLineInput::UnitsControllerChanged, this, &AbstractLineInput::onUnitsControllerChanged);
    }
    if (mapUnitsController.isEmpty()) {
        units_ptr->setText(default_units);
    } else {
        onUnitsControllerChanged(unitsController->CurrentText());
    }
}

void AbstractLineInput::SetUnitsController(const AbstractLineInput *unitsController_)
{
    if (unitsController)
        disconnect(unitsController);
    unitsController = unitsController_;
    connect(unitsController, &AbstractLineInput::UnitsControllerChanged, this, &AbstractLineInput::onUnitsControllerChanged);
}

void AbstractLineInput::SetDefaultUnits(const QString &defaultUnits)
{
    default_units = defaultUnits;
}

void AbstractLineInput::SetTitleMinWidth(int width)
{
    title_ptr->setMinimumWidth(width);
    title_ptr->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Preferred);
}

void AbstractLineInput::SetUnitsMinWidth(int width)
{
    units_ptr->setMinimumWidth(width);
    units_ptr->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
}

void AbstractLineInput::onUnitsControllerChanged(const QString &value)
{
    if (mapUnitsController.contains(value))
        units_ptr->setText(mapUnitsController.value(value));
    else
        units_ptr->setText(default_units);
}

void AbstractLineInput::onVisibilityControllerChanged(const QString &value)
{
    if (mapVisibilityController.contains(value))
        this->setVisible(mapVisibilityController.value(value) == VisibilityState::Visible);
    else
        this->setVisible((true));
}

void AbstractLineInput::AddUnitsDependency(const QString &controllerState, const QString unitsToShow)
{
    mapUnitsController[controllerState] = unitsToShow;
}

TextLineInput::TextLineInput(const QString& title,
                             AbstractLineInput const *unitsController_,
                             const QHash<QString, QString>& mapUnitsController,
                             AbstractLineInput const *visibilityController_,
                             const QHash<QString, VisibilityState>& mapVisibilityController,
                             QWidget *parent):
    AbstractLineInput(title, unitsController_, mapUnitsController ,visibilityController_, mapVisibilityController, parent)
   , line_edit(new QLineEdit)
   , validator(new QDoubleValidator(this))
{
    mainLayout->replaceWidget(blankWidget, line_edit);
    line_edit->setValidator(validator);
    validator->setBottom(0.0);
    validator->setLocale(QLocale::C);
    delete blankWidget;
    connect(line_edit, &QLineEdit::textChanged, this, &AbstractLineInput::UnitsControllerChanged);
    connect(line_edit, &QLineEdit::textChanged, this, &AbstractControlledHidable::VisibilityControllerChanged);
}

const QString TextLineInput::CurrentText() const
{
    return line_edit->text();
}

ComboLineinput::ComboLineinput(const QString& title,
                               AbstractLineInput const *unitsController_,
                               const QHash<QString, QString>& mapUnitsController,
                               AbstractLineInput const *visibilityController_,
                               const QHash<QString, VisibilityState>& mapVisibilityController,
                               QWidget *parent):
      AbstractLineInput(title, unitsController_, mapUnitsController ,visibilityController_, mapVisibilityController, parent)
  , combo_box(new QComboBox)
{
    mainLayout->replaceWidget(blankWidget, combo_box);
    delete blankWidget;
    connect(combo_box, &QComboBox::currentTextChanged, this, &AbstractLineInput::UnitsControllerChanged);
    connect(combo_box, &QComboBox::currentTextChanged, this, &AbstractControlledHidable::VisibilityControllerChanged);
}

const QString ComboLineinput::CurrentText() const
{
    return combo_box->currentText();
}

void ComboLineinput::AddComboItem(const QString & text)
{
    combo_box->addItem(text);
}

void ComboLineinput::AddComboItems(const QList<QString> & texts)
{
    combo_box->addItems(texts);
}


TexComboLineInput::TexComboLineInput(const QString &title,
                                     const QList<QString> &comboItems_,
                                     const AbstractLineInput *visibilityController,
                                     const QHash<QString, VisibilityState> &mapVisibilityController,
                                     QWidget *parent):
    AbstractControlledHidable(visibilityController, mapVisibilityController, parent)
  , title_ptr(new QLabel(title))
  , box_ptr(new QComboBox)
  , layout_ptr(new QHBoxLayout)
  , line_ptr(new QLineEdit)
{
    for (const auto& it: comboItems_) {
        comboItems.append(it);
    }
    this->setLayout(layout_ptr);
    layout_ptr->addWidget(title_ptr);
    layout_ptr->addWidget(line_ptr);
    layout_ptr->addWidget(box_ptr);
    box_ptr->addItems(comboItems);
    connect(line_ptr, &QLineEdit::textChanged, this, &AbstractControlledHidable::VisibilityControllerChanged);
}

const QString TexComboLineInput::CurrentText() const
{
    return line_ptr->text();
}

const QString TexComboLineInput::ComboText() const
{
    return box_ptr->currentText();
}

void TexComboLineInput::AddComboItem(const QString & item)
{
    comboItems.append(item);
    box_ptr->addItem(item);
}

void TexComboLineInput::SetValidator(const QValidator *validator)
{
    line_ptr->setValidator(validator);
}

void TexComboLineInput::SetTitleMinWidth(int width)
{
    title_ptr->setMinimumWidth(width);
    title_ptr->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Preferred);
}

void TexComboLineInput::SetUnitsMinWidth(int width)
{
    box_ptr->setMinimumWidth(width);
    box_ptr->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Preferred);
}
