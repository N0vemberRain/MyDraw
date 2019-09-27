#ifndef POINT_H
#define POINT_H

#include <QObject>
#include <QGraphicsItem>
#include <QPen>
#include <QGraphicsSceneMouseEvent>
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
    void setShear(const double horizontalShear, const double verticalShear) {
        mHorizontalShear = horizontalShear * 4.4;
        mVerticalShear = verticalShear * 6;

        updateTransform();
    }

    void setShearMouse(const double horizontalShear, const double verticalShear) {
        mHorizontalShear = horizontalShear;// / 7;// / 4.4;
        mVerticalShear = verticalShear;// / 7;// / 6;

        updateTransform();
    }
    void setMouseEvent(QGraphicsSceneMouseEvent *event) {
        if(event->type() == QEvent::GraphicsSceneMousePress) {
            mousePressEvent(event);
        }
        if(event->type() == QEvent::GraphicsSceneMouseMove) {
            mouseMoveEvent(event);
        }
    }

    QRectF* mapToPixels();
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    QRectF boundingRect() const {
        return mRect;
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

private:
    void updateTransform() {
        QTransform transform;
        transform.translate(mHorizontalShear, mVerticalShear);
        setTransform(transform);

        auto tmp = mRect;

        mRect.setCoords(tmp.topLeft().rx() + mHorizontalShear,
                        tmp.topLeft().ry() + mVerticalShear,
                        tmp.bottomRight().rx() + mHorizontalShear,
                        tmp.bottomRight().ry() + mVerticalShear);
        update();
    }

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

    double mHorizontalShear;
    double mVerticalShear;
    QPointF mPreviousPoint;
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
