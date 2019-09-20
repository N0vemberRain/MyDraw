#ifndef POINT_H
#define POINT_H

#include <QObject>
#include <QGraphicsItem>
#include "types.h"

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
