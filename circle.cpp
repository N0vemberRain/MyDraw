#include "circle.h"

#include <QPainter>
#include <math.h>

void CircleDot::updateTransform() {
    QTransform transform;
    transform.translate(mHorizontalShear, mVerticalShear);
    setTransform(transform);

    auto tmp = mRect;
    mRect.setCoords(tmp.topLeft().rx() + mHorizontalShear,
                     tmp.topLeft().ry() + mVerticalShear,
                     tmp.bottomRight().rx() + mHorizontalShear,
                     tmp.bottomRight().ry() + mVerticalShear);

    foreach(auto dot, mPoints) {
        dot->setShear(mHorizontalShear, mVerticalShear);
    }

    update();

}

void CircleDot::setShear(const double horizontalShear, const double verticalShear) {
    mHorizontalShear = horizontalShear;
    mVerticalShear = verticalShear;

    updateTransform();
}

QVector<QRectF*> CircleDot::mapToPixels() {
    QVector<QRectF*> pixels;
    foreach(auto point, mPoints) {
        auto pixel = point->mapToPixels();
        pixels.append(pixel);
    }

    return pixels;
}

QVector<Dot*> CircleDot::mapToBitmap() {
    return mPoints;
}

void CircleDot::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    auto dx = (event->scenePos().toPoint().rx() - mPreviousPoint.rx()) / 4.4;
    auto dy = (event->scenePos().toPoint().ry() - mPreviousPoint.ry()) / 6;
    mPreviousPoint = event->scenePos().toPoint();
    QGraphicsItem::mouseMoveEvent(event);
    setShear(dx, dy);
}

void CircleDot::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    setSelected(true);
    this->setCursor(QCursor(Qt::ClosedHandCursor));

    QGraphicsItem::mousePressEvent(event);
}

void CircleDot::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

}

void CircleDot::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {

}

Circle::Circle(const QPointF &origin, const double radius)
    : m_origin(origin), m_radius(radius) {
    m_pen = new QPen(Qt::black, 1);

    m_rPoint.setX(m_origin.rx() + radius);
    m_rPoint.setY(m_origin.ry());
}

Circle::Circle(const QPointF &origin, const QPointF radius)
    : m_origin(origin), m_rPoint(radius) {
    m_pen = new QPen(Qt::black, 1);
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
    mPen = new QPen(Qt::black, 1);
    this->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
}

CircleItem::CircleItem(const QPointF &origin, const QPointF &radius)
    : QObject (), QGraphicsItem (), mOrigin(origin), mRPoint(radius) {
    mRadius = abs(mRPoint.x() - mOrigin.x());
    mPen = new QPen(Qt::black, 1);
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
    if(this->isSelected()) {
        mPen->setColor(Qt::green);
        mPen->setWidth(2);
    } else {
        mPen->setColor(Qt::black);
        mPen->setWidth(1);
    }
}

QStringList CircleItem::getData() const {
    mPen->setColor(Qt::blue);
    mPen->setWidth(2);
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
