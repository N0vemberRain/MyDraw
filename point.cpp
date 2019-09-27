#include "point.h"
#include <QPainter>
#include <QCursor>
#include <QGraphicsSceneEvent>

Dot::Dot(const int x, const int y,
             const double imgX, const double imgY)
    : QObject(nullptr), QGraphicsItem (), /*QGraphicsRectItem (),*/ _x(x), _y(y), _imgX(imgX), _imgY(imgY), _state(false),
        mHorizontalShear(0.0), mVerticalShear(0.0) {
    mRect.setTopLeft(QPointF(_imgX, _imgY));
    mRect.setBottomRight(QPointF(_imgX + width, _imgY + height));
    mPen = new QPen(Qt::black, 0.5);
    mPen->setBrush(Qt::white);
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    mPreviousPoint = mRect.center();
}

Dot::Dot()
    : QObject(nullptr), QGraphicsItem (), /*QGraphicsRectItem (),*/ _x(0), _y(0), _imgX(0), _imgY(0), _state(false),
        mHorizontalShear(0.0), mVerticalShear(0.0) {
    mRect.setTopLeft(QPointF(_imgX, _imgY));
    mRect.setBottomRight(QPointF(_imgX + width, _imgY + height));
    mPen = new QPen(Qt::black, 0.5);
    mPen->setBrush(Qt::white);
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    mPreviousPoint = mRect.center();//QPointF(_imgX, _imgY);

}

Dot::Dot(const int x, const int y)
    : QObject (nullptr), QGraphicsItem (), /*QGraphicsRectItem (),*/ _x(x), _y(y),
        mHorizontalShear(0.0), mVerticalShear(0.0) {
    mRect.setTopLeft(QPointF(_imgX, _imgY));
    mRect.setBottomRight(QPointF(_imgX + width, _imgY + height));
    mPen = new QPen(Qt::black, 0.5);
    mPen->setBrush(Qt::white);
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    mPreviousPoint = mRect.center();// QPointF(_imgX, _imgY);
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

QRectF* Dot::mapToPixels() {
    auto pixelPos = QPointF(mRect.topLeft().x() / 10, mRect.topLeft().y() / 10);
    auto pixelWidth = 0.44;
    auto pixelHeight = 0.6;
    auto pixel = new QRectF(pixelPos.x(), pixelPos.y(), pixelWidth, pixelHeight);
    return pixel;
}

void Dot::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    auto dx = (event->scenePos().toPoint().rx() - mPreviousPoint.rx());// / 10;//4.4;
    auto dy = (event->scenePos().toPoint().ry() - mPreviousPoint.ry());// / 10;//6;
    mPreviousPoint = event->scenePos().toPoint();
    QGraphicsItem::mouseMoveEvent(event);
    setShearMouse(dx, dy);
    update();
}
void Dot::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    setSelected(true);
    this->setCursor(QCursor(Qt::ClosedHandCursor));

    QGraphicsItem::mousePressEvent(event);
}
void Dot::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

}
void Dot::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {

}

void Dot::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
           QWidget *widget) {
    if(isSelected()) {
        mPen->setColor(Qt::blue);
    } else {
        mPen->setColor(Qt::white);
    }

    painter->setPen(*mPen);
    painter->setBrush(Qt::black);
    painter->drawRect(mapRectFromScene(mRect));
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
