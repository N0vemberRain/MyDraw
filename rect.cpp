#include "rect.h"

#include <QPainter>
#include <QGraphicsSceneEvent>
#include <QCursor>


Rect::Rect(const QRectF &rect)
#if RECT_RECT
    : QObject (), QGraphicsRectItem (), m_rect(rect) {
#endif
#if RECT_ITEM
    : QObject (), QGraphicsItem (), m_rect(rect) {
#endif
    m_topLeft = m_rect.topLeft();
    m_bottomRight = m_rect.bottomRight();
    m_width = m_rect.width();
    m_height = m_rect.height();

    m_pen = new QPen(Qt::black, 1);
    setFlags(QGraphicsItem::ItemIsSelectable|
                 QGraphicsItem::ItemSendsGeometryChanges|
                 QGraphicsItem::ItemIsMovable|
                 QGraphicsItem::ItemIsFocusable);
}

Rect::Rect(const QPointF &topLeft, const double width, const double height)
    : m_topLeft(topLeft), m_width(width), m_height(height) {
    m_bottomRight.setX(m_topLeft.rx() + m_width);
    m_bottomRight.setY(m_topLeft.ry() - m_height);
    m_rect.setWidth(m_width);
    m_rect.setHeight(m_height);
    m_rect.setTopLeft(m_topLeft);
    m_rect.setBottomRight(m_bottomRight);

    m_pen = new QPen(Qt::black, 1);
}

Rect::Rect(const QPointF &topLeft, const QPointF &bottomRight)
    : m_topLeft(topLeft), m_bottomRight(bottomRight) {

    m_pen = new QPen(Qt::black, 1);
    m_rect = QRectF(m_topLeft, m_bottomRight);
    m_width = m_rect.width();
    m_height = m_rect.height();
}

QRectF Rect::boundingRect() const {
    //qreal penWidth = 1;
    return m_rect;
}

void Rect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setPen(*m_pen);
    painter->drawRect(m_rect);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void Rect::mouseMoveEvent(QGraphicsSceneMouseEvent *pe) {
    setCursor(Qt::ClosedHandCursor);
    auto dx = pe->scenePos().x() - m_previousPosition.x();
    auto dy = pe->scenePos().y() - m_previousPosition.y();
    moveBy(dx,dy);
    //m_rect.moveTo(dx, dy);

    m_previousPosition = pe->scenePos();
    if(pe->scenePos().rx() > 30 && pe->scenePos().ry() > 30) {
        int d = 5;
    }

    update();
 //   this->setPos(mapToScene(pe->pos()));
    //emit signalMove(this, dx, dy);
    //m_rect = this->boundingRect();*/

#if RECT_RECT
    QGraphicsRectItem::mouseMoveEvent(pe);
#endif

#if RECT_ITEM
    QGraphicsItem::mouseMoveEvent(pe);
  //  Q_UNUSED(pe);
#endif
}

void Rect::mousePressEvent(QGraphicsSceneMouseEvent *pe) {
    this->setCursor(QCursor(Qt::ClosedHandCursor));
    m_previousPosition = pe->scenePos();
    update();
    setSelect();

#if RECT_RECT
    QGraphicsRectItem::mousePressEvent(pe);
#endif

#if RECT_ITEM
   QGraphicsItem::mousePressEvent(pe);
  //  Q_UNUSED(pe);
#endif
    //Q_UNUSED(pe);
}

void Rect::mouseReleaseEvent(QGraphicsSceneMouseEvent *pe) {
    this->setCursor(QCursor(Qt::ArrowCursor));
    update();

#if RECT_RECT
    QGraphicsRectItem::mouseReleaseEvent(pe);
#endif

#if RECT_ITEM
   QGraphicsItem::mouseReleaseEvent(pe);
  //  Q_UNUSED(pe);
#endif

    //Q_UNUSED(pe);
}

void Rect::setData(const QStringList &data) {
    for(int i = 0; i < data.count(); i++) {
        if(data.at(i) == "P1") {
            m_topLeft.setX(i + 1);
            m_topLeft.setY(i + 2);
            i += 2;
            continue;
        }
        if(data.at(i) == "P2") {
            m_bottomRight.setX(i + 1);
            m_bottomRight.setY(i + 2);
            i += 2;
            continue;
        }
        if(data.at(i) == "W") {
            m_width = data.at(i + 1).toDouble();
            i++;
            continue;
        }
        if(data.at(i) == "H") {
            m_height = data.at(i + 1).toDouble();
            i++;
            continue;
        }
    }
}

QStringList Rect::getData() const {
    m_pen->setColor(Qt::blue);
    m_pen->setWidth(2);
    QStringList data;
    data << QString::number(this->type()) << "P1" << QString::number(m_topLeft.x()) << QString::number(m_topLeft.y())
         << "P2" << QString::number(m_bottomRight.x()) << QString::number(m_bottomRight.y())
         << "W" << QString::number(m_width) << "H" << QString::number(m_height);
    return data;
}

void Rect::setSelect() {
    this->setSelected(true);
    auto s = this->isSelected();
    if(s) {
        m_pen->setColor(Qt::green);
        m_pen->setWidth(2);
    }

  //  this->setSelected(true);
}


/////////////////////////////////////////////////////////////////

RectItem::RectItem(QObject *parent)
    : QObject(parent), QGraphicsItem() {
    this->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    setAcceptHoverEvents(true);
    hovered = false;
    mState = ItemState::Rendering;
}
        
RectItem::RectItem(const QRectF &rect)
    : QObject (), QGraphicsItem (), m_rect(rect) {
    m_pen = new QPen(Qt::black, 1);
    setFlags(QGraphicsItem::ItemIsSelectable|
                 QGraphicsItem::ItemSendsGeometryChanges|
                 QGraphicsItem::ItemIsMovable|
                 QGraphicsItem::ItemIsFocusable);
}

RectItem::RectItem(const QPointF &topLeft, const double width, const double height)
    : QObject (), QGraphicsItem () {
    m_rect.setTopLeft(topLeft);
    m_rect.setWidth(width);
    m_rect.setHeight(height);

    m_pen = new QPen(Qt::black, 1);
    setFlags(QGraphicsItem::ItemIsSelectable|
                 QGraphicsItem::ItemSendsGeometryChanges|
                 QGraphicsItem::ItemIsMovable|
                 QGraphicsItem::ItemIsFocusable);
}
RectItem::RectItem(const QPointF &topLeft, const QPointF &bottomRight)
    : QObject (), QGraphicsItem () {
    m_rect.setTopLeft(topLeft);
    m_rect.setBottomRight(bottomRight);

    m_pen = new QPen(Qt::black, 1);
    setFlags(QGraphicsItem::ItemIsSelectable|
                 QGraphicsItem::ItemSendsGeometryChanges|
                 QGraphicsItem::ItemIsMovable|
                 QGraphicsItem::ItemIsFocusable);
}

void RectItem::setData(const QStringList &data) {
    for(int i = 1; i < data.count(); i++) {
        if(data.at(i) == "P1") {
            m_rect.setTopLeft(QPointF(i + 1, i + 2));
            i += 2;
            continue;
        }
        if(data.at(i) == "P2") {
            m_rect.setBottomRight(QPointF(i + 1, i + 2));
            i += 2;
            continue;
        }
        if(data.at(i) == "W") {
            m_rect.setWidth(data.at(i + 1).toDouble());
            i++;
            continue;
        }
        if(data.at(i) == "H") {
            m_rect.setHeight(data.at(i + 1).toDouble());
            i++;
            continue;
        }
    }
}

QStringList RectItem::getData() const {
    m_pen->setColor(Qt::blue);
    m_pen->setWidth(2);
    QStringList data;
    data << QString::number(this->type()) << "P1" << QString::number(m_rect.topLeft().x())
         << QString::number(m_rect.topLeft().y())
         << "P2" << QString::number(m_rect.bottomRight().x())
         << QString::number(m_rect.bottomRight().y())
         << "W" << QString::number(m_rect.width()) << "H" << QString::number(m_rect.height());

    return data;
}

void RectItem::select(bool state) {
    this->setSelected(state);
    if(this->isSelected()) {
        m_pen->setColor(Qt::green);
        m_pen->setWidth(2);
    } else {
        m_pen->setColor(Qt::black);
        m_pen->setWidth(1);
    }
}

QVariant RectItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) {
    return QGraphicsItem::itemChange(change, value);
}

void RectItem::setShearHorizontal(const double shear) {
    mHorizontalShear = shear;
}

void RectItem::setShearVertical(const double shear) {
    mVerticalShear = shear;
}

void RectItem::updateTransform() {
    QTransform transform;
    transform.translate(mHorizontalShear, mVerticalShear);
    setTransform(transform);
    auto tmp = m_rect;
    m_rect.setCoords(tmp.topLeft().rx() + mHorizontalShear,
                     tmp.topLeft().ry() + mVerticalShear,
                     tmp.bottomRight().rx() + mHorizontalShear,
                     tmp.bottomRight().ry() + mVerticalShear);
    update();
}

void RectItem::setShear(const double horizontal, const double vertical) {
    mHorizontalShear = horizontal;
    mVerticalShear = vertical;

    updateTransform();
}

void RectItem::move(const double horizontal, const double vertical) {
    moveBy(horizontal, vertical);

    auto tmp = m_rect;
    m_rect.setCoords(tmp.topLeft().rx() + horizontal,
                     tmp.topLeft().ry() + vertical,
                     tmp.bottomRight().rx() + horizontal,
                     tmp.bottomRight().ry() + vertical);
    update();
}

void RectItem::paintNormal(QPainter *painter) {
    painter->setPen(QPen(Qt::black, 1));
    painter->drawRect(m_rect);
}

void RectItem::paintRendering(QPainter *painter) {
    if(!m_rect.isNull()) {
        painter->setPen(QPen(Qt::black, 1));
        painter->drawRect(m_rect);
    }
    painter->setPen(QPen(Qt::black, 3));
    painter->drawPoint(mTopLeft);
    painter->drawPoint(mBottomRight);
}

void RectItem::paintFocus(QPainter *painter) {
    painter->setPen(QPen(Qt::green, 1));
    painter->drawRect(m_rect);
    painter->setPen(QPen(Qt::green, 3));
    painter->drawPoint(mTopLeft);
    painter->drawPoint(mBottomRight);
}

void RectItem::paintEdit(QPainter *painter) {
    painter->setPen(QPen(Qt::blue, 2));
    painter->drawRect(m_rect);
    painter->setPen(QPen(Qt::blue, 3));
    painter->drawPoint(mTopLeft);
    painter->drawPoint(mBottomRight);
}

void RectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    switch (mState) {
    case ItemState::Normal: paintNormal(painter); break;
    case ItemState::Rendering: paintRendering(painter); break;
    case ItemState::Focus: paintFocus(painter); break;
    case ItemState::Edit: paintEdit(painter); break;
    }

    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void RectItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {

}

void RectItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    hovered = true;
    mState = ItemState::Focus;
    QGraphicsItem::hoverEnterEvent(event);
}

void RectItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    hovered = false;
    mState = ItemState::Normal;
    QGraphicsItem::hoverLeaveEvent(event);
}



