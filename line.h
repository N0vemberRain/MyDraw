#ifndef LINE_H
#define LINE_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QCursor>

#include "types.h"
#include "point.h"

class LineDot : public QObject {
    Q_OBJECT
public:
    enum { Type = LineType };
    explicit LineDot(QObject *parent)
        : QObject(parent)/*, p1(0, 0), p2(0, 0)*/ {

    }
    explicit LineDot(const int px1, const int py1, const int px2, const int py2)
        : QObject(), x0(px1), y0(py1), x1(px2), y1(py2) {
        createLine2();
    }

    QVector<QPair<int, int>> getLine() const {
        return points;
    }
protected:
    /*void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override {
        for(const auto & it : points) {
            painter->drawRect(it);
        }
    }*/

private:
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
            points.append(qMakePair(x, y));

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
            points.append(qMakePair(comp ? y : x, comp ? x : y));
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
    QVector<QPair<int, int>> points;
};

class Line : public QObject, public QGraphicsItem {
    Q_OBJECT
public:
    enum { Type = LineType };
    //explicit Line(const QPointF &p1, const QPointF &p2);
    explicit Line(QObject *parent) : QObject(parent), QGraphicsItem() {
        this->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    }
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
            mouseMoveEvent(pe);
        }
        if(pe->type() == QEvent::GraphicsSceneMousePress) {
            mousePressEvent(pe);
        }
    }

    void setShear(const double horizontal, const double vertical);
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

    QLineF mLine;
    QPen *mPen;
    QPointF mPreviousPoint;

    double mHorizontalShear;
    double mVerticalShear;
};


#endif // LINE_H
