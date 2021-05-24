#include "unicombobox.h"

UniComboBox::UniComboBox(QWidget *parent): QComboBox(parent)
{
    connect(this, SIGNAL(currentTextChanged(const QString&)),
            this, SIGNAL(textChanged(const QString&)));
}

QString UniComboBox::text() const {
    return QComboBox::currentText();
}
