#include "circle.h"

#include <QPainter>
#include <math.h>

Circle::Circle(const QPointF &origin, const double radius)
    : m_origin(origin), m_radius(radius) {

    m_rPoint.setX(m_origin.rx() + radius);
    m_rPoint.setY(m_origin.ry());
}

Circle::Circle(const QPointF &origin, const QPointF radius)
    : m_origin(origin), m_rPoint(radius) {
}

QRectF Circle::boundingRect() const {
    qreal penWidth = 1;
    return QRectF(-10 - penWidth / 2, -10 - penWidth / 2, 20 + penWidth, 20 + penWidth);
}

void Circle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setPen(*m_pen);
    auto data = makeCircle();
    painter->drawEllipse(data);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QRectF Circle::makeCircle() {
    auto dist = sqrt(pow(m_rPoint.rx() - m_origin.rx(), 2) +
                     pow(m_rPoint.ry() - m_origin.ry(), 2));
    auto xTopLeft = m_origin.rx() - dist;
    auto yTopLeft = m_origin.ry() - dist;
    QRectF rect(xTopLeft, yTopLeft, dist*2, dist*2);
    return rect;
}
///////////////////////////////////////////////////////////////////////////////////
CircleItem::CircleItem(const QPointF &origin, const double radius)
    : QObject (), QGraphicsItem (), mOrigin(origin), mRadius(radius) {
    mRPoint.setX(mOrigin.rx() + radius);
    mRPoint.setY(mOrigin.ry());
    this->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
}

CircleItem::CircleItem(const QPointF &origin, const QPointF &radius)
    : QObject (), QGraphicsItem (), mOrigin(origin), mRPoint(radius) {
    mRadius = abs(mRPoint.x() - mOrigin.x());
    this->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
}

QRectF CircleItem::makeCircle() const {
    auto dist = sqrt(pow(mRPoint.x() - mOrigin.x(), 2) +
                     pow(mRPoint.y() - mOrigin.y(), 2));
    auto xTopLeft = mOrigin.x() - dist;
    auto yTopLeft = mOrigin.y() - dist;
    QRectF rect(xTopLeft, yTopLeft, dist*2, dist*2);

    return rect;
}

void CircleItem::updateTransform() {
    QTransform transform;
    transform.translate(mHorizontalShear, mVerticalShear);
    setTransform(transform);

    mOrigin.setX(mOrigin.x() + mHorizontalShear);
    mOrigin.setY(mOrigin.y() + mVerticalShear);
    mRPoint.setX(mRPoint.x() + mHorizontalShear);
    mRPoint.setY(mRPoint.y() + mVerticalShear);

    update();
}

void CircleItem::setShear(const double horizontal, const double vertical) {
    mHorizontalShear = horizontal;
    mVerticalShear = vertical;

    updateTransform();
}

void CircleItem::select(bool state) {
    this->setSelected(state);
}

QStringList CircleItem::getData() const {
    QStringList data;
    data << QString::number(this->type()) << "PO" << QString::number(mOrigin.x())
         << QString::number(mOrigin.y())
         << "PR" << QString::number(mRPoint.x())
         << QString::number(mRPoint.y())
         << "R" << QString::number(mRadius);

    return data;
}

void CircleItem::setData(const QStringList &data) {
    if(data.at(0) != CircleType) {
        return;
    }

    for(int i = 1; i < data.count(); i++) {
        if(data.at(i) == "P1") {
            mOrigin.setX(data.at(i + 1).toDouble());
            mOrigin.setY(data.at(i + 2).toDouble());
            i += 2;
            continue;
        }
        if(data.at(i) == "P2") {
            mRPoint.setX(data.at(i + 1).toDouble());
            mRPoint.setY(data.at(i + 2).toDouble());
            i += 2;
            continue;
        }
        if(data.at(i) == "R") {
            mRadius = data.at(i + 1).toDouble();
            i++;
            continue;
        }
    }
}

void CircleItem::paintNormal(QPainter *painter) {
    painter->setPen(QPen(Qt::white, 1));
    painter->drawEllipse(mapRectFromScene(makeCircle()));
}

void CircleItem::paintRendering(QPainter *painter) {
    if(!makeCircle().isNull()) {
        painter->setPen(QPen(Qt::white, 1));
        painter->drawEllipse(mapRectFromScene(makeCircle()));
    }
    painter->setPen(QPen(Qt::white, 3));
    painter->drawPoint(mOrigin);
}

void CircleItem::paintFocus(QPainter *painter) {
    painter->setPen(QPen(Qt::green, 1));
    painter->drawEllipse(mapRectFromScene(makeCircle()));
    painter->setPen(QPen(Qt::green, 3));
    painter->drawPoint(mOrigin);
}

void CircleItem::paintEdit(QPainter *painter) {

}

void CircleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    switch (mState) {
    case ItemState::Normal: paintNormal(painter); break;
    case ItemState::Rendering: paintRendering(painter); break;
    case ItemState::Focus: paintFocus(painter); break;
    case ItemState::Edit: paintEdit(painter); break;
    }

    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void CircleItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {

}

void CircleItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    //hovered = true;
    mState = ItemState::Focus;
    QGraphicsItem::hoverEnterEvent(event);
}

void CircleItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    //hovered = false;
    mState = ItemState::Normal;
    QGraphicsItem::hoverLeaveEvent(event);
}
