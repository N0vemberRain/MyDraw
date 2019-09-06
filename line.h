#ifndef LINE_H
#define LINE_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QCursor>

#include "types.h"

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
