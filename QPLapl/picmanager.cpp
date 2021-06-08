#include "picmanager.h"
#include <QDebug>

PicManager::PicManager(QLabel* parent): QObject(parent), parent(parent) {
}

const QPixmap& PicManager::getPixmap() {
    try {
    pixmap = QPixmap(selector_map.at(selector));
    } catch (std::out_of_range&) {
        qDebug() << "unknown selector: " << selector.drainageShape << " " << selector.wellType;
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
    qDebug() << "PicManager::setWellType called";
    selector.wellType = wtype;
    emit sendPixmap(getPixmap());
}
