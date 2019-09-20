#ifndef PIXMAP_H
#define PIXMAP_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

#include "types.h"
#include "line.h"
#include "circle.h"

class Dot : public QObject, public QGraphicsItem { //QGraphicsRectItem {
    Q_OBJECT
public:
    enum { Type = DotType };
    explicit Dot(const int x, const int y,
                 const double imgX, const double imgY);
    explicit Dot();
    explicit Dot(const int x, const int y);

    int type() const { return Type; }
    void setImgCoords(const double imgX, const double imgY);
    void setCoords(const int x, const int y);
    QRectF getRect();
    int getX() const { return _x; }
    int getY() const { return _y; }
    void check() { update(); }
   // void setState(const bool state) { _state = state; update(); }
protected:
    QRectF boundingRect() const {
        return mRect;
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

private:

    const double realWidth = 0.44;
    const double realHeight = 0.6;
    double width = 10 * realWidth;
    double height = 10 * realHeight;

    int _x;
    int _y;
    double _imgX;
    double _imgY;
    bool _state;
    QRectF mRect;
    QPen *mPen;
};

class PixMap : public QObject, public QGraphicsRectItem {
    Q_OBJECT
public:
    enum { Type = GridType };
    explicit PixMap();
    int type() const { return Type; }
    QPair<int, int> findDot(const QPointF &p);
    void start();
    void addPoint(const int x, const int y);
    void addLine(const QVector<QPair<int, int>> line);
    void setScene(QGraphicsScene *scene);
    void addObject(QObject *item);
    void check();

protected:
    QRectF boundingRect() const {
        return mRect;
        //return QRectF(m_rect.topLeft(), m_rect.bottomRight());
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

private:
    Dot pixels[128][64];
    void setNodes() {
        for(int i = 0; i <= 16; i++) {
            auto line = lines.at(i);
            int x, y, dx, dy;
            x = line.p1().x();
            y = line.p1().y();
            dx = 0; dy = 0;
            for(int j = 0; j <= 8; j++) {
                nodes.append(QPoint(x + dx, y + dy));
                dy += 8;
            }
            dx += 8;
        }
    }
    void createImageMap();

    QVector<QLineF> lines;
    QVector<QPoint> nodes;
    QVector<QObject*> items;
    QRect mRect;
    QPen   *mPen;
    QGraphicsScene *mScene;
    QRect imgmap;
};

class PixMap2 : public QObject, public QGraphicsItem {
    Q_OBJECT
public:
    enum { Type = GridType };
    explicit PixMap2();
    int type() const { return Type; }
    QPair<int, int> findDot(const QPointF &p);
    QRectF findPixRect(const QPointF &p);
    void start();
    void addPoint(const int x, const int y);
    void addLine(const QVector<QPair<int, int>> line);
    void setScene(QGraphicsScene *scene);
    void check();

protected:
    QRectF boundingRect() const {
        return mBoundingRect;
        //return QRectF(m_rect.topLeft(), m_rect.bottomRight());
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);


private:
    QRectF pixels[128][64];
    QVector<QRectF> rects;
    QVector<QPoint> nodes;
    QRectF mBoundingRect;
    QPen   *mPen;

};


#endif // PIXMAP_H
