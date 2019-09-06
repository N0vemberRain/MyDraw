#ifndef SELECTAREA_H
#define SELECTAREA_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QCursor>

#include "types.h"

class SelectArea : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    enum { Type = SelectType };
    explicit SelectArea(QObject *parent = nullptr);
    ~SelectArea() override;

    int type() const { return Type; }
    void setMouseEvent(QGraphicsSceneMouseEvent *event);
    QRectF getArea() const { return boundingRect(); }
signals:

public slots:

protected:
    QRectF boundingRect() const override {
        return mRect;
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
  //  QRectF *mRect = nullptr;
    QRectF mRect;
    QPointF mPreviousPosition;
};

#endif // SELECTAREA_H
