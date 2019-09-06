#ifndef POLYLINE_H
#define POLYLINE_H

#include <QObject>
#include <QGraphicsPathItem>
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QCursor>

#include "types.h"

class Polyline : public QObject, public QGraphicsItem {
    Q_OBJECT
public:
    enum { Type = PolylineType };
    explicit Polyline(const QVector<QPointF> &points);
    ~Polyline() {}

    int type() const { return Type; }
    void setMouseEvent(QGraphicsSceneMouseEvent *pe) {
        if(pe->type() == QEvent::GraphicsSceneMouseMove) {
            mouseMoveEvent(pe);
        }
        if(pe->type() == QEvent::GraphicsSceneMousePress) {
            mousePressEvent(pe);
        }
    }

    void setData(const QStringList &data);
    QStringList getData() const;
    QRectF getRect() const {
        return boundingRect();
    }
    void setShear(const double horizontal, const double vertical);

public:
    void select(bool state);

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
private:
    void updateTransform();
    QPainterPath mPoints;
    QVector<QLineF> mPolyline;
    QPen *mPen;

    QPointF mPreviousPoint;
    double mHorizontalShear;
    double mVerticalShear;
};














class MoveItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit MoveItem(QObject *parent = 0);
    ~MoveItem();

    void setup() {
        this->setPos(50, 50);
    }

    void setMouseEvent(QGraphicsSceneMouseEvent *event);
signals:

private:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

public slots:
};


#endif // POLYLINE_H
