#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPixmap>
#include <QBuffer>
#include <QByteArray>
#include <QFile>
#include <QMessageBox>
#include <math.h>

#include "types.h"

//Line::Line(const QPointF &p1, const QPointF &p2)
  //  : points.


static int randomBetween(int low, int high)
{
    return (qrand() % ((high + 1) - low) + low);
}





DrawFactory::DrawFactory(QObject *parent)
    : QObject (parent) {
    state = true;
}

DrawFactory::~DrawFactory() {

}

void DrawFactory::draw(const ItemType type, QGraphicsScene *scene, QGraphicsItem *item) {

}



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //m_scene = new QGraphicsScene(QRect(0, 0, 100, 100));
    m_scene = new Scene;
    connect(m_scene, SIGNAL(editSignal(QGraphicsItem*)), this, SLOT(slotEditItem(QGraphicsItem*)));
  //  m_factory = new DrawFactory(this);
    ui->graphicsView->setScene(m_scene);
    ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    ui->graphicsView->scale(4, 4);
    QHBoxLayout *layout = new QHBoxLayout;
    ui->widget->setLayout(layout);

    connect(ui->pointBut, SIGNAL(clicked()), this, SLOT(slotPointBut()));
    connect(ui->lineBut, SIGNAL(clicked()), this, SLOT(slotLineBut()));
    connect(ui->polylineBut, SIGNAL(clicked()), this, SLOT(slotPolylineBut()));
    connect(ui->circleBut, SIGNAL(clicked()), this, SLOT(slotCircleBut()));
    connect(ui->rectBut, SIGNAL(clicked()), this, SLOT(slotRectBut()));
    connect(ui->textBut, SIGNAL(clicked()), this, SLOT(slotTextBut()));

    connect(ui->saveViewBut, SIGNAL(clicked()), this, SLOT(slotSaveView()));
    connect(ui->saveSceneBut, SIGNAL(clicked()), this, SLOT(slotSaveScene()));

    connect(ui->moveBut, SIGNAL(clicked()), this, SLOT(slotMoveItem()));
    connect(ui->delBut, SIGNAL(clicked()), this, SLOT(slotDeleteSelected()));
    connect(ui->clearBut, SIGNAL(clicked()), this, SLOT(slotClearBut()));
    connect(ui->bindBox, SIGNAL(stateChanged(int)), this, SLOT(slotBind(int)));
    connect(ui->gridBox, SIGNAL(stateChanged(int)), this, SLOT(slotGrid(int)));
    connect(ui->setScaleBut, SIGNAL(clicked()), this, SLOT(slotSetScale()));

    connect(ui->checkBut, SIGNAL(clicked()), this, SLOT(slotCheck()));
    connect(ui->checkRectBut, SIGNAL(clicked()), this, SLOT(slotCheckRect()));
    connect(ui->checkCircleBut, SIGNAL(clicked()), this, SLOT(slotCheckCircle()));
   // connect(m_scene, SIGNAL(moveSignal(QGraphicsItem*)), this, SLOT(slotSelect(QGraphicsItem*)));
    //auto x = m_scene->addLine(QLine(0, 0, 100, 0), QPen(Qt::black, 5));
    //auto y = m_scene->addLine(QLine(0, 0, 0, 100), QPen(Qt::red, 5));

    //m_scene->addRect(QRectF(0, 0, 128, 64), QPen(Qt::red, 2));

    ui->bindBox->setCheckState(Qt::CheckState::Checked);
    ui->gridBox->setCheckState(Qt::CheckState::Checked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotSetScale() {
    double sx = ui->xEdit->text().toDouble();
    double sy = ui->yEdit->text().toDouble();
    ui->graphicsView->scale(sx, sy);
}

void MainWindow::slotSaveScene() {
//    m_scene->gridActivate(false);
    QPixmap img;// = QPixmap::grabWidget(ui->graphicsView);

    auto r = ui->graphicsView->sceneRect().toRect();
    r.setWidth(- r.width());
    auto tl = r.topLeft();

    img = QWidget::grab(QRect(r.topLeft(), QSize(-128, 64)));

    auto r2 = img.rect();
    QByteArray bytes;
    QFile file("scene.xbm");

    file.open(QIODevice::WriteOnly);

    QPainter pixPainter(&img);

    img.save(file.fileName(), "xbm");
    m_scene->gridActivate(true);

    /*
    auto r = ui->graphicsView->sceneRect().toRect();
    QImage img(r.width(), r.height(), QImage::Format_Mono);
    QPainter painter(&img);
    m_scene->render(&painter, QRectF(0, 0, r.width(), r.height()), QRect(0, 0, r.width(), r.height()));

    QFile file("scene.xbm");
    file.open(QIODevice::WriteOnly);
    img.save(file.fileName(), "xbm");
*/
}

void MainWindow::slotSaveView() {
    m_scene->gridActivate(false);
    ui->graphicsView->scale(0.25, 0.25);
    auto r = ui->graphicsView->sceneRect().toRect();
    auto pos = QPoint(ui->graphicsView->pos().x(), ui->graphicsView->pos().y() + 8);
    r.setTopLeft(pos);
    r.setWidth(130);
    r.setHeight(70);
    QPixmap img = QWidget::grab(r);
    QFile file("view.xbm");
    file.open(QIODevice::WriteOnly);
    QPainter pixPainter(&img);
    img.save(file.fileName(), "xbm");
    m_scene->gridActivate(true);
    ui->graphicsView->scale(4, 4);
}

void MainWindow::slotClearBut() {
    m_scene->clearAll();
}

/*void MainWindow::startInput(const ItemType type) {
    m_scene->setTypeMode(mode);
    this->type = type;
    changeType(type)
    wgt = new WorkWidget(this, type);
 //   QHBoxLayout *layout = new QHBoxLayout;
   // layout->addWidget(wgt);
    //ui->widget->setLayout(layout);
    auto l = ui->widget->layout();
    l->addWidget(wgt);
    connect(wgt, SIGNAL(stop()), this, SLOT(slotStop()));
    connect(wgt, SIGNAL(create()), this, SLOT(slotCreate()));

    if(m_mode == Mode::Edit) {
        QStringList data;
        auto type = currentItem->type();
        switch (type) {
        case RectType: data = qgraphicsitem_cast<Rect*>(currentItem)->getData();
            //data = item->getData();
            wgt->setItemData(data);
            break;
        }
    }
}*/

void MainWindow::startInput() {
    //m_scene->setTypeMode(m_mode);
    wgt = new WorkWidget(this, m_type);
    auto l = ui->widget->layout();
    l->addWidget(wgt);
    connect(wgt, SIGNAL(stop()), this, SLOT(slotStop()));
    connect(wgt, SIGNAL(create()), this, SLOT(slotCreate()));

    QStringList data;
    if(m_mode == Mode::Edit) {
        auto type = currentItem->type();
        switch (type) {
        case RectType: data = qgraphicsitem_cast<RectItem*>(currentItem)->getData();
            break;
        case CircleType: data = qgraphicsitem_cast<CircleItem*>(currentItem)->getData();
            break;
        case LineType: data = qgraphicsitem_cast<Line*>(currentItem)->getData();
            break;
        case PolylineType: data = qgraphicsitem_cast<Polyline*>(currentItem)->getData();
            break;
        case TextType: data = qgraphicsitem_cast<Text*>(currentItem)->getData();
         //   qgraphicsitem_cast<Text*>(currentItem)->edit();
            break;
        }
        currentData = data;
        wgt->setItemData(data);
    }
}

void MainWindow::slotTextBut() {
    if(m_mode == Mode::Input || m_mode == Mode::Edit) {
        QMessageBox::information(this, "Внимание", "Завершите текущую операцию");
        return;
    }
    changeType(ItemType::Text);
    changeMode(Mode::Input);
    startInput();
}

void MainWindow::slotPointBut() {
    if(m_mode == Mode::Input || m_mode == Mode::Edit) {
        QMessageBox::information(this, "Внимание", "Завершите текущую операцию");
        return;
    }

    changeMode(Mode::Input);
    changeType(ItemType::Point);
}
void MainWindow::slotLineBut() {
    if(m_mode == Mode::Input || m_mode == Mode::Edit) {
        QMessageBox::information(this, "Внимание", "Завершите текущую операцию");
        return;
    }

    changeMode(Mode::Input);
    changeType(ItemType::Line);
    startInput();
}
void MainWindow::slotCircleBut() {
    if(m_mode == Mode::Input || m_mode == Mode::Edit) {
        QMessageBox::information(this, "Внимание", "Завершите текущую операцию");
        return;
    }

    changeMode(Mode::Input);
    changeType(ItemType::Circle);
    startInput();
}
void MainWindow::slotRectBut() {
    if(m_mode == Mode::Input || m_mode == Mode::Edit) {
        QMessageBox::information(this, "Внимание", "Завершите текущую операцию");
        return;
    }

    changeMode(Mode::Input);
    changeType(ItemType::Rect);
    startInput();
}
void MainWindow::slotPolylineBut() {
    if(m_mode == Mode::Input || m_mode == Mode::Edit) {
        QMessageBox::information(this, "Внимание", "Завершите текущую операцию");
        return;
    }

    changeMode(Mode::Input);
    changeType(ItemType::Polyline);
    startInput();
}

void MainWindow::slotStop() {
    if(m_mode == Mode::Edit) {
        m_scene->removeItem(currentItem);
        wgt->checkData();
        m_scene->addShape(currentData);
    }
    delete wgt;
    changeMode(Mode::Normal);
}

void MainWindow::slotCreate() {

    if(m_mode == Mode::Input) {
        auto data = wgt->getData(m_type);
        m_scene->setTypeMode(Mode::Input);
        m_scene->addShape(data);
    } else if(m_mode == Mode::Edit) {
        m_scene->removeItem(currentItem);
        auto data = wgt->getData(m_type);
        m_scene->setTypeMode(Mode::Edit);
        m_scene->addShape(data);
        delete wgt;
        changeMode(Mode::Normal);
    }

}

void MainWindow::slotEditItem(QGraphicsItem *item) {
    changeMode(Mode::Edit);
    currentItem = item;
    startInput();
    m_scene->update();
}

void MainWindow::changeMode(const Mode mode) {
    m_mode = mode;
    m_scene->setTypeMode(mode);
}

void MainWindow::changeType(const ItemType type) {
    m_type = type;
    m_scene->setTypeShape(type);
}

void MainWindow::slotSelect(QGraphicsItem *item) {
    changeMode(Mode::Select);
    currentItem = item;
    currentItem->setSelected(true);

        switch (currentItem->type()) {
        case RectType: qgraphicsitem_cast<Rect*>(currentItem)->setSelect();
            break;
        case MoveType: auto m = new MoveItem;
            m->setPos(randomBetween(30, 470),
                         randomBetween(30, 470));
            m_scene->addItem(m);

    }

    m_scene->update();
}

void MainWindow::slotMoveItem() {
    if(m_mode != Mode::Normal || m_scene->items().count() == 0) {
        return;
    }

    //Q_ASSERT(!m_scene->selectedItems().isEmpty());
    if(m_scene->selectedItems().isEmpty()) {
        return;
    }
    auto items = m_scene->selectedItems();
    auto item = items.at(0);
    //item->moveBy(ui->xEdit->text().toDouble(), ui->yEdit->text().toDouble());
  /*  QTransform shearTransform;
    shearTransform.translate(ui->xEdit->text().toDouble(), ui->yEdit->text().toDouble());
    item->setTransform(shearTransform);*/
    if(item->type() == RectType) {
        qgraphicsitem_cast<RectItem*>(item)->setShear(ui->xEdit->text().toDouble(), ui->yEdit->text().toDouble());
       // qgraphicsitem_cast<RectItem*>(item)->moveBy(ui->xEdit->text().toDouble(), ui->yEdit->text().toDouble());
    }
    changeMode(Mode::Normal);
    m_scene->update();
}

void MainWindow::slotCheck() {
    auto allItems = m_scene->items();
    foreach(auto item, allItems) {
    //    item->setSelected(true);
    }
    auto items = m_scene->selectedItems();

    int d = 5;
}

void MainWindow::slotCheckRect() {
    RectItem *item = new RectItem();
    item->setPos(randomBetween(0, 128),
                 randomBetween(0, 64));
    m_scene->addItem(item);
    changeType(ItemType::Rect);
}

void MainWindow::slotCheckCircle() {
   // CircleItem *item = new CircleItem();
   auto item = m_scene->addEllipse(QRect(randomBetween(0, 128),
                             randomBetween(0, 64), 30, 30));
   // item->setPos(randomBetween(0, 128),
     //             randomBetween(0, 64));
    //m_scene->addItem(item);
   item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
   item->setSelected(true);
    changeType(ItemType::Circle);
}

void MainWindow::slotDeleteSelected() {
    auto items = m_scene->selectedItems();
    foreach(auto item, items) {
        m_scene->removeItem(item);
        delete item;
    }
}

void MainWindow::slotBind(int state) {
    switch (state) {
    case Qt::Checked: m_scene->bindActivate(true);
        break;
    case Qt::Unchecked: m_scene->bindActivate(false);
        break;
    }
}

void MainWindow::slotGrid(int state) {
    switch (state) {
    case Qt::Checked: m_scene->gridActivate(true);
        break;
    case Qt::Unchecked: m_scene->gridActivate(false);
        break;
    }
}
