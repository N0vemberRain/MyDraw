#include "text.h"

Text::Text(const QPointF &pos, const QString &text)
    : QGraphicsTextItem (text), mPos(pos), mText(text),
      mHorizontalShear(0.0), mVerticalShear(0.0) {
    setFont(QFont("Helvetica", 11));
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    setPos(mPos);
}

Text::~Text() {

}

void Text::setData(const QStringList &data) {
    if(data.at(0) != TextType) {
        return;
    }

    for(int i = 0; i < data.count(); i++) {
        if(data.at(i) == "P") {
            mPos.setX(data.at(i + 1).toDouble());
            mPos.setY(data.at(i + 2).toDouble());
            i += 2;
            continue;
        }
        if(data.at(i) == "T") {
            mText = data.at(i + 1);
        }
    }
}

QStringList Text::getData() const {
    QStringList data;
    data << QString::number(this->type()) << "P" << QString::number(mPos.x())
         << QString::number(mPos.y()) << "T" << mText;
    return data;
}

void Text::setShear(const double horizontal, const double vertical) {
    mHorizontalShear = horizontal;
    mVerticalShear = vertical;

    updateTransform();
}

void Text::select(bool state) {

}

void Text::updateTransform() {

}

QRectF Text::boundingRect() const {

}

void Text::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->drawText(mPos, mText);
}
/*/
void Text::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {

}

void Text::mousePressEvent(QGraphicsSceneMouseEvent *event) {

}

void Text::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

}

*/
