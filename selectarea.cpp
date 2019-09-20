
#include "selectarea.h"

SelectArea::SelectArea(QObject *parent) : QObject(parent)
{
    mRect.setTopLeft(QPointF(-1, -1));
    mRect.setBottomRight(QPointF(-1, -1));
}

SelectArea::~SelectArea() {

}

void SelectArea::setMouseEvent(QGraphicsSceneMouseEvent *event) {
    if(event->type() == QEvent::GraphicsSceneMouseMove) {
        mouseMoveEvent(event);
    }
    if(event->type() == QEvent::GraphicsSceneMousePress) {
        mousePressEvent(event);
    }
    if(event->type() == QEvent::GraphicsSceneMouseRelease) {
        mouseReleaseEvent(event);
    }
}

void SelectArea::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QPen pen;
    pen.setWidth(1);
    pen.setStyle(Qt::DotLine);
    pen.setColor(Qt::black);
    painter->setPen(pen);
    //if(mRect != nullptr) {
        painter->drawRect(mapRectFromScene(mRect));
    //}
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void SelectArea::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if(mRect.topLeft() == QPoint(-1, -1)) {
        mRect.setTopLeft(event->scenePos());
    }
    mRect.setBottomRight(event->scenePos());
    QGraphicsItem::mouseMoveEvent(event);
}

void SelectArea::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    mRect = QRectF(event->scenePos(), event->scenePos());
    //mRect.setTopLeft(event->scenePos());
    //mPreviousPosition = event->scenePos();
    QGraphicsItem::mousePressEvent(event);
}

void SelectArea::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    mRect.setTopLeft(QPointF(-1, -1));
    mRect.setBottomRight(QPointF(-1, -1));
    //delete mRect;
    //mPreviousPosition = QPoin
    QGraphicsItem::mouseReleaseEvent(event);
}
