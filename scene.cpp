#include "scene.h"

#include <QMessageBox>
#include <QGraphicsView>

Scene::Scene(QObject *parent )
    : QGraphicsScene (parent)
{
    setSceneRect(QRectF(0, 0, 128, 64));
    waitingPoint = false;
    targetItem = false;
    oneItemSelect = true;
    bindState = true;
    gridState = true;
    mode = Mode::Normal;
    //rubberBand = new QRubberBand(QRubberBand::Rectangle);
    drawGrid();
    this->setItemIndexMethod(QGraphicsScene::NoIndex); // настраиваем индексацию элементов
    addItem(&selectArea);
}

Scene::~Scene() {

}

bool Scene::oneItemSelectCheck() const {
    auto items = this->selectedItems();
    if(items.count() != 0) {
        return true;
    } else {
        return false;
    }
}

void Scene::selectOneItemMode(const bool state) {
    oneItemSelect = state;
}

void Scene::selectOff() {
    foreach(auto item, this->selectedItems()) {
        if(auto itemObj = dynamic_cast<QObject*>(item)) {
            auto metaObj = itemObj->metaObject();
            metaObj->invokeMethod(itemObj, "select", Qt::DirectConnection,
                                  Q_ARG(bool, false));
        }
    }
}

void Scene::selectOneItemMode(QGraphicsSceneMouseEvent *event) {
    selectOff();
    foreach(auto item, this->items()) {
        if(item->type() != GridType && item->type() != SelectType) {
            if(item->boundingRect().contains(event->scenePos())) {
                targetItem = true;
                switch (m_type) {
                case ItemType::Rect: qgraphicsitem_cast<RectItem*>(item)->setMouseEvent(event);
                    break;
                case ItemType::Circle: qgraphicsitem_cast<CircleItem*>(item)->setMouseEvent(event);
                    break;
                case ItemType::Line: qgraphicsitem_cast<Line*>(item)->setMouseEvent(event);
                    break;
                }
            }
        }
    }
}

void Scene::selectManyItemMode(QGraphicsSceneMouseEvent *event) {
    foreach(auto item, this->items()) {
        if(item->type() != GridType && item->type() != SelectType) {
            if(item->boundingRect().contains(event->scenePos())) {
                targetItem = true;
                switch (m_type) {
                case ItemType::Rect: qgraphicsitem_cast<RectItem*>(item)->setMouseEvent(event);
                    break;
                case ItemType::Circle: qgraphicsitem_cast<CircleItem*>(item)->setMouseEvent(event);
                    break;
                case ItemType::Line: qgraphicsitem_cast<Line*>(item)->setMouseEvent(event);
                    break;
                }
            }
        }
    }
}

void Scene::inNormalMode(QGraphicsSceneMouseEvent *pe) {
    if(pe->modifiers() & Qt::ControlModifier) {
      //  selectManyItemMode(pe);
    } else {
      //  selectOneItemMode(pe);
        selectOff();
    }

    foreach(auto item, this->items()) {
        if(item->type() != GridType && item->type() != SelectType) {
            if(item->boundingRect().contains(pe->scenePos())) {
                targetItem = true;
                auto t = item->type();
                switch (t) {
                case RectType: qgraphicsitem_cast<RectItem*>(item)->setMouseEvent(pe);
                    break;
                case CircleType: qgraphicsitem_cast<CircleItem*>(item)->setMouseEvent(pe);
                    break;
                case LineType: qgraphicsitem_cast<Line*>(item)->setMouseEvent(pe);
                    break;
                }
            }
        }
    }


    if(targetItem) {
        targetItem = false;
        return;
    } else {
        selectOff();
        selectArea.setMouseEvent(pe);
    }
}

void Scene::inInputMode(QGraphicsSceneMouseEvent *pe) {
    auto pos = pe->scenePos();
    if(bindState) {
        pos = checkBind(pos);
    }
    pointsVec.append(pos);
    waitingPoint = true;
    switch (m_type) {
    case ItemType::Point: drawPoint(); break;
    case ItemType::Line: drawLine(); break;
    case ItemType::Circle: drawCircle(); break;
    case ItemType::Rect: drawRect(); break;
    case ItemType::Polyline: drawPolyline(); break;
    case ItemType::Text: drawText(); break;
    }
}

void Scene::inSelectMode(QGraphicsSceneMouseEvent *pe) {        // Использовать
    if(mode == Mode::Edit || mode == Mode::Input) {
        QMessageBox::information(nullptr, "Внимание", "Завершите текущую операцию");
        return;
    }

    auto pos = pe->scenePos();
    auto items = this->items();
    foreach(auto item, items) {
        if(item->type() == GridType && item->type() != SelectType) {
            continue;
        }
        if(item->boundingRect().contains(pos)) {
            emit moveSignal(item);
            break;
        }
    }

  //  update();
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *pe) {

    switch (mode) {
    case Mode::Normal: inNormalMode(pe);
        /*foreach(auto item, this->items()) {
            if(item->type() == GridType) {
                continue;
            }

            if(item->boundingRect().contains(pe->scenePos())) {
                mode = Mode::Select;
                inSelectMode(pe);
            }
        }*/
        break;
    case Mode::Input: inInputMode(pe);
        break;
    case Mode::Edit: Q_UNUSED(pe);
        break;
    }

    this->update();
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    auto m = event->buttons();
    if(m == Qt::NoButton) {
        return;
    }
    if(m == Qt::LeftButton) {
        foreach(auto item, this->items()) {
            if(mode == Mode::Select) {
                selectArea.setMouseEvent(event);
                continue;
            }
            if(item->type() != GridType && item->type() != SelectType) {
                if(item->boundingRect().contains(event->scenePos())) {
                    targetItem = true;
                    switch (m_type) {
                    case ItemType::Rect: qgraphicsitem_cast<RectItem*>(item)->setMouseEvent(event);
                        break;
                    case ItemType::Circle: qgraphicsitem_cast<CircleItem*>(item)->setMouseEvent(event);
                        break;
                    case ItemType::Line: qgraphicsitem_cast<Line*>(item)->setMouseEvent(event);
                        break;
                    }
                }
            }
        }
        if(targetItem) {
            targetItem = false;
            this->update();
            return;
        } else {
            if(mode == Mode::Input) {
                return;
            }
            selectArea.setMouseEvent(event);
            mode = Mode::Select;
        }
    }

    this->update();
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *pe) {
    if(mode == Mode::Select) {
        auto area = selectArea.getArea();
        auto list = this->items();
        foreach(auto item, list) {
            if(item->type() == GridType && item->type() == SelectType) {
                continue;
            }
            auto scenePos = item->scenePos();
            auto pos2 = item->pos();
            if(area.contains(item->boundingRect())) {
                if(auto itemObj = dynamic_cast<QObject*>(item)) {
                    auto metaObj = itemObj->metaObject();
                    metaObj->invokeMethod(itemObj, "select", Qt::DirectConnection,
                                          Q_ARG(bool, true));
                }                //item->setSelected(true);
            }
        }
        mode = Mode::Normal;
    }
    if(selectArea.getArea().contains(pe->scenePos())) {

        selectArea.setMouseEvent(pe);
    }
    update();
}

void Scene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *pe) {
    if(mode == Mode::Edit || mode == Mode::Input) {
        QMessageBox::information(nullptr, "Внимание", "Завершите текущую операцию");
        return;
    }
    auto pos = pe->scenePos();
    auto items = this->items();
    foreach(auto item, items) {
        if(item->type() == GridType && item->type() != SelectType) {
            continue;
        }
        if(item->boundingRect().contains(pos)) {
            emit editSignal(item);
            break;
        }
    }

    update();
}

void Scene::drawGrid() {

   // auto gridArea = this->sceneRect();
   /* int x1 = 0, y1 = 0, x2 = 0, y2 = 64 , d = 0;

    for(int i = 0; i < 16; i++) {
        this->addLine(QLine(x1 + d, y1, x2 + d, y2), QPen(Qt::gray));
        d += 8;
    }

    d = 0; x1 = 0; x2 = 128; y1 = 0; y2 = 0;
    for (int i = 0; i < 8; i++) {
        this->addLine(QLine(x1, y1 + d, x2, y2 + d), QPen(Qt::gray));
        d += 8;
    }*/this->addItem(&grid);
}

void Scene::drawText() {
    this->addItem(new Text(pointsVec.at(0)));
    pointsVec.clear();
    waitingPoint = false;
}

void Scene::drawPoint() {
   // auto point = new Point(pointsVec.at(0));
    this->addItem(new Point(pointsVec.at(0)));
    pointsVec.clear();
    waitingPoint = false;
}

void Scene::drawLine() {
    if(waitingPoint) {
        if(pointsVec.count() == 2) {
            Line *l = new Line(pointsVec.at(0), pointsVec.at(1));
            this->addItem(l);
            pointsVec.clear();
            waitingPoint = false;
        } else {
            waitingPoint = true;
        }
    }
}

void Scene::drawCircle() {
    if(!waitingPoint) {
        return;
    }

    if(pointsVec.count() == 2) {
        //Circle *c = new Circle(pointsVec.at(0), pointsVec.at(1));
        CircleItem *c = new CircleItem(pointsVec.at(0), pointsVec.at(1));
        this->addItem(c);
        pointsVec.clear();
        waitingPoint = false;
    } else {
        waitingPoint = true;
    }
}

void Scene::drawCircle(const double radius) {
    if(!waitingPoint) {
        return;
    }

    //Circle *r = new Circle(pointsVec.at(0), radius);
    CircleItem *c = new CircleItem(pointsVec.at(0), radius);
    this->addItem(c);
    pointsVec.clear();
    waitingPoint = false;
}

void Scene::drawRect() {
    if(!waitingPoint) {
        return;
    }

    if(pointsVec.count() == 2) {
       // Rect *r = new Rect(pointsVec.at(0), pointsVec.at(1));
        RectItem *r = new RectItem(pointsVec.at(0), pointsVec.at(1));
        //r->setFlag(QGraphicsItem::ItemIsMovable);
        this->addItem(r);
        pointsVec.clear();
        waitingPoint = false;
    } else {
        waitingPoint = true;
    }
}

void Scene::drawRect(const double width, const double height) {
    if(!waitingPoint) {
        return;
    }

    auto bottomRightX = pointsVec.at(0).x() + width;
    auto bottomRightY = pointsVec.at(0).y() + height;

    //Rect *r = new Rect(pointsVec.at(0), QPointF(bottomRightX, bottomRightY));
    RectItem *r = new RectItem(pointsVec.at(0), QPointF(bottomRightX, bottomRightY));
    this->addItem(r);
    pointsVec.clear();
    waitingPoint = false;
}

void Scene::drawPolyline() {
    if(!waitingPoint) {
        return;
    }


}

void Scene::wheelEvent(QGraphicsSceneWheelEvent *pe) {
    const double factor = 1.15;
    auto v = this->views();
    foreach(QGraphicsView *view, v) {
        if(pe->delta() > 0) {
            view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
            view->scale(factor, factor);
        } else {
            view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
            view->scale(1 / factor, 1 / factor);
        }
    }

}

void Scene::addShape(const QStringList &list) {
    if(mode == Mode::Normal) {
        return;
    }

   /* for(int i = 0; i < list.count(); i++) {
        if(list.at(i) == "P") {
            int x = i + 1;
            int y = i + 2;
            pointsVec.append(QPointF(list.at(x).toDouble(), list.at(y).toDouble()));
        }
    }*/

    waitingPoint = true;
    pointsVec.clear();
    switch (m_type) {
    case ItemType::Point: setPointData(list); break;
    case ItemType::Line: setLineData(list); break;
    case ItemType::Circle: setCircleData(list); break;
    case ItemType::Rect: setRectData(list); break;
    case ItemType::Polyline: drawPolyline(); break;
    }

    this->update();
}

void Scene::setPointData(const QStringList &data) {
    for(int i = 0; i < data.count(); i++) {
        if(data.at(i) == "P") {
            int x = i + 1;
            int y = i + 2;
            pointsVec.append(QPointF(data.at(x).toDouble(), data.at(y).toDouble()));
        }
    }
    drawPoint();
}

void Scene::setLineData(const QStringList &data) {
    for(int i = 0; i < data.count(); i++) {
        if(data.at(i) == "P1" || data.at(i) == "P2") {
            int x = i + 1;
            int y = i + 2;
            pointsVec.append(QPointF(data.at(x).toDouble(), data.at(y).toDouble()));
        }
    }
    drawLine();
}

void Scene::setRectData(const QStringList &data) {
    double width = 0, height = 0;
    for(int i = 0; i < data.count(); i++) {
        if(data.at(i) == "P1" || data.at(i) == "P2") {
            int x = i + 1;
            int y = i + 2;
            if(data.at(x).isEmpty() || data.at(y).isEmpty()) {
                continue;
            }
            pointsVec.append(QPointF(data.at(x).toDouble(), data.at(y).toDouble()));
        }
        if(data.at(i) == "W") {
            int k = i + 1;
            width = data.at(k).toDouble();
        }
        if(data.at(i) == "H") {
            int k = i + 1;
            height = data.at(k).toDouble();
        }
    }
    if(pointsVec.count() == 1) {
        drawRect(width, height);
    } else {
        drawRect();
    }
}

void Scene::setCircleData(const QStringList &data) {
    double radius = 0;
    for(int i = 0; i < data.count(); i++) {
        if(data.at(i) == "PO" || data.at(i) == "PR") {
            int x = i + 1;
            int y = i + 2;
            if(data.at(x).isEmpty() || data.at(y).isEmpty()) {
                continue;
            }
            pointsVec.append(QPointF(data.at(x).toDouble(), data.at(y).toDouble()));
        }
        if(data.at(i) == "R") {
            int k = i + 1;
            radius = data.at(k).toDouble();
        }
    }

    if(pointsVec.count() == 1) {
        drawCircle(radius);
    } else {
        drawCircle();
    }
}

void Scene::setTextData(const QStringList &data) {
    if(data.at(0) != TextType) {
        return;
    }

    QString text = "";
    for(int i = 0; i < data.count(); i++) {
        if(data.at(i) == "P") {
            int x = i + 1;
            int y = i + 2;
            if(data.at(x).isEmpty() || data.at(y).isEmpty()) {
                continue;
            }
            pointsVec.append(QPointF(data.at(x).toDouble(), data.at(y).toDouble()));
        }
        if(data.at(i) == "T") {
            int k = i + 1;
            text = data.at(k);
        }
    }

    if(pointsVec.count() == 1) {
        drawText(text);
    } else {
        drawText();
    }
}

void Scene::bindActivate(const bool state) {
    if(bindState == state) {
        return;
    }

    bindState = state;
}

void Scene::gridActivate(const bool state) {
    if(gridState == state) {
        return;
    }

    gridState = state;
    if(state) {
        grid.show();
    } else {
        grid.hide();
    }
}

QPointF Scene::checkBind(QPointF &point) {
    foreach(auto node, grid.getNodes()) {
        QLineF tmp(point, node);
        if(tmp.length() < 5) {
            point = node;
            break;
        }
    }
    return point;
}


