#ifndef POINT_H
#define POINT_H

#include <QObject>
#include <QGraphicsItem>

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

#endif // POINT_H
