#ifndef POINT_H
#define POINT_H

#include <QObject>
#include <QGraphicsItem>
#include <QPen>
#include "types.h"

class Dot : public QObject, public QGraphicsItem { //QGraphicsRectItem {
    Q_OBJECT
public:
    enum { Type = DotType };
    explicit Dot(const int x, const int y,
                 const double imgX, const double imgY);
    explicit Dot();
    explicit Dot(const int x, const int y);
    ~Dot() {
        delete mPen;
    }

    int type() const { return Type; }
    void setImgCoords(const double imgX, const double imgY);
    void setCoords(const int x, const int y);
    QRectF getRect() const;
    int getX() const { return _x; }
    int getY() const { return _y; }
    void check() {
        _x += 1;
        _y += 1;
        update();
    }
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

class Point : public QObject, public QGraphicsItem {
    Q_OBJECT
public:
    explicit Point(const QPointF &p);
    ~Point();

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget);

 /*   virtual void mousePressEvent(QGraphicsSceneMouseEvent* ре) {

    }*/

private:
    QPointF m_point;
    QPen *m_pen;
};

class Point2 : public QObject, public QGraphicsRectItem {
    Q_OBJECT
public:
    enum { Type = PointType };
    explicit Point2(const int x, const int y)
        : QObject(), QGraphicsRectItem(), _x(x), _y(y) {
        emit drawPoint(_x, _y);
    }


    QPair<int, int> getCoords() const {
        return qMakePair(_x, _y);
    }

signals:
    QPair<int, int> drawPoint(const int x, const int y);

private:
    int _x;
    int _y;
    //double _imgX;
    //double _imgY;
    QRectF mRect;
};

#endif // POINT_H
