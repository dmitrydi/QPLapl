#ifndef PICMANAGER_H
#define PICMANAGER_H
#include <QObject>
#include <QLabel>
#include <QString>
#include <QPixmap>
#include <unordered_map>

struct imageSelector {
    QString drainageShape;
    QString wellType;
};

struct selectorHash {
    size_t operator()(const imageSelector& s) const {
        size_t h1(qHash(s.drainageShape));
        size_t h2(qHash(s.wellType));
        return h1 ^ (h2 + 0x9e3779b9 + (h1<<6) + (h1>>2));
    }
};

struct selectorEqual {
    bool operator() (const imageSelector& lhs, const imageSelector& rhs) const {
        return lhs.drainageShape == rhs.drainageShape && lhs.wellType == rhs.wellType;
    }
};

const std::unordered_map<imageSelector, QString, selectorHash, selectorEqual> selector_map = {
    {{"Rectangular", "Fracture"}, ":/Fractured.jpg"},
    {{"Rectangular", "Vertical"}, ":/Vertical.jpg"},
    {{"Rectangular", "Horizontal"}, ":/Horizontal.jpg"},
    {{"Rectangular", "Multifractured"}, ":/Multifractured.jpg"}
};

class PicManager: public QObject
{
    Q_OBJECT
public:
    explicit PicManager(QLabel* parent);
    const QPixmap& getPixmap();

private:
    QLabel *parent;
    QPixmap pixmap;
    imageSelector selector;


public slots:
    void setDrainageShape(const QString&);
    void setWellType(const QString&);

signals:
    void sendPixmap(const QPixmap&);
};

#endif // PICMANAGER_H
