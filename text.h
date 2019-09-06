#ifndef TEXT_H
#define TEXT_H

#include <QObject>
#include <QGraphicsTextItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QCursor>

#include "types.h"

class Text : public QGraphicsTextItem
{
    Q_OBJECT
public:
    enum { Type = TextType };
    //explicit Text(const QPointF &pos);
    explicit Text(const QPointF &pos, const QString &text = "Text");
    ~Text();

    int type() const { return Type; }
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

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
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

#endif // TEXT_H
