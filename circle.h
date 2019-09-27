#ifndef CIRCLE_H
#define CIRCLE_H

#include <QObject>
#include <QGraphicsEllipseItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QCursor>

#include "types.h"
#include "point.h"

class CircleDot : public QObject, public QGraphicsItem {
    Q_OBJECT
public:
    enum { Type = CircleType };
    explicit CircleDot(QObject *parent)
        : QObject(parent), QGraphicsItem (), x0(0), y0(0), radius(0) {
        setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
        QPoint topLeft(4.4 * (x0 - radius), 6 * (y0 + radius + 1));
        QPointF bottomRight(4.4 * (x0 + radius + 1), 6 * (y0 - radius));
        mRect.setTopLeft(topLeft);
        mRect.setBottomRight(bottomRight);
        mPreviousPoint = mRect.center();
    }

    explicit CircleDot(const int x, const int y, const int r)
        : QObject(), QGraphicsItem (), x0(x), y0(y), radius(r) {
        createCircle();
        setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
        QPoint topLeft(4.4 * (x0 - radius), 6 * (y0 + radius + 1));
        QPointF bottomRight(4.4 * (x0 + radius + 1), 6 * (y0 - radius));
        mRect.setTopLeft(topLeft);
        mRect.setBottomRight(bottomRight);
        mPreviousPoint = mRect.center();
    }

    ~CircleDot() {}

    int type() const { return Type; }

    QVector<QPair<int, int>> getPoints() const {
        return points;
    }

    QRectF getRect() const { return boundingRect(); }

    void setMouseEvent(QGraphicsSceneMouseEvent *event) {
        switch (event->type()) {
        case QEvent::GraphicsSceneMousePress: mousePressEvent(event);
            break;
        case QEvent::GraphicsSceneMouseMove: mouseMoveEvent(event);
            break;
        default: return;
        }
    }

    void setShear(const double horizontalShear, const double verticalShear);
    QVector<QRectF*> mapToPixels();
    QVector<Dot*> mapToBitmap();

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

    QRectF boundingRect() const override {
        return mRect;
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) {
        if(isSelected()) {
            painter->setPen(QPen(Qt::blue, 1));
        } else {
            painter->setPen(QPen(Qt::black, 1));
        }
        painter->setBrush(Qt::black);
        for(const auto & it : mPoints) {
            painter->drawRect(mapRectFromScene(it->getRect()));
        }

        Q_UNUSED(option);
        Q_UNUSED(widget);
    }

private:
    void updateTransform();
    void createDot(const int x, const int y) {
        auto imgX = x * 4.4;
        auto imgY = y * 6;
        Dot *dot = new Dot(x, y, imgX, imgY);
        mPoints.append(dot);
    }
    void createCircle() {
        int x = radius;
        int y = 0;
        int radiusError = 1 - x;
        while (x >= y) {
             points.append(qMakePair(x + x0, y + y0));
             createDot(x + x0, y + y0);
             points.append(qMakePair(y + x0, x + y0));
             createDot(y + x0, x + y0);
             points.append(qMakePair(-x + x0, y + y0));
             createDot(-x + x0, y + y0);

             points.append(qMakePair(-y + x0, x + y0));
             createDot(-y + x0, x + y0);

             points.append(qMakePair(-x + x0, -y + y0));
             createDot(-x + x0, -y + y0);

             points.append(qMakePair(-y + x0, -x + y0));
             createDot(-y + x0, -x + y0);

             points.append(qMakePair(x + x0, -y + y0));
             createDot(x + x0, -y + y0);

             points.append(qMakePair(y + x0, -x + y0));
             createDot(y + x0, -x + y0);

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
    QVector<Dot*> mPoints;

    double mHorizontalShear;
    double mVerticalShear;
    QPointF mPreviousPoint;
    QRectF mRect;
};

class Circle : public QObject, public QGraphicsEllipseItem {
    Q_OBJECT
public:
    enum { Type = CircleType };
    explicit Circle(const QPointF &origin, const double radius);
    explicit Circle(const QPointF &origin, const QPointF radius);
    ~Circle() override {}

    int type() const override { return Type; }
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget) override;

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

public slots:
    void select(bool state);

signals:

private:
    void updateTransform();

    QRectF boundingRect() const {
        return makeCircle(); //QRectF (-30,-30,35,35);
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
        painter->setPen(*mPen);
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

};


#endif // CIRCLE_H
