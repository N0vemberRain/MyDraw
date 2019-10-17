#include "line.h"

#include <QPainter>

Line::Line(QObject *parent) : QObject(parent), QGraphicsItem() {
    this->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    mPen = new QPen(Qt::black, 1);
    setAcceptHoverEvents(true);
    hovered = false;
}

Line::Line(const QLineF &line)
    : QObject(), QGraphicsItem(), mLine(line) {
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    mPen = new QPen(Qt::black, 1);
}

Line::Line(const QPointF &p1, const QPointF &p2)
    : QObject (), QGraphicsItem () {
    mLine.setP1(p1);
    mLine.setP2(p2);

    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    mPen = new QPen(Qt::black, 1);
}

Line::~Line() {

}

QRectF Line::boundingRect() const {
    return QRectF(mLine.p1(), mLine.p2());
}

void Line::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    if(this->isSelected()) {
        mPen->setColor(Qt::red);
    }

    if(hovered) {
        painter->setPen(QPen(Qt::green, 2));
    } else {
        mPen->setColor(Qt::white);
        painter->setPen(*mPen);
    }

    if(!mLine.isNull()) {
        painter->setPen(*mPen);
        //mPen->setColor(Qt::white);
        QPainterPath path(mLine.p1());
        path.lineTo(mLine.p2());
        painter->drawPath(mapFromScene(path));
    }

    painter->setPen(QPen(Qt::white, 3));
    painter->drawPoint(p1);
    painter->drawPoint(p2);

    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void Line::setData(const QStringList &data) {
    if(data.at(0).toInt() != LineType) {
        return;
    }

    for (int i = 0; i < data.count(); i++) {
        if(data.at(i) == "P1") {
            mLine.setP1(QPointF(data.at(i + 1).toDouble(), data.at(i + 2).toDouble()));
            i += 2;
            continue;
        }
        if(data.at(i) == "P2") {
            mLine.setP2(QPointF(data.at(i + 1).toDouble(), data.at(i + 2).toDouble()));
            i += 2;
            continue;
        }
        if(data.at(i) == "L") {
            mLine.setLength(data.at(i + 1).toDouble());
            i++;
            continue;
        }
        if(data.at(i) == "A") {
            mLine.setAngle(data.at(i + 1).toDouble());
            i++;
            continue;
        }
    }
}

QStringList Line::getData() const {
    mPen->setColor(Qt::blue);
    mPen->setWidth(2);
    QStringList data;
    data << QString::number(this->type()) << "P1" << QString::number(mLine.p1().x())
         << QString::number(mLine.p1().y())
         << "P2" << QString::number(mLine.p2().x())
         << QString::number(mLine.p2().y())
         << "L" << QString::number(mLine.length()) << "A" << QString::number(mLine.angle());

    return data;
}

void Line::select(bool state) {
    this->setSelected(state);
    if(this->isSelected()) {
        mPen->setColor(Qt::green);
        mPen->setWidth(2);
    } else {
        mPen->setColor(Qt::black);
        mPen->setWidth(1);
    }
}

void Line::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    auto dx = event->scenePos().rx() - mPreviousPoint.rx();
    auto dy = event->scenePos().ry() - mPreviousPoint.ry();
    mPreviousPoint = event->scenePos();
    setShear(dx, dy);
    QGraphicsItem::mouseMoveEvent(event);
}

void Line::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    this->setCursor(QCursor(Qt::ClosedHandCursor));
    mPreviousPoint = event->scenePos();
    update();
    if(this->isSelected()) {
        select(false);
    } else {
        select(true);
    }
}

void Line::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    this->setCursor(QCursor(Qt::ArrowCursor));
    Q_UNUSED(event);
}

void Line::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {

}


void Line::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {

}

void Line::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    hovered = true;
    QGraphicsItem::hoverEnterEvent(event);
}

void Line::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    hovered = false;
    QGraphicsItem::hoverLeaveEvent(event);
}

void Line::setShear(const double horizontal, const double vertical) {
    mHorizontalShear = horizontal;
    mVerticalShear = vertical;

    updateTransform();
}

void Line::updateTransform() {
    QTransform transform;
    transform.translate(mHorizontalShear, mVerticalShear);
    setTransform(transform);
    mLine.setLine(mLine.p1().x() + mHorizontalShear,
                  mLine.p1().y() + mVerticalShear,
                  mLine.p2().x() + mHorizontalShear,
                  mLine.p2().y() + mVerticalShear);

    update();
}
