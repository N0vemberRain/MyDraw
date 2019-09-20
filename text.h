#ifndef TEXT_H
#define TEXT_H

#include <QObject>
#include <QGraphicsTextItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QCursor>

#include <QDialog>
#include <QDialogButtonBox>
#include <QTextEdit>

#include "types.h"

class Text : public QObject, public QGraphicsItem//public QGraphicsTextItem
{
    Q_OBJECT
public:
    enum { Type = TextType };
    //explicit Text(const QPointF &pos);
    explicit Text(/*QGraphicsScene *scene,*/ const QPointF &pos, const QString &text = "Text");
    ~Text() override;

    int type() const override { return Type; }
    void setMouseEvent(QGraphicsSceneMouseEvent *pe) {
        if(pe->type() == QEvent::GraphicsSceneMouseMove) {
            mouseMoveEvent(pe);
        }
        if(pe->type() == QEvent::GraphicsSceneMousePress) {
            mousePressEvent(pe);
        }
    }

    void setData(const QStringList &data);
    QStringList getData() const;
    void setShear(const double horizontal, const double vertical);
    void edit();

signals:

public slots:
    void select(bool state);

private:
    void updateTransform();

//    QRectF boundingRect() const;
   // void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
/*    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
*/
    QPointF mPreviousPoint;
    QPointF mPos;
    QString mText;

    double mHorizontalShear;
    double mVerticalShear;
    QPen *mPen;
};

class TextItem : public QGraphicsTextItem
{
    Q_OBJECT
#ifdef ANIMATE_ALIGNMENT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
#endif
    Q_PROPERTY(double angle READ angle WRITE setAngle)
    Q_PROPERTY(double shearHorizontal READ shearHorizontal
               WRITE setShearHorizontal)
    Q_PROPERTY(double shearVertical READ shearVertical
               WRITE setShearVertical)

public:
    enum {Type = TextType};

    explicit TextItem(const QPoint &position, QGraphicsScene *scene);
  //  explicit TextItem(const QPointF &position);
    explicit TextItem(const QPointF &position, const QString &text, const QString &fontHeight);

    int type() const { return Type; }

    double angle() const { return m_angle; }
    double shearHorizontal() const { return mHorizontalShear; }
    double shearVertical() const { return mVerticalShear; }

    void setMouseEvent(QGraphicsSceneMouseEvent *pe) {
        if(pe->type() == QEvent::GraphicsSceneMouseMove) {
            mouseMoveEvent(pe);
        }
        if(pe->type() == QEvent::GraphicsSceneMousePress) {
            mousePressEvent(pe);
        }
    }

    QRectF boundingRect() const {
        return mBoundingRect;
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
        //painter->setPen(*mPen);
        //painter->setBrush(Qt::green);
        auto h = textHeight;
        if(this->isSelected()) {
            painter->setFont(QFont("Helvetica", textHeight, QFont::Bold));
        } else {
            painter->setFont(QFont("Helvetica", textHeight));
        }
        painter->drawText(mPreviousPoint, mText);
        Q_UNUSED(option);
        Q_UNUSED(widget);
    }

    QStringList getData() const;
    void setData(const QStringList &data);
public slots:
    void setAngle(double angle);
    void setShearHorizontal(double shearHorizontal)
        { setShear(shearHorizontal, mVerticalShear); }
    void setShearVertical(double shearVertical)
        { setShear(mHorizontalShear, shearVertical); }
    void setShear(double shearHorizontal, double shearVertical);
    void edit();

    void select(bool state);



signals:
    void dirty();

protected:
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent*) { edit(); }
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    /*QRectF boundingRect() const {
        return
    }*/
   /* void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
        painter->setFont(QFont("Helvetica", 8));
        auto c = mText.data();
        auto x = this->toHtml().begin()->digitValue();
        auto y = this->toHtml().begin()->digitValue();
        painter->drawText(QPointF(x, y), mText);
        Q_UNUSED(option);
        Q_UNUSED(widget);
    }*/





    void updateTransform();

    double m_angle;
    double mHorizontalShear;
    double mVerticalShear;

    QPointF mPreviousPoint;
    QPoint mPos;
    QPen *mPen;
    QString mText;

    QFont mFont;
    QRectF mBoundingRect;
    int textHeight;

};


class TextItemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TextItemDialog(TextItem *item_=0,
            const QPoint &position_=QPoint(),
            QGraphicsScene *scene_=0, QWidget *parent=0);

    TextItem *textItem() const { return item; }

public slots:
    void accept();

private slots:
    void updateUi();

private:
    void createWidgets();
    void createLayout();
    void createConnections();

    QTextEdit *textEdit;
    QDialogButtonBox *buttonBox;

    TextItem *item;
    QPoint position;
    QGraphicsScene *scene;
    QString originalHtml;
};

#endif // TEXT_H
