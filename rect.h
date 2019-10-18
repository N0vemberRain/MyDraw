#ifndef RECT_H
#define RECT_H

#define RECT_ITEM 1
#define RECT_RECT 0

#include <QObject>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QCursor>

#include "types.h"

#if RECT_RECT
class Rect : public QObject, public QGraphicsRectItem {
    Q_OBJECT
public:
    enum { Type = RectType };
    explicit Rect(const QRectF &rect);
    explicit Rect(const QPointF &topLeft, const double width, const double height);
    explicit Rect(const QPointF &topLeft, const QPointF &bottomRight);

    ~Rect() {}

    int type() const override { return Type; }
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget);

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
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    /*void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
*/
private:
    QRectF m_rect;
    QPointF m_topLeft;
    QPointF m_bottomRight;
    double m_width;
    double m_height;
    QPointF m_previousPosition;
    QPen   *m_pen;
};
#endif

#if RECT_ITEM
class Rect : public QObject, public QGraphicsItem {
    Q_OBJECT
public:
    enum { Type = RectType };
    explicit Rect(const QRectF &rect);
    explicit Rect(const QPointF &topLeft, const double width, const double height);
    explicit Rect(const QPointF &topLeft, const QPointF &bottomRight);

    ~Rect() {}

    int type() const override { return Type; }
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget);

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
    void setSelect();
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    /*void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
*/
private:
    QRectF m_rect;
    QPointF m_topLeft;
    QPointF m_bottomRight;
    double m_width;
    double m_height;
    QPointF m_previousPosition;
    QPen   *m_pen;
};
#endif

class RectItem : public QObject, public QGraphicsItem {
    Q_OBJECT
public:
    enum { Type = RectType };
    explicit RectItem(QObject *parent = nullptr);
    explicit RectItem(const QRectF &rect);
    explicit RectItem(const QPointF &topLeft, const double width, const double height);
    explicit RectItem(const QPointF &topLeft, const QPointF &bottomRight);

    ~RectItem() override {
        delete m_pen;
    }

    int type() const override { return Type; }

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

    void setShearVertical(const double shear);
    void setShearHorizontal(const double shear);
    void setShear(const double horizontal, const double vertical);
    void move(const double horizontal, const double vertical);
    
    void setState(const ItemState state) { mState = state; }
    void setBegin(const QPointF &p) {
        //mLine.setP1(p);
        mTopLeft = p;
        update();
    }
    void setEnd(const QPointF &p) {

        //mLine.setP2(p);
        mBottomRight = p;
        m_rect.setTopLeft(mTopLeft);
        m_rect.setBottomRight(mBottomRight);
        //mState = ItemState::Normal;
    }
    QPointF getBegin() const { return m_rect.topLeft(); }
    QPointF getEnd() const { return m_rect.bottomRight(); }

    void setHoverEvent(QGraphicsSceneHoverEvent *event) {
        if(boundingRect().contains(event->scenePos())) {
            hoverEnterEvent(event);
        } else {
            hoverLeaveEvent(event);
        }
    }
signals:
    void sceneUpdate();
   
protected:
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
private:
    QRectF boundingRect() const override {
        return m_rect;
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
        painter->setPen(*m_pen);
        m_pen->setColor(Qt::white);
    //    painter->setBrush(Qt::green);
        //painter->drawPolygon(mapToScene(m_rect));
        painter->drawRect(mapRectFromScene(m_rect));
        Q_UNUSED(option);
        Q_UNUSED(widget);
    }
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
       // this->setPos(mapToScene(event->pos()));
        auto dx = event->scenePos().rx() - mPreviousPoint.rx();
        auto dy = event->scenePos().ry() - mPreviousPoint.ry();
        mPreviousPoint = event->scenePos();
        QGraphicsItem::mouseMoveEvent(event);
        setShear(dx, dy);
       // emit sceneUpdate();
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
//        Q_UNUSED(event);
    }
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
        this->setCursor(QCursor(Qt::ArrowCursor));
        Q_UNUSED(event);
    }

    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value);
    void updateTransform();
    void paintNormal(QPainter *painter);
    void paintRendering(QPainter *painter);
    void paintFocus(QPainter *painter);
    void paintEdit(QPainter *painter);

    QRectF m_rect;
    QPen *m_pen;

    QPointF mPreviousPoint;

    double mHorizontalShear;
    double mVerticalShear;
    
    QPointF mTopLeft;
    QPointF mBottomRight;
    bool hovered;
    ItemState mState;
public slots:
    void select(bool state);

};

#endif // RECT_H
