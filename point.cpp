#include "point.h"
#include <QPainter>

Point::Point(const QPointF &p)
    : m_point(p) {
    m_pen = new QPen(Qt::black, 1);
}


Point::~Point() {
    //delete m_pen;
}

QRectF Point::boundingRect() const {
    qreal penWidth = 1;
    return QRectF(-10 - penWidth / 2, -10 - penWidth / 2, 20 + penWidth, 20 + penWidth);
}

void Point::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
   // QPen pen = QPen(Qt::green, 5, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(*m_pen);
    painter->drawPoint(static_cast<int>(m_point.rx()), static_cast<int>(m_point.ry()));
   // painter->drawEllipse(QRectF(x-5, y-5, x+5, y+5));
    Q_UNUSED(option);
    Q_UNUSED(widget);
}
