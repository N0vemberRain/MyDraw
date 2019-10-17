#ifndef LINE_H
#define LINE_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QCursor>

#include "types.h"
#include "point.h"


class LineDot : public QObject, public QGraphicsItem {
    Q_OBJECT
public:
    enum { Type = LineType };
    explicit LineDot(QObject *parent)
        : QObject(parent)/*, p1(0, 0), p2(0, 0)*/ {
        setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);

    }
    explicit LineDot(const int px1, const int py1, const int px2, const int py2)
        : QObject(nullptr), QGraphicsItem(), x0(px1), y0(py1), x1(px2), y1(py2) {
        createLine2();
        setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    }

    ~LineDot() {

    }

    QVector<QPair<int, int>> getLine() const {
        return coords;
    }

    QRectF getRect() const { return boundingRect(); }
protected:
    QRectF boundingRect() const {
        auto begin = points.at(0);
        auto end = points.last();

        if(begin->getX() < end->getX()) {
            if(begin->getY() < end->getY()) {
                return QRectF(begin->getRect().topLeft(), end->getRect().bottomRight());
            } else {
                return QRectF(begin->getRect().bottomLeft(), end->getRect().topRight());
            }
        } else {
            if(begin->getY() < end->getY()) {
                return QRectF(begin->getRect().bottomRight(), end->getRect().topLeft());
            } else {
                return QRectF(begin->getRect().topRight(), end->getRect().bottomLeft());
            }
        }

    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) {
        if(isSelected()) {
            painter->setPen(QPen(Qt::blue, 1));
        } else {
            painter->setPen(QPen(Qt::black, 1));
        }
        painter->setBrush(Qt::black);
        for(const auto & it : points) {
            painter->drawRect(it->getRect());
            Q_UNUSED(option);
            Q_UNUSED(widget);
        }
    }

private:
    void createDot(const int x, const int y) {
        auto imgX = x * 4.4;
        auto imgY = y * 6;
        Dot *dot = new Dot(x, y, imgX, imgY);
        points.append(dot);
    }
    void createLine() {
        int deltaX = abs(x1 - x0);
        int deltaY = abs(y1 - y0);
        int error = 0;
        int deltaError = deltaY;
        int y = y0;
        int diry = y1 - y0;

        if(diry > 0) {
            diry = 1;
        }
        if(diry < 0) {
            diry = -1;
        }
        for(int x = x0; x <= x1; x++) {
            // p(x, y);
            coords.append(qMakePair(x, y));
            createDot(x, y);
            error += deltaError;
            if((2 * deltaError) >= deltaX) {
                y += diry;
                error -= deltaX;
            }
        }
    }
    void createLine2() {
        int deltaX = abs(x1 - x0);
        int deltaY = abs(y1 - y0);
        bool comp = deltaY > deltaX;
        if(comp) {
            swap(x0, y0);
            swap(x1 , y1);
        }
        if(x0 > x1) {
            swap(x0, x1);
            swap(y0, y1);
        }
        int error = deltaX / 2;
        int diry = (y0 < y1) ? 1 : -1;
        int y = y0;
        for(int x = x0; x <= x1; x++) {
            coords.append(qMakePair(comp ? y : x, comp ? x : y));
            createDot(x, y);
            error -= deltaY;
            if (error < 0) {
                y += diry;
                error += deltaX;
            }
        }
    }

    void swap(int &a, int &b) {
        int tmp = a;
        a = b;
        b = tmp;
    }
    //Point2 p1;
    //Point2 p2;
    int x0, y0, x1, y1;
    QVector<QPair<int, int>> coords;
    QVector<Dot*> points;

};

class Line : public QObject, public QGraphicsItem {
    Q_OBJECT
public:
    enum { Type = LineType };
    explicit Line(QObject *parent = nullptr);
    explicit Line(const QLineF &line);
    explicit Line(const QPointF &p1, const QPointF &p2);
    ~Line() override;

    int type() const override { return Type; }

    void setData(const QStringList &data);
    QStringList getData() const;
    QRectF getRect() const {
        return boundingRect();
    }

    void setMouseEvent(QGraphicsSceneMouseEvent *pe) {
        if(pe->type() == QEvent::GraphicsSceneMouseMove) {
           // mouseMoveEvent(pe);
        }
        if(pe->type() == QEvent::GraphicsSceneMousePress) {
            mousePressEvent(pe);
        }
    }

    void setShear(const double horizontal, const double vertical);
    void setBegin(const QPointF &p) {
        p1 = p;
        update();
    }
    void setEnd(const QPointF &p) {
        p2 = p;
        mLine.setPoints(p1, p2);
    }
    QPointF getBegin() const { return mLine.p1(); }
    QPointF getEnd() const { return mLine.p2(); }

    void setHover(QGraphicsSceneHoverEvent *event) {
        hoverEnterEvent(event);
    }
public slots:
    void select(bool state);
protected:
    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget) override;



    void updateTransform();
private:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    QLineF mLine;
    QPen *mPen;
    QPointF mPreviousPoint;

    double mHorizontalShear;
    double mVerticalShear;
    
    QPointF p1;
    QPointF p2;
    bool hovered;
};


#endif // LINE_H
