#include "picmanager.h"

PicManager::PicManager(QLabel* parent): parent(parent) {
}

const QPixmap& PicManager::getPixmap() {
    try {
    pixmap = QPixmap(selector_map.at(selector));
    } catch (std::out_of_range&) {
        pixmap = QPixmap(":/Blank.JPG");
    }
    QSize labelSize = parent->size();
    pixmap = pixmap.scaled(labelSize, Qt::KeepAspectRatio);
    return pixmap;
}

void PicManager::setDrainageShape(const QString& shape) {
    selector.drainageShape = shape;
    emit sendPixmap(getPixmap());
}
void PicManager::setWellType(const QString& wtype) {
    selector.wellType = wtype;
    emit sendPixmap(getPixmap());
}
