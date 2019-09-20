#ifndef DOTSCENE_H
#define DOTSCENE_H

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

/*

class DotScene : public QGraphicsScene {
    Q_OBJECT
public:
    explicit DotScene(QObject *parent = nullptr);
    ~DotScene();

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
    void drawPixMap();
    void drawGrid();
    void drawPoint();
    void drawLine();
    void drawCircle();
    void drawCircle(const double radius);
    void drawRect();
    void drawRect(const double width, const double height);
    void drawPolyline();
    void drawText();
    void drawText(const QString &text, const QString &fontHeight);

    void setPointData(const QStringList &data);
    void setLineData(const QStringList &data);
    void setRectData(const QStringList &data);
    void setCircleData(const QStringList &data);
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
    PixMap map;
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
};*/

#endif // DOTSCENE_H
