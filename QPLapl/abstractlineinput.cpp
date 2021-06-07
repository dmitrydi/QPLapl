#include "abstractlineinput.h"
#include <QDebug>
#include <QLocale>

AbstractControlledHidable::AbstractControlledHidable(const AbstractControlledHidable *visibilityController,
                                                     const QHash<QString, VisibilityState> &mapVisibilityController,
                                                     QWidget *parent):
    QWidget(parent)
  , mapVisibilityController(mapVisibilityController)
{
    if (visibilityController) {
        controllers.append(visibilityController);
        for (auto cnt: controllers)
            connect(cnt,
                    &AbstractControlledHidable::VisibilityControllerChanged,
                    this,
                    &AbstractControlledHidable::onVisibilityControllerChanged);
    }
    CheckCurrentState();
}

void AbstractControlledHidable::CheckCurrentState() {
    bool needToHide = false;
    for (auto cnt: controllers) {
        if (!mapVisibilityController.isEmpty()) {
            const auto& state = cnt->CurrentText();
            if (mapVisibilityController.contains(state) && (mapVisibilityController.value(state) == VisibilityState::Invisible)) {
                    needToHide = true;
                    break;
            }
        }
    }
    if (needToHide)
        this->hide();
}

void AbstractControlledHidable::AddVisibilityDependency(const QString &controllerState, VisibilityState visState)
{
    mapVisibilityController[controllerState] = visState;
    CheckCurrentState();
}

void AbstractControlledHidable::AddVisibilityMap(const QHash<QString, VisibilityState> &aMap)
{
    for (const auto& k: aMap.keys()) {
        mapVisibilityController[k] = aMap.value(k);
    }
    CheckCurrentState();
}

void AbstractControlledHidable::AddVisibilityController(const AbstractControlledHidable *controller)
{
    controllers.append(controller);
    connect(controller,
            &AbstractControlledHidable::VisibilityControllerChanged,
            this,
            &AbstractControlledHidable::onVisibilityControllerChanged);
    CheckCurrentState();
}

void AbstractControlledHidable::onVisibilityControllerChanged(const QString &value)
{
    if (mapVisibilityController.contains(value))
        this->setVisible(mapVisibilityController.value(value) == VisibilityState::Visible);
    else
        this->setVisible((true));
    CheckCurrentState();
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
    mainLayout->setContentsMargins(0,1,0,1);

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

int AbstractLineInput::GetTitleWidth() const
{
    return title_ptr->width();
}

int AbstractLineInput::GetUnitsWidth() const
{
    return units_ptr->width();
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
    connect(line_edit, &QLineEdit::textChanged, this, &AbstractLineInput::SendText);
}

const QString TextLineInput::CurrentText() const
{
    return line_edit->text();
}

void TextLineInput::SetDefaultValue(const QString &value)
{
    line_edit->setText(value);
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
    connect(combo_box, &QComboBox::currentTextChanged, this, &AbstractLineInput::SendText);
}

const QString ComboLineinput::CurrentText() const
{
    return combo_box->currentText();
}

void ComboLineinput::SetDefaultValue(const QString &value)
{
    combo_box->setCurrentIndex(0);
}

void ComboLineinput::AddComboItem(const QString & text)
{
    combo_box->addItem(text);
}

void ComboLineinput::AddComboItems(const QList<QString> & texts)
{
    combo_box->addItems(texts);
}


TextComboLineInput::TextComboLineInput(const QString &title,
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
    layout_ptr->setContentsMargins(0,0,0,0);
    layout_ptr->addWidget(title_ptr);
    layout_ptr->addWidget(line_ptr);
    layout_ptr->addWidget(box_ptr);
    box_ptr->addItems(comboItems);
    connect(line_ptr, &QLineEdit::textChanged, this, &AbstractControlledHidable::VisibilityControllerChanged);
}

const QString TextComboLineInput::CurrentText() const
{
    return line_ptr->text();
}

const QString TextComboLineInput::ComboText() const
{
    return box_ptr->currentText();
}

void TextComboLineInput::AddComboItem(const QString & item)
{
    comboItems.append(item);
    box_ptr->addItem(item);
}

void TextComboLineInput::SetValidator(const QValidator *validator)
{
    line_ptr->setValidator(validator);
}

void TextComboLineInput::SetTitleMinWidth(int width)
{
    title_ptr->setMinimumWidth(width);
    title_ptr->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Preferred);
}

void TextComboLineInput::SetUnitsMinWidth(int width)
{
    box_ptr->setMinimumWidth(width);
    box_ptr->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Preferred);
}

int TextComboLineInput::GetTitleWidth() const
{
    return title_ptr->width();
}

int TextComboLineInput::GetUnitsWidth() const
{
    return box_ptr->width();
}
