#include "text.h"

#include <QGraphicsScene>
#include <QGraphicsView>

#include <QPushButton>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QStyle>

/*Text::Text(QGraphicsScene *scene, const QPointF &pos, const QString &text)
    : QGraphicsTextItem (), mPos(pos), mText(text),
      mHorizontalShear(0.0), mVerticalShear(0.0) {
    setFont(QFont("Helvetica", textHeight));
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    setPos(mPos);
    setPlainText(text);
    scene->clearSelection();
    auto d = this->toPlainText();
    scene->addItem(this);
    show();
}*/


Text::Text(const QPointF &pos, const QString &text)
    : QObject (), QGraphicsItem (), mPos(pos), mText(text),
      mHorizontalShear(0.0), mVerticalShear(0.0) {
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

/*QRectF Text::boundingRect() const {
    QPointF topLeft = QPointF(this->pos().x(), this->pos().y() + textHeight);
    QPointF bottomRight = QPointF(this->pos().x() + this->textWidth(), this->pos().y());
    return QRectF(topLeft, bottomRight);
}*/

/*void Text::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setFont(QFont("Helvetica", textHeight));
    painter->drawText(mapFromScene(mPos), mText);
}
/*
void Text::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {

}

void Text::mousePressEvent(QGraphicsSceneMouseEvent *event) {

}

void Text::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

}

*/

//////////////////////////////////////////////////////////////////////////////////

TextItem::TextItem(const QPoint &position, QGraphicsScene *scene)
    : QGraphicsTextItem("Hello!"), m_angle(0.0), mHorizontalShear(0.0),
      mVerticalShear(0.0)
{
    setFont(QFont("Helvetica", textHeight));
    setFlags(QGraphicsItem::ItemIsSelectable|
     QGraphicsItem::ItemIsMovable);
    setPos(position);
    scene->clearSelection();
    scene->addItem(this);
    setSelected(true);
}

TextItem::TextItem(const QPointF &position, const QString &text, const QString &fontHeight)
    : QGraphicsTextItem("Hello!"), m_angle(0.0), mHorizontalShear(0.0),
      mVerticalShear(0.0), textHeight(fontHeight.toInt())
{
    mFont = QFont("Helvetica", textHeight);
    setFlags(QGraphicsItem::ItemIsSelectable|
             QGraphicsItem::ItemIsMovable);
    setPos(position);
    setPlainText(text);
    mText = text;
    setTextInteractionFlags(Qt::TextEditorInteraction);
    auto n = text.count();
    setTextWidth(n*textHeight);
    auto x = position.x();
    auto y = position.y() - textHeight;
    mBoundingRect.setTopLeft(QPointF(x, y));
    mBoundingRect.setWidth(n*textHeight);
    mBoundingRect.setHeight(textHeight);
    auto tl = mBoundingRect.topLeft();
    auto w = mBoundingRect.width();
    auto h = mBoundingRect.height();
    int d = 5;
}

void TextItem::setData(const QStringList &data) {
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
        if(data.at(i) == "FH") {
            textHeight = data.at(i + 1).toInt();
        }
    }
}

QStringList TextItem::getData() const {
    QStringList data;
    data << QString::number(this->type()) << "P" << QString::number(this->pos().x())
         << QString::number(this->pos().y()) << "T" << mText << "FH" << QString::number(textHeight);
    return data;
}

void TextItem::edit()
{
    QWidget *window = 0;
    QList<QGraphicsView*> views = scene()->views();
    if (!views.isEmpty())
        window = views.at(0)->window();
    TextItemDialog dialog(this, QPoint(), scene(), window);
    if (dialog.exec())
        emit dirty();
}


void TextItem::setAngle(double angle)
{
    if (isSelected() && !qFuzzyCompare(m_angle, angle)) {
        m_angle = angle;
        updateTransform();
    }
}


void TextItem::setShear(double horizontal, double vertical)
{
    mHorizontalShear = horizontal;
    mVerticalShear = vertical;

    updateTransform();
}


void TextItem::updateTransform()
{
    QTransform transform;
    transform.shear(mHorizontalShear, mVerticalShear);
    transform.rotate(m_angle);
    setTransform(transform);

    auto tmp = mBoundingRect;
    auto topLeft = mapFromScene(QPointF(tmp.topLeft().rx() + mHorizontalShear,
                                        tmp.topLeft().ry() + mVerticalShear));
    auto bottomRight = mapFromScene(QPointF(tmp.bottomRight().rx() + mHorizontalShear,
                                        tmp.bottomRight().ry() + mVerticalShear));
    /*mBoundingRect.setCoords(mapFromScene(tmp.topLeft().rx() + mHorizontalShear),
                            tmp.topLeft().ry() + mVerticalShear,
                            tmp.bottomRight().rx() + mHorizontalShear,
                            tmp.bottomRight().ry() + mVerticalShear);*/

    mBoundingRect.setTopLeft(topLeft);
    mBoundingRect.setBottomRight(bottomRight);
}


QVariant TextItem::itemChange(GraphicsItemChange change,
                              const QVariant &value)
{
  //  if (isDirtyChange(change))
        emit dirty();
    return QGraphicsTextItem::itemChange(change, value);
}

void TextItem::select(bool state) {
    this->setSelected(state);
    if(this->isSelected()) {
        mFont = QFont("Helvetica", textHeight, Qt::blue);
    } else {
        mFont = QFont("Helvetica", textHeight, Qt::black);
    }
}


void TextItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    auto dx = event->scenePos().rx() - mPreviousPoint.rx();
    auto dy = event->scenePos().ry() - mPreviousPoint.ry();
    mPreviousPoint = event->scenePos();
    setShear(dx, dy);
    QGraphicsTextItem::mouseMoveEvent(event);
}

void TextItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    this->setCursor(QCursor(Qt::ClosedHandCursor));
   // mPreviousPoint = event->scenePos();
    //update();
    if(this->isSelected()) {
        select(false);
    } else {
        select(true);
    }
    update();
    QGraphicsTextItem::mousePressEvent(event);
}

void TextItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    this->setCursor(QCursor(Qt::ArrowCursor));
    QGraphicsTextItem::mouseReleaseEvent(event);
}

////////////////////////////////////////////////////////////////////////////

TextItemDialog::TextItemDialog(TextItem *item_,
        const QPoint &position_, QGraphicsScene *scene_,
        QWidget *parent)
    : QDialog(parent), item(item_), position(position_), scene(scene_)
{
    createWidgets();
    createLayout();
    createConnections();

    updateUi();
}


void TextItemDialog::createWidgets()
{
    textEdit = new QTextEdit;
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|
                                     QDialogButtonBox::Cancel);
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    buttonBox->button(QDialogButtonBox::Ok)->setIcon(
            style()->standardIcon(QStyle::SP_DialogOkButton));
    buttonBox->button(QDialogButtonBox::Cancel)->setIcon(
            style()->standardIcon(QStyle::SP_DialogCancelButton));

    if (item)
        textEdit->setHtml(item->toHtml());
}


void TextItemDialog::createLayout()
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(textEdit);
    layout->addWidget(buttonBox);
    setLayout(layout);
}


void TextItemDialog::createConnections()
{
    connect(textEdit, SIGNAL(textChanged()), this, SLOT(updateUi()));
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}


void TextItemDialog::updateUi()
{
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(
            !textEdit->toPlainText().isEmpty());
}


void TextItemDialog::accept()
{
    if (item && !textEdit->document()->isModified()) {
        QDialog::reject();
        return;
    }
    if (!item)
        item = new TextItem(position, scene);
    item->setHtml(textEdit->toHtml());
    item->update();
    QDialog::accept();
}
