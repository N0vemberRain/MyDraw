#include "scene.h"

#include <QMessageBox>
#include <QGraphicsView>

Scene::Scene(QObject *parent )
    : QGraphicsScene (parent)
{
    setSceneRect(QRectF(0, 0, 440, 320));
    waitingPoint = false;
    targetItem = false;
    oneItemSelect = true;
    bindState = true;
    gridState = true;
    mode = Mode::Normal;
    //rubberBand = new QRubberBand(QRubberBand::Rectangle);
    drawGrid();
    //drawPixMap();
    this->setItemIndexMethod(QGraphicsScene::NoIndex); // настраиваем индексацию элементов
    addItem(&selectArea);
    addRect(QRect(10, 10, 420, 297), QPen(Qt::white, 2));
    addRect(this->sceneRect(), QPen(Qt::white, 1));
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
        item->setSelected(false);
        /*if(auto itemObj = dynamic_cast<QObject*>(item)) {
            auto metaObj = itemObj->metaObject();
            metaObj->invokeMethod(itemObj, "select", Qt::DirectConnection,
                                  Q_ARG(bool, false));
        }*/
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
                case TextType: qgraphicsitem_cast<TextItem*>(item)->setMouseEvent(pe);
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
    //emit getPointSignal(pos);
    waitingPoint = true;
    switch (m_type) {
    case ItemType::Point: drawPoint(); break;
    case ItemType::Line: drawLine(); break;
    case ItemType::Circle: drawCircle(); break;
    case ItemType::Rect: drawRect(); break;
    case ItemType::Polyline: drawPolyline(); break;
    case ItemType::Text: drawText(); break;
    }
    update(sceneRect());
    emit getPointSignal(pos);
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
   // pointsVec.append(pe->scenePos());
//    drawLine();
    //drawPoint();
    switch (mode) {
    case Mode::Normal: inNormalMode(pe);
        break;
    case Mode::Input: inInputMode(pe);
        break;
    case Mode::Edit: Q_UNUSED(pe);
        break;
    }

    this->update();
}

void Scene::tmpRendering(QGraphicsSceneMouseEvent *event) {
    if(pointsVec.count() != 1) {
        return;
    }

    switch (mCurrentItem->type()) {
        case LineType: qgraphicsitem_cast<Line*>(mCurrentItem)->setEnd(event->scenePos());
            break;
        case RectType: qgraphicsitem_cast<RectItem*>(mCurrentItem)->setEnd(event->scenePos());
            break;
        case CircleType: qgraphicsitem_cast<CircleItem*>(mCurrentItem)->setRadius(event->scenePos());
            break;
    }

    update(sceneRect());
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    auto m = event->buttons();
    if(m == Qt::NoButton) {
        if(mode == Mode::Input) {
            tmpRendering(event);
        } else if(mode == Mode::Normal) {
            QGraphicsSceneHoverEvent *hover = new QGraphicsSceneHoverEvent;
            hover->setScenePos(event->scenePos());
            foreach(auto item, items()) {
                switch (item->type()) {
                case LineType: qgraphicsitem_cast<Line*>(item)->setHoverEvent(hover);
                    break;
                case RectType: qgraphicsitem_cast<RectItem*>(item)->setHoverEvent(hover);
                    break;
                case CircleType: qgraphicsitem_cast<CircleItem*>(item)->setHoverEvent(hover);
                    break;
                }
            }
        }
        return;
    }
    if(m == Qt::LeftButton) {
        foreach(auto item, this->items()) {
            if(mode == Mode::Select) {
                selectArea.setMouseEvent(event);
                continue;
            }
            auto type = item->type();
            if(type == DotType) {
                int d = 5;
            }
            if(item->type() != GridType && item->type() != SelectType && item->type() != DotType) {
                if(item->boundingRect().contains(event->scenePos())) {
                    targetItem = true;
                    //auto r = itemsBoundingRect();
                    //addRect(r);
                    switch (m_type) {
                    case ItemType::Rect: qgraphicsitem_cast<RectItem*>(item)->setMouseEvent(event);
                        break;
                    case ItemType::Circle: qgraphicsitem_cast<CircleItem*>(item)->setMouseEvent(event);
                        break;
                    case ItemType::Line: qgraphicsitem_cast<Line*>(item)->setMouseEvent(event);
                        break;
                    case ItemType::Text: qgraphicsitem_cast<TextItem*>(item)->setMouseEvent(event);
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
                item->setSelected(true);
                /*if(auto itemObj = dynamic_cast<QObject*>(item)) {
                    auto metaObj = itemObj->metaObject();
                    metaObj->invokeMethod(itemObj, "select", Qt::DirectConnection,
                                          Q_ARG(bool, true));
                } */               //item->setSelected(true);
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
    this->addItem(&grid);
}

void Scene::drawPixMap() {
    map.setScene(this);
    this->addItem(&map);
    map.start();
    update();
}

void Scene::drawText() {
    if(tmpText.isEmpty()) {
        waitingPoint = false;
        emit textPos();
        return;
    }
    //Text text(this, pointsVec.at(0), "Text");
    //this->addItem(new Text(pointsVec.at(0), tmpText));

    TextItem item(pointsVec.at(0), "Text", "11");
    this->addItem(&item);
    pointsVec.clear();
    waitingPoint = false;
    emit endInputSignal();
}

void Scene::drawText(const QString &text, const QString &fontHeight) {
    //Text t(this, pointsVec.at(0), text);

    //this->addItem(new Text(pointsVec.at(0), text));
    TextItem *textItem = new TextItem(pointsVec.at(0), text, fontHeight);
    this->addItem(textItem);
    pointsVec.clear();
    waitingPoint = false;
    emit endInputSignal();

    auto r = textItem->boundingRect();
    auto tl = r.topLeft();
    auto w = r.width();
    auto h = r.height();
    int d = 5;
}

QPair<int, int> Scene::findDot(const QPointF &p) {
    return map.findDot(p);
}

QRectF Scene::findPixRect(const QPointF &p) {
    return map.findPixRect(p);
}

void Scene::drawPoint() {
   // auto point = new Point(pointsVec.at(0));
    //this->addItem(new Point(pointsVec.at(0)));

    auto xy = findDot(pointsVec.at(0));
    auto r = findPixRect(pointsVec.at(0));
    Dot *dot = new Dot(xy.first, xy.second, r.topLeft().x(), r.topLeft().y());
    //map.addPoint(coords.first, coords.second);
    this->addItem(dot);
    //auto n = items();
   // auto c = new CheckRect(xy.first, xy.second, r.topLeft().x(), r.topLeft().y());
    //addItem(c);
    //addItem(new CheckRect);
    pointsVec.clear();
    emit endInputSignal();
    update();
}

void Scene::slotAddPoint(const int x, const int y) {
    map.addPoint(x, y);
}

/*void Scene::drawLine() {
    if(waitingPoint) {
        if(pointsVec.count() == 2) {
            Line *l = new Line(pointsVec.at(0), pointsVec.at(1));
            this->addItem(l);
            pointsVec.clear();
            waitingPoint = false;
            emit endInputSignal();
        } else {
            waitingPoint = true;
        }
    }
}
*/
void Scene::drawLine() {
    if(waitingPoint) {
            if(pointsVec.count() == 2) {
                //auto p1 = findDot(pointsVec.at(0));
                //auto p2 = findDot(pointsVec.at(1));
                //auto l = new Line(pointsVec.at(0), pointsVec.at(1));
                auto l = qgraphicsitem_cast<Line*>(mCurrentItem);   //!!!
                l->setEnd(pointsVec.at(1));
                l->setState(ItemState::Normal);
                //LineDot *line = new LineDot(p1.first, p1.second, p2.first, p2.second);
                //auto points = line->getLine();
                //map.addLine(points);
                //addItem(l);
                mCurrentState = new AddingState(mCurrentItem);
              //  addRect(line->getRect());
                pointsVec.clear();
                waitingPoint = false;
                emit endInputSignal();
            } else {
                mCurrentItem = new Line();
                auto l = qgraphicsitem_cast<Line*>(mCurrentItem);   //!!!
                l->setBegin(pointsVec.at(0));
                l->setState(ItemState::Rendering);
                addItem(mCurrentItem);
                waitingPoint = true;
            }
        }
}

void Scene::drawDotLine() {
    if(waitingPoint) {
        if(pointsVec.count() == 2) {
            //auto p1 = findDot(pointsVec.at(0));
            //auto p2 = findDot(pointsVec.at(1));
            LineDot *line = new LineDot(static_cast<int>(pointsVec.at(0).x()),
                                        static_cast<int>(pointsVec.at(0).y()),
                                        static_cast<int>(pointsVec.at(1).x()),
                                        static_cast<int>(pointsVec.at(1).y()));
            auto points = line->getLine();
            map.addLine(points);
            pointsVec.clear();
            waitingPoint = false;
            emit endInputSignal();
        } else {
            waitingPoint = true;
        }
    }
}

/*void Scene::drawCircle() {
    if(!waitingPoint) {
        return;
    }

    if(pointsVec.count() == 2) {
        //Circle *c = new Circle(pointsVec.at(0), pointsVec.at(1));
        CircleItem *c = new CircleItem(pointsVec.at(0), pointsVec.at(1));
        this->addItem(c);
        pointsVec.clear();
        waitingPoint = false;
        emit endInputSignal();
    } else {
        waitingPoint = true;
    }
}*/

void Scene::drawCircle() {
    if(!waitingPoint) {
        return;
    }

    if(pointsVec.count() == 2) {
            //Circle *c = new Circle(pointsVec.at(0), pointsVec.at(1));
            //CircleItem *c = new CircleItem(pointsVec.at(0), pointsVec.at(1));
            auto c = qgraphicsitem_cast<CircleItem*>(mCurrentItem);
            c->setRadius(pointsVec.at(1));
            c->setState(ItemState::Normal);
            pointsVec.clear();
            waitingPoint = false;
            emit endInputSignal();
        } else {
            mCurrentItem = new CircleItem();
            auto c = qgraphicsitem_cast<CircleItem*>(mCurrentItem);
            c->setOrigin(pointsVec.at(0));
            c->setState(ItemState::Rendering);
            addItem(c);
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
    mCurrentState = new AddingState(c);
    pointsVec.clear();
    waitingPoint = false;
    emit endInputSignal();
}

void Scene::drawDotCircle(const int radiaus) {
    if(!waitingPoint) {
        return;
    }

    //auto p0 = findDot(pointsVec.at(0));
    CircleDot *c = new CircleDot(static_cast<int>(pointsVec.at(0).x()),
                                 static_cast<int>(pointsVec.at(0).y()), radiaus);
    map.addLine(c->getPoints());
    pointsVec.clear();
    waitingPoint = false;
    emit endInputSignal();
}

void Scene::drawRect() {
    if(!waitingPoint) {
        return;
    }
    if(pointsVec.count() == 2) {
        auto r = qgraphicsitem_cast<RectItem*>(mCurrentItem);
        r->setEnd(pointsVec.at(1));
        r->setState(ItemState::Normal);
        mCurrentState = new AddingState(r);
        pointsVec.clear();
        waitingPoint = false;
        emit endInputSignal();
    } else {
        mCurrentItem = new RectItem();
        auto r = qgraphicsitem_cast<RectItem*>(mCurrentItem);
        r->setBegin(pointsVec.at(0));
        r->setState(ItemState::Rendering);
        addItem(mCurrentItem);
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
    mCurrentState = new AddingState(r);
    pointsVec.clear();
    waitingPoint = false;
    emit endInputSignal();
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

    waitingPoint = true;
    pointsVec.clear();
    switch (m_type) {
    case ItemType::Point: setPointData(list); break;
    case ItemType::Line: setLineData(list); break;
    case ItemType::Circle: setCircleDotData(list); break;
    case ItemType::Rect: setRectData(list); break;
    case ItemType::Polyline: drawPolyline(); break;
    case ItemType::Text: setTextData(list); break;
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

void Scene::setLineDotData(const QStringList &data) {
    for(int i = 0; i < data.count(); i++) {
        if(data.at(i) == "P1" || data.at(i) == "P2") {
            int x = i + 1;
            int y = i + 2;
            pointsVec.append(QPointF(data.at(x).toInt(), data.at(y).toInt()));
        }
    }
    drawDotLine();
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

void Scene::setCircleDotData(const QStringList &data) {
    int radius = 0;
        for(int i = 0; i < data.count(); i++) {
            if(data.at(i) == "PO" || data.at(i) == "PR") {
                int x = i + 1;
                int y = i + 2;
                if(data.at(x).isEmpty() || data.at(y).isEmpty()) {
                    continue;
                }
                pointsVec.append(QPointF(data.at(x).toInt(), data.at(y).toInt()));
            }
            if(data.at(i) == "R") {
                int k = i + 1;
                radius = data.at(k).toInt();
            }
        }

        if(pointsVec.count() == 1) {
            drawDotCircle(radius);
        } else {
            drawCircle();
        }
}

void Scene::setTextData(const QStringList &data) {
    /*if(data.at(0) != TextType) {
        return;
    }*/


    QString text = "";
    QString fontHeight = "";
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
        if(data.at(i) == "FH") {
            fontHeight = data.at(i + 1);
        }
    }

    if(pointsVec.count() == 1) {
        drawText(text, fontHeight);
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


