#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QRubberBand>

#include "types.h"
#include "point.h"
#include "line.h"
#include "polyline.h"
#include "circle.h"
#include "rect.h"
#include "selectarea.h"
#include "text.h"
#include "pixmap.h"


class Grid : public QObject, public QGraphicsRectItem {
    Q_OBJECT
public:
    enum { Type = GridType };
    explicit Grid() : QObject(), QGraphicsRectItem() {
        m_rect = QRect(0, 0, 128, 64);
        m_pen = new QPen(Qt::gray, 1);

        int x1 = 0, y1 = 0, x2 = 0, y2 = 64 , d = 0;

        for(int i = 0; i < 16; i++) {
            lines.append(QLine(x1 + d, y1, x2 + d, y2));
           /* for(int j = 0; j < 8; j++) {
                int dy = y1 + d;
                nodes.append(QPoint(x1 + d, dy));
            }*/
            d += 8;
        }

        d = 0; x1 = 0; x2 = 128; y1 = 0; y2 = 0;
        for (int i = 0; i < 8; i++) {
            lines.append(QLine(x1, y1 + d, x2, y2 + d));
            d += 8;
        }

        setNodes();

    }

    int type() const { return Type; }
    ~Grid() {}

    QRectF boundingRect() const {
        qreal penWidth = 1;
        return QRectF(m_rect.topLeft(), m_rect.bottomRight());
        //return QRectF(-10 - penWidth / 2, -10 - penWidth / 2, 20 + penWidth, 20 + penWidth);
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) {
        painter->setPen(*m_pen);
        painter->drawRect(m_rect);
        painter->drawLines(lines);
        painter->drawPoints(nodes.constData(), nodes.count());
        Q_UNUSED(option);
        Q_UNUSED(widget);
    }

    QVector<QPoint> getNodes() const {
        return nodes;
    }

protected:

private:
    void setNodes() {
        for(int i = 0; i <= 16; i++) {
            auto line = lines.at(i);
            int x, y, dx, dy;
            x = line.p1().x();
            y = line.p1().y();
            dx = 0; dy = 0;
            for(int j = 0; j <= 8; j++) {
                nodes.append(QPoint(x + dx, y + dy));
                dy += 8;
            }
            dx += 8;
        }
    }
    QVector<QLine> lines;
    QVector<QPoint> nodes;
    QRect m_rect;
    QPen   *m_pen;
};

class Scene : public QGraphicsScene {
    Q_OBJECT
public:
    explicit Scene(QObject *parent = nullptr);
    ~Scene();

    void addShape(const QStringList &list);
    void setTypeShape(ItemType type) { m_type = type; }
    ItemType getTypeShape() const { return m_type; }
    void setTypeMode(const Mode type) {
        mode = type;
        if (type == Mode::Normal) {
            waitingPoint = false;
        } else {
            waitingPoint = true;
        }
    }
    void clearAll() {
        foreach(auto item, this->items()) {
            if(item->type() == GridType || item->type() == SelectType) {
                continue;
            }
            this->removeItem(item);
            delete item;
        }
        pointsVec.clear();
        this->update();
    }

    void bindActivate(const bool state);
    void gridActivate(const bool state);
    //QVector<QPoint> getNodes() const;
    void setText(const QString &text) {
        tmpText = text;
    }
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *pe);
    void wheelEvent(QGraphicsSceneWheelEvent *pe);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *pe);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *pe);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *pe);

private slots:
    void slotAddPoint(const int x, const int y);
private:
    void inNormalMode(QGraphicsSceneMouseEvent *pe);
    void inInputMode(QGraphicsSceneMouseEvent *pe);
    void inSelectMode(QGraphicsSceneMouseEvent *pe);

    QPair<int, int> findDot(const QPointF &p);
    QRectF findPixRect(const QPointF &p);
    void drawPixMap();
    void drawGrid();
    void drawPoint();
    void drawLine();
    void drawDotLine();
    void drawCircle();
    void drawCircle(const double radius);
    void drawDotCircle(const int radiaus);
    void drawRect();
    void drawRect(const double width, const double height);
    void drawPolyline();
    void drawText();
    void drawText(const QString &text, const QString &fontHeight);

    void setPointData(const QStringList &data);
    void setLineData(const QStringList &data);
    void setLineDotData(const QStringList &data);
    void setRectData(const QStringList &data);
    void setCircleData(const QStringList &data);
    void setCircleDotData(const QStringList &data);
    void setTextData(const QStringList &data);

    bool oneItemSelectCheck() const;
    void selectOneItemMode(const bool state);

    void selectOneItemMode(QGraphicsSceneMouseEvent *event);
    void selectManyItemMode(QGraphicsSceneMouseEvent *event);
    void selectOff();

    QPointF checkBind(QPointF &point);

  //  float distance(const QPointF &p1, const QPointF &p2);

    QVector<QPointF> pointsVec;
    ItemType m_type;
    bool waitingPoint;
    bool targetItem;
    bool oneItemSelect;
    bool oneItemMode;
    bool bindState;
    bool gridState;
    Mode mode;
    Grid grid;
    PixMap2 map;
    QPointF m_previousPosition;
    //QRubberBand rubberBand;
    SelectArea selectArea;
    //void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    bool waitingText;
    QString tmpText;

signals:
    QGraphicsItem* editSignal(QGraphicsItem *item);
    QGraphicsItem* moveSignal(QGraphicsItem *item);
    void textPos();
    QPointF getPointSignal(const QPointF &p);
    void endInputSignal();
};

#endif // SCENE_H
