#include "polyline.h"
#include <QPainter>

Polyline::Polyline(const QVector<QPointF> &points)
    : QObject (), QGraphicsItem () {
    bool first = true;
    QLineF tmp;
    for(int i = 0; i < points.count(); i++) {
        if(first) {
            tmp.setP1(points.at(i));
            first = false;
        } else {
            tmp.setP2(points.at(i));
            mPolyline.append(tmp);
            first = true;
        }
    }

    mPen = new QPen(Qt::black, 1);
    this->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
}

QRectF Polyline::boundingRect() const {
    //return mPoints.boundingRect();
}

void Polyline::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setPen(*mPen);
    painter->drawPath(mapFromScene(mPoints));
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void Polyline::updateTransform() {
    QTransform transform;
    transform.translate(mHorizontalShear, mVerticalShear);
    setTransform(transform);

    auto tmpPath = mPoints;
    //mPoints.c
    for(int i = 0; i < tmpPath.elementCount(); i++) {
        QPointF p = tmpPath.elementAt(i);
        p.setX(p.x() + mHorizontalShear);
        p.setY(p.y() + mVerticalShear);
        mPoints.lineTo(p);

    }
    /*
    for(int i = 0; i < mPoints.elementCount(); i++) {
        auto item = mPoints.elementAt(i);
        int t = item.type;
        if(t == QGraphicsItem::Type::)
    }*/
}

void Polyline::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {

}

void Polyline::mousePressEvent(QGraphicsSceneMouseEvent *event) {

}

void Polyline::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

}

QStringList Polyline::getData() const {

}


















MoveItem::MoveItem(QObject *parent) :
    QObject(parent), QGraphicsItem()
{

}

MoveItem::~MoveItem()
{

}

QRectF MoveItem::boundingRect() const
{
    return QRectF (-30,-30,60,60);
}

void MoveItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::black);
    painter->setBrush(Qt::green);
    painter->drawRect(-30,-30, 35, 35);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void MoveItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    /* Устанавливаем позицию графического элемента
     * в графической сцене, транслировав координаты
     * курсора внутри графического элемента
     * в координатную систему графической сцены
     * */
    this->setPos(mapToScene(event->pos()));
    QGraphicsItem::mouseMoveEvent(event);
}

void MoveItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    /* При нажатии мышью на графический элемент
     * заменяем курсор на руку, которая держит этот элемента
     * */
    this->setCursor(QCursor(Qt::ClosedHandCursor));    QGraphicsItem::mouseMoveEvent(event);

//    Q_UNUSED(event);
}

void MoveItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    /* При отпускании мышью элемента
     * заменяем на обычный курсор стрелку
     * */
    this->setCursor(QCursor(Qt::ArrowCursor));
    QGraphicsItem::mouseMoveEvent(event);

//    Q_UNUSED(event);
}

void MoveItem::setMouseEvent(QGraphicsSceneMouseEvent *event) {
    mouseMoveEvent(event);
}
