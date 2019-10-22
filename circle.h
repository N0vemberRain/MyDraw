#ifndef CIRCLE_H
#define CIRCLE_H

#include <QObject>
#include <QGraphicsEllipseItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QCursor>

#include "types.h"

class CircleDot : QObject {
    Q_OBJECT
public:
    enum { Type = CircleType };
    explicit CircleDot(QObject *parent) : QObject(parent), x0(0), y0(0), radius(0) {

    }

    explicit CircleDot(const int x, const int y, const int r)
        : QObject(), x0(x), y0(y), radius(r) {
        createCircle();
    }
    int type() const { return Type; }

    QVector<QPair<int, int>> getPoints() const {
        return points;
    }
private:
    void createCircle() {
        int x = radius;
        int y = 0;
        int radiusError = 1 - x;
        while (x >= y) {
             points.append(qMakePair(x + x0, y + y0));
             points.append(qMakePair(y + x0, x + y0));
             points.append(qMakePair(-x + x0, y + y0));
             points.append(qMakePair(-y + x0, x + y0));
             points.append(qMakePair(-x + x0, -y + y0));
             points.append(qMakePair(-y + x0, -x + y0));
             points.append(qMakePair(x + x0, -y + y0));
             points.append(qMakePair(y + x0, -x + y0));

            y++;
            if(radiusError < 0) {
                radiusError += 2 * y + 1;
            } else {
                x--;
                radiusError += 2 * (y - x + 1);
            }
        }
    }

    int x0;
    int y0;
    int radius;
    QVector<QPair<int, int>> points;
};

class Circle : public QObject, public QGraphicsEllipseItem {
    Q_OBJECT
public:
    enum { Type = CircleType };
    explicit Circle(const QPointF &origin, const double radius);
    explicit Circle(const QPointF &origin, const QPointF radius);
    ~Circle() {}

    int type() const override { return Type; }
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget);

private:
    QRectF makeCircle();
    QPointF m_origin;
    QPointF m_rPoint;
    double m_radius;
    QPen *m_pen;
    //QGraphicsEllipseItem *ellipse;
};

class CircleItem : public QObject, public QGraphicsItem {
    Q_OBJECT
public:
    enum { Type = CircleType };
    explicit CircleItem(QObject *parent = nullptr)
        : QObject(parent), QGraphicsItem() {
        this->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
        //setSelected(true);
    }
    explicit CircleItem(const QPointF &origin, const double radius);
    explicit CircleItem(const QPointF &origin, const QPointF &radius);
    ~CircleItem() {

    }

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
    void setShear(const double horizontal, const double vertical);
    void setOrigin(const QPointF &o) {
        mOrigin = o;
    }

    void setRadius(const QPointF &rp) {
        mRPoint = rp;
    }

    void setState(const ItemState state) { mState = state; }
    void setHoverEvent(QGraphicsSceneHoverEvent *event) {
        if(boundingRect().contains(event->scenePos())) {
            hoverEnterEvent(event);
        } else {
            hoverLeaveEvent(event);
        }
    }
public slots:
    void select(bool state);

signals:
    
protected:
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    void paintNormal(QPainter *painter);
    void paintRendering(QPainter *painter);
    void paintFocus(QPainter *painter);
    void paintEdit(QPainter *painter);
    void updateTransform();

    QRectF boundingRect() const {
        return makeCircle(); //QRectF (-30,-30,35,35);
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
        painter->setPen(*mPen);
        mPen->setColor(Qt::white);
        //painter->setBrush(Qt::green);
        painter->drawEllipse(mapRectFromScene(makeCircle()));
        Q_UNUSED(option);
        Q_UNUSED(widget);
    }
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
        auto dx = event->scenePos().rx() - mPreviousPoint.rx();
        auto dy = event->scenePos().ry() - mPreviousPoint.ry();
        mPreviousPoint = event->scenePos();
        setShear(dx, dy);
        QGraphicsItem::mouseMoveEvent(event);
    }
    void mousePressEvent(QGraphicsSceneMouseEvent *event) {
        this->setCursor(QCursor(Qt::ClosedHandCursor));
        mPreviousPoint = event->scenePos();
        update();
        if(this->isSelected()) {
            select(false);
        } else {
            select(true);
        }
    }
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
        this->setCursor(QCursor(Qt::ArrowCursor));
        Q_UNUSED(event);
    }

    QRectF makeCircle() const;
    QPointF mPreviousPoint;

    //QRectF mRect;
    QPointF mOrigin;
    QPointF mRPoint;
    double mRadius;
    QPen *mPen;

    double mHorizontalShear;
    double mVerticalShear;
    ItemState mState;
};


#endif // CIRCLE_H
