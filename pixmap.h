#ifndef PIXMAP_H
#define PIXMAP_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

#include "types.h"
#include "line.h"
#include "circle.h"
#include "point.h"

class CheckRect : public QObject, public QGraphicsItem {
    Q_OBJECT
public:
    enum {Type = DotType };
    explicit CheckRect(QObject *parent = nullptr)
        : QObject(parent), QGraphicsItem() {
        mRect.setTopLeft(QPoint(0, 0));
        mRect.setBottomRight(QPoint(50, 50));
    }
    explicit CheckRect(const int x, const int y, const double imgX, const double imgY)
        : QObject(nullptr), QGraphicsItem(), _x(x), _y(y), _imgX(imgX), _imgY(imgY) {
        mRect.setTopLeft(QPointF(_imgX, _imgY));
        mRect.setBottomRight(QPointF(_imgX + 4.4, _imgY + 6));
    }
    ~CheckRect() {

    }

    QRectF getRect() const {
        return mRect;
    }
protected:
    QRectF boundingRect() const {
        return mRect;
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) {
        painter->setPen(QPen(Qt::red, 2));
        painter->drawRect(mRect);
        Q_UNUSED(option);
        Q_UNUSED(widget);
    }

private:
    QRectF mRect;
    int _x;
    int _y;
    double _imgX;//10 * realWidth;
    double _imgY;//10 * realHeight;

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
