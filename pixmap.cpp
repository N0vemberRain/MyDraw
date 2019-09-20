#include "pixmap.h"


PixMap::PixMap() : QObject(), QGraphicsRectItem(), mRect(QRect(0, 0, 1280, 640)),
mPen(new QPen(Qt::gray, 1)) {
    double dx = 0;
    double dy = 0;
    double tx = 4.4;    // 10 * 0.44
    double ty = 6;      // 10 * 0.6
    for(int i = 0; i < 128; i++) {
        for(int j = 0; j < 64; j++) {
            pixels[i][j].setCoords(i, j);
            pixels[i][j].setImgCoords(dx, dy);
            dy += ty;
        }
        dy = 0;
        dx += tx;
    }

    dx = dy = 0;
    for(int i = 0; i < 65; i++) {
        lines.append(QLineF(dx, dy + ty, 563, dy + ty));
        dy += ty;
    }
    dy = 0;
    for(int i = 0; i < 128; i++) {
        lines.append(QLineF(dx + tx, dy, dx + tx, 384));
        dx += tx;
    }
}

QPair<int, int> PixMap::findDot(const QPointF &p) {
    for (int i = 0; i < 128; ++i) {
        for (int j = 0; j < 64; ++j) {
            if(pixels[i][j].getRect().contains(p)) {
                 return qMakePair(pixels[i][j].getX(), pixels[i][j].getY());
            }
        }
    }
    return qMakePair(0, 0);
}

void PixMap::start() {
    for (int i = 0; i < 128; ++i) {
        for (int j = 0; j < 64; ++j) {
            pixels[i][j].getRect();
        }
    }
}

void PixMap::addPoint(const int x, const int y) {
//    pixels[x][y].setState(true);
}

void PixMap::addLine(const QVector<QPair<int, int>> line) {
    for(const auto &it : line) {
        addPoint(it.first, it.second);
        //auto coords = it.getCoords();
        //addPoint(coords.first, coords.second);
    }
}

void PixMap::setScene(QGraphicsScene *scene) {
    for(int i = 0; i < 128; i++) {
        for(int j = 0; j < 64; j++) {
            scene->addItem(&pixels[i][j]);
        }
    }
    mScene = scene;
}

void PixMap::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
           QWidget *widget) {
    painter->setPen(*mPen);
    painter->drawRect(mRect);
  /*  for(int i = 0; i < 128; i++) {
        for(int j = 0; j < 64; j++) {
            mScene->addItem(&pixels[i][j]);
        }
    }*/
    //painter->drawLines(lines);
  /*  for(int i = 0; i < 128; i++) {
        for(int j = 0; j < 64; j++) {
            auto r = pixels[i][j].getRect();
            painter->drawRect(r);
        }
    }*/
    //painter->drawPoints(nodes.constData(), nodes.count());
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void PixMap::createImageMap() {
    imgmap.setWidth(static_cast<int>(4.4 * 128));
    imgmap.setHeight(6 * 64);

}

void PixMap::addObject(QObject *item) {
    items.append(item);
    /*auto obj = qobject_cast<QGraphicsItem*>(item);
    switch (obj->type()) {
    case DotType: qgraphicsitem_cast<Dot*>(obj);
        break;
    case LineType: qgraphicsitem_cast<LineDot*>(obj)->getLine();
        break;
    case CircleType: qgraphicsitem_cast<CircleDot*>(obj);
        break;
    }*/

}

void PixMap::check() {
}

/////////////////////////////////////////////////////////////////////////////////////////////

PixMap2::PixMap2() : QObject(), QGraphicsItem(), mBoundingRect(QRect(0, 0, 1280, 640)),
mPen(new QPen(Qt::gray, 1)) {
    double dx = 0;
    double dy = 0;
    double tx = 4.4;    // 10 * 0.44
    double ty = 6;      // 10 * 0.6
    for(int i = 0; i < 128; i++) {
        for(int j = 0; j < 64; j++) {
            pixels[i][j].setTopLeft(QPointF(dx, dy));
            pixels[i][j].setWidth(4.4);
            pixels[i][j].setHeight(6);
            //pixels[i][j].setImgCoords(dx, dy);
            dy += ty;
        }
        dy = 0;
        dx += tx;
    }

   /* dx = dy = 0;
    for(int i = 0; i < 65; i++) {
        lines.append(QLineF(dx, dy + ty, 563, dy + ty));
        dy += ty;
    }
    dy = 0;
    for(int i = 0; i < 128; i++) {
        lines.append(QLineF(dx + tx, dy, dx + tx, 384));
        dx += tx;
    }*/
}

QPair<int, int> PixMap2::findDot(const QPointF &p) {
    for (int i = 0; i < 128; ++i) {
        for (int j = 0; j < 64; ++j) {
            if(pixels[i][j].contains(p)) {
                 return qMakePair(i, j);
            }
        }
    }
    return qMakePair(0, 0);
}

QRectF PixMap2::findPixRect(const QPointF &p) {
    for (int i = 0; i < 128; ++i) {
        for (int j = 0; j < 64; ++j) {
            if(pixels[i][j].contains(p)) {
                 return QRectF(pixels[i][j]);
            }
        }
    }
    return QRectF();
}

void PixMap2::start() {
    for (int i = 0; i < 128; ++i) {
        for (int j = 0; j < 64; ++j) {
           // pixels[i][j].getRect();
        }
    }
}

void PixMap2::addPoint(const int x, const int y) {
//    pixels[x][y].setState(true);
}

void PixMap2::addLine(const QVector<QPair<int, int>> line) {
    for(const auto &it : line) {
        addPoint(it.first, it.second);
        //auto coords = it.getCoords();
        //addPoint(coords.first, coords.second);
    }
}

void PixMap2::setScene(QGraphicsScene *scene) {
    /*for(int i = 0; i < 128; i++) {
        for(int j = 0; j < 64; j++) {
            scene->addItem(&pixels[i][j]);
        }
    }
    mScene = scene;*/
}

void PixMap2::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
           QWidget *widget) {
    painter->setPen(QPen(Qt::black, 0.5));
    painter->drawRect(mBoundingRect);
    for(int i = 0; i < 128; i++) {
        for(int j = 0; j < 64; j++) {
            painter->drawRect(pixels[i][j]);
        }
    }
    //painter->drawLines(lines);
  /*  for(int i = 0; i < 128; i++) {
        for(int j = 0; j < 64; j++) {
            auto r = pixels[i][j].getRect();
            painter->drawRect(r);
        }
    }*/
    //painter->drawPoints(nodes.constData(), nodes.count());
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

