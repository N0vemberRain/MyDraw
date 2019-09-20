#include "point.h"
#include <QPainter>


Dot::Dot(const int x, const int y,
             const double imgX, const double imgY)
    : QObject(nullptr), QGraphicsItem (), /*QGraphicsRectItem (),*/ _x(x), _y(y), _imgX(imgX), _imgY(imgY), _state(false) {
    mRect.setTopLeft(QPointF(_imgX, _imgY));
    mRect.setBottomRight(QPointF(_imgX + width, _imgY + height));
    mPen = new QPen(Qt::black, 0.5);
    mPen->setBrush(Qt::white);
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
}

Dot::Dot()
    : QObject(nullptr), QGraphicsItem (), /*QGraphicsRectItem (),*/ _x(0), _y(0), _imgX(0), _imgY(0), _state(false) {
    mRect.setTopLeft(QPointF(_imgX, _imgY));
    mRect.setBottomRight(QPointF(_imgX + width, _imgY + height));
    mPen = new QPen(Qt::black, 0.5);
    mPen->setBrush(Qt::white);
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
}

Dot::Dot(const int x, const int y)
    : QObject (nullptr), QGraphicsItem (), /*QGraphicsRectItem (),*/ _x(x), _y(y) {
    mRect.setTopLeft(QPointF(_imgX, _imgY));
    mRect.setBottomRight(QPointF(_imgX + width, _imgY + height));
    mPen = new QPen(Qt::black, 0.5);
    mPen->setBrush(Qt::white);
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
}

void Dot::setImgCoords(const double imgX, const double imgY) {
    if(!qFuzzyCompare(_imgX, imgX)) {
        _imgX = imgX;
    }
    if(!qFuzzyCompare(_imgY, imgY)) {
        _imgY = imgY;
    }
}

void Dot::setCoords(const int x, const int y) {
    if(_x != x) {
        _x = x;
    }
    if(_y != y) {
        _y = y;
    }
}

QRectF Dot::getRect() const {
    //mRect.setTopLeft(QPointF(_imgX, _imgY));
    //mRect.setBottomRight(QPointF(_imgX + width, _imgY + height));
    return mRect;
}

void Dot::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
           QWidget *widget) {
    /*if(_state) {
        painter->setBrush(Qt::black);
    } else {
        painter->setBrush(Qt::white);
    }*/

    mPen->setColor(Qt::black);
    painter->setPen(*mPen);
    painter->setBrush(Qt::black);
    painter->drawRect(mRect);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}


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
