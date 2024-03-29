#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPixmap>
#include <QBuffer>
#include <QByteArray>
#include <QFile>
#include <QMessageBox>
#include <QMenuBar>
#include <QAction>
#include <QDockWidget>
#include <QMdiSubWindow>
#include <QFileDialog>
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
    createMenu();
    createToolBar();
    createMdiArea();

    //m_scene = new QGraphicsScene(QRect(0, 0, 100, 100));
 //   m_scene = new Scene;
   // connect(m_scene, SIGNAL(editSignal(QGraphicsItem*)), this, SLOT(slotEditItem(QGraphicsItem*)));
  //  m_factory = new DrawFactory(this);
    //ui->graphicsView->setScene(m_scene);
    //ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
//    ui->graphicsView->scale(4, 4);
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
   // connect(getScene(), SIGNAL(textPos()), this, SLOT(slotSetText()));
   // connect(m_scene, SIGNAL(moveSignal(QGraphicsItem*)), this, SLOT(slotSelect(QGraphicsItem*)));
    //auto x = m_scene->addLine(QLine(0, 0, 100, 0), QPen(Qt::black, 5));
    //auto y = m_scene->addLine(QLine(0, 0, 0, 100), QPen(Qt::red, 5));

    //m_scene->addRect(QRectF(0, 0, 128, 64), QPen(Qt::red, 2));

    //ui->bindBox->setCheckState(Qt::CheckState::Checked);
    //ui->gridBox->setCheckState(Qt::CheckState::Checked);
    slotBind(true);
    //slotGrid(true);

   // workWgtDock = new QDockWidget("Dock", this);
   // addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, workWgtDock);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createMenu() {
    QMenu *fileMenu = menuBar()->addMenu("&Файл");
    QMenu *editMenu = menuBar()->addMenu("&Правка");
    QMenu *geometryMenu = new QMenu("&Геометрия");
    QMenu *moveMenu = new QMenu("&Перемещение");

    newFileAction = new QAction(tr("&Новый"), this);
    openFileAction = new QAction(tr("&Открыть"), this);
    saveFileAction = new QAction(tr("&Сохранить"), this);
    closeFileActions = new QAction(tr("&Закрыть"), this);
    exitAction = new QAction("&Выход", this);

    pointAction = new QAction(tr("&Точка"), this);
    lineAction = new QAction(tr("&Прямая"), this);
    polylineAction = new QAction(tr("&Ломаная"), this);
    rectAction = new QAction(tr("&Прямоугольник"), this);
    circleAction = new QAction(tr("&Окружность"), this);
    textAction = new QAction(tr("&Текст"), this);

    removeAction = new QAction(tr("&Удалить"), this);
    removeAction->setShortcut(QKeySequence::Delete);
    removeAllAction = new QAction(tr("&Удалить все"), this);
    bindAction = new QAction(tr("&Привязка"), this);
    bindAction->setCheckable(true);
    shearAction = new QAction(tr("&Сдвиг"));
    boundAction = new QAction(tr("&Граница"));
    boundAction->setCheckable(true);
    boundAction->setChecked(false);
    gridAction = new QAction(tr("&Сетка"));
    boundAction->setCheckable(true);

    connect(newFileAction, SIGNAL(triggered()), this, SLOT(slotNewFile()));
    connect(openFileAction, SIGNAL(triggered()), this, SLOT(slotOpenFile()));
    connect(saveFileAction, SIGNAL(triggered()), this, SLOT(slotSaveFile()));
    connect(closeFileActions, SIGNAL(triggered()), this, SLOT(slotCloseFile()));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(slotExit()));

    connect(pointAction, SIGNAL(triggered()), this, SLOT(slotPointBut()));
    connect(lineAction, SIGNAL(triggered()), this, SLOT(slotLineBut()));
    connect(polylineAction, SIGNAL(triggered()), this, SLOT(slotPolylineBut()));
    connect(rectAction, SIGNAL(triggered()), this, SLOT(slotRectBut()));
    connect(circleAction, SIGNAL(triggered()), this, SLOT(slotCircleBut()));
    connect(textAction, SIGNAL(triggered()), this, SLOT(slotTextBut()));

    connect(removeAction, SIGNAL(triggered()), this, SLOT(slotRemove()));
    connect(removeAllAction, SIGNAL(triggered()), this, SLOT(slotRemoveAll()));
    connect(bindAction, SIGNAL(triggered(bool)), this, SLOT(slotBind(bool)));
    connect(shearAction, SIGNAL(triggered()), this, SLOT(slotShear()));
    connect(boundAction, SIGNAL(triggered(bool)), this, SLOT(slotBound(bool)));
    connect(gridAction, SIGNAL(triggered(bool)), this, SLOT(slotGrid(bool)));

    fileMenu->addAction(newFileAction);
    fileMenu->addAction(openFileAction);
    fileMenu->addAction(saveFileAction);
    fileMenu->addAction(closeFileActions);
    fileMenu->addAction(exitAction);

    editMenu->addMenu(geometryMenu);
    editMenu->addMenu(moveMenu);
    editMenu->addAction(removeAction);
    editMenu->addAction(removeAllAction);
    editMenu->addAction(bindAction);
    editMenu->addAction(shearAction);
    editMenu->addAction(boundAction);
    editMenu->addAction(gridAction);

    geometryMenu->addAction(pointAction);
    geometryMenu->addAction(lineAction);
    geometryMenu->addAction(polylineAction);
    geometryMenu->addAction(rectAction);
    geometryMenu->addAction(circleAction);
    geometryMenu->addAction(textAction);
}

void MainWindow::createToolBar() {
    QToolBar *toolBar = new QToolBar;
    toolBar->addAction(newFileAction);
    toolBar->addAction(openFileAction);
    toolBar->addAction(saveFileAction);
    toolBar->addSeparator();
    toolBar->addAction(pointAction);
    toolBar->addAction(lineAction);
    toolBar->addAction(polylineAction);
    toolBar->addAction(rectAction);
    toolBar->addAction(circleAction);
    toolBar->addAction(textAction);
    toolBar->addSeparator();
    toolBar->addAction(removeAction);
    toolBar->addAction(removeAllAction);
    toolBar->addSeparator();
    toolBar->addAction(bindAction);

    addToolBar(Qt::TopToolBarArea, toolBar);
}

void MainWindow::createMdiArea() {
    mMdiArea = new QMdiArea;
    mMdiArea->setViewMode(QMdiArea::TabbedView);
    mMdiArea->setTabsClosable(true);
    mMdiArea->setTabsMovable(true);
    mMdiArea->setTabShape(QTabWidget::Triangular);
    mMdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mMdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    setCentralWidget(mMdiArea);
}

Scene* MainWindow::getScene() {
    auto w = mMdiArea->currentSubWindow();
    return qobject_cast<Scene*>(
                qobject_cast<QGraphicsView*>(
                    w->widget())->scene()
                );
}

QGraphicsView* MainWindow::getView() {
    auto w = mMdiArea->currentSubWindow();
    return qobject_cast<QGraphicsView*>(w->widget());
}

void MainWindow::slotOpenFile() {
    QString strFilter;

    auto str = QFileDialog::getOpenFileName(nullptr, "Open Pixmap", QString(), strFilter);
    if(str.isEmpty()) {
        return;
    }

    if(!str.contains("xbm")) {
        return;
    }

    QPixmap img;
    img.load(str, "xbm");
    auto view = new QGraphicsView(mMdiArea);
    auto scene = new Scene;
    mScenes.append(scene);
    view->setScene(scene);
    mMdiArea->addSubWindow(view);
    view->setWindowTitle("Subwindow");
    view->show();
    view->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    scene->addPixmap(img);
    view->scale(7, 7);
}

void MainWindow::slotNewFile() {
    auto view = new QGraphicsView(mMdiArea);
    auto scene = new Scene;
    scene->setSceneRect(QRectF(0, 0, 1280, 640));
    connect(scene, SIGNAL(editSignal(QGraphicsItem*)), this, SLOT(slotEditItem(QGraphicsItem*)));
    mScenes.append(scene);
    view->setScene(scene);
    view->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    mMdiArea->addSubWindow(view);
    view->setWindowTitle("Subwindow");
    view->show();
    view->scale(7, 7);
}

void MainWindow::slotRemove() {
    auto all = getScene()->items();
    auto items = getScene()->selectedItems();
    foreach(auto item, items) {
        getScene()->removeItem(item);
        delete item;
    }
    getScene()->update();
}

void MainWindow::slotRemoveAll() {
    auto list = getScene()->items();
    foreach(auto item, list) {
        if(item->type() == TextType) {
            auto r = qgraphicsitem_cast<TextItem*>(item)->boundingRect();
            auto tl = r.topLeft();
            auto w = r.width();
            auto h = r.height();
            int d = 5;
        }
    }

    getScene()->clearAll();
}

void MainWindow::slotBound(const bool state) {
    if(getScene()->selectedItems().count() != 1) {
        return;
    }

    auto item = getScene()->selectedItems().at(0);
    QGraphicsRectItem *bound;
    if(state) {
        switch (item->type()) {
        case DotType: bound = getScene()->addRect(qgraphicsitem_cast<Dot*>(item)->getRect());
            break;
        case LineType: bound = getScene()->addRect(qgraphicsitem_cast<LineDot*>(item)->getRect());
            break;
        case CircleType: bound = getScene()->addRect(qgraphicsitem_cast<CircleDot*>(item)->getRect());
            break;
        }

        bounds.append(qMakePair(item, bound));
    } else {


        foreach(auto b, bounds) {
            if(b.first == item) {
                getScene()->removeItem(b.second);
            }
        }
    }

    getScene()->update();
}

void MainWindow::slotOk(const int sh, const int sv) {
    if(getScene()->selectedItems().isEmpty() || getScene()->selectedItems().count() != 1) {
        return;
    }

    auto item = getScene()->selectedItems().at(0);
    switch (item->type()) {
    case LineType: qgraphicsitem_cast<LineDot*>(item)->setShear<int>(sh, sv);
        break;
    case CircleType: qgraphicsitem_cast<CircleDot*>(item)->setShear(sh, sv);
        break;
    case DotType: qgraphicsitem_cast<Dot*>(item)->setShear(sh, sv);
        break;
    }

    getScene()->update();
}

void MainWindow::slotShear() {
    if(getScene()->selectedItems().isEmpty() || getScene()->selectedItems().count() != 1) {
        return;
    }

    auto wgt = new ShearInputWgt();
    connect(wgt, SIGNAL(emitOk(int, int)), this, SLOT(slotOk(int, int)));
    connect(wgt, SIGNAL(emitCancel()), this, SLOT(slotCancel()));


    workWgtDock = new QDockWidget("Dock", this);
    addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, workWgtDock);
    workWgtDock->setWidget(wgt);


}

void MainWindow::slotCloseFile() {

}

void MainWindow::slotSaveFile() {
    /*auto w = mMdiArea->currentSubWindow();
    if(w == nullptr) {
        return;
    }
    auto view = qobject_cast<QGraphicsView*>(w->widget());
    auto scene = qobject_cast<Scene*>(view->scene());
    scene->gridActivate(false);
    view->scale(0.14, 0.14);
    auto r = view->sceneRect().toRect();
    r.setTopLeft(QPoint(1, 63));
    r.setWidth(128);
    r.setHeight(64);
    QPixmap img = QWidget::grab(r);
    QString strFilter;

    QPainter pixPainter(&img);
    auto res = QFileDialog::getSaveFileName(nullptr, tr("Save Pixmap"), "Pixmap", "*.xbm ;; *.jpeg ;; *.bmp", &strFilter);
    if(strFilter.contains("xbm")) {
        img.save(res, "xbm");
    }
    auto s = view->size(); // 900 * 558

    scene->gridActivate(true);
    view->scale(7, 7);

*/



    Scene *resScene = new Scene;
    resScene->gridActivate(false);
    resScene->setSceneRect(0, 0, 128, 64);

    //QVector<QRectF*> pixels;
    QVector<Dot*> pixels;
    QVector<Dot*> bitMap;

    foreach(auto item, getScene()->items()) {
        switch (item->type()) {
        case CircleType: pixels =  qgraphicsitem_cast<CircleDot*>(item)->mapToBitmap();
            break;
        case LineType: pixels = qgraphicsitem_cast<LineDot*>(item)->mapToBitmap();
            break;
        }

        foreach(auto pixel, pixels) {
            bitMap.append(pixel);
        }
    }
/*
    auto resView = new QGraphicsView(mMdiArea);
    resView->setScene(resScene);
    resView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    mMdiArea->addSubWindow(resView);
    resView->setWindowTitle("Subwindow");
    resView->show();
    resView->scale(1, 1);

    auto r = resView->sceneRect().toRect();
    r.setTopLeft(QPoint(1, 63));
    r.setWidth(128);
    r.setHeight(64);
    QPixmap img = QWidget::grab(r);
    QString strFilter;

    QPainter pixPainter(&img);
    auto res = QFileDialog::getSaveFileName(nullptr, tr("Save Pixmap"), "Pixmap", "*.xbm ;; *.jpeg ;; *.bmp", &strFilter);
    if(strFilter.contains("xbm")) {
        img.save(res, "xbm");
    }
*/
    auto data = createXbm(bitMap);
    QFile file;
    QString strFilter;
    auto res = QFileDialog::getSaveFileName(nullptr, tr("Save xbm"), "Pixmap", "*.xbm", &strFilter);
    if(strFilter.contains("xbm")) {
        file.setFileName(res);
        if(file.open(QIODevice::WriteOnly)) {
            file.write(data);
        }

    }

}

QByteArray MainWindow::createXbm(QVector<Dot *> &pixels) {

    QByteArray array;
    int count = 0;
    int bits = 0;/*
    array.append("#define _width 128\n#define _height 64\nstatic char _bits[] = {\n");
    for(int i = 0; i < 64; i++) {
        for(int j = 0; j < 128; j++) {
            count++;
            if(pixels.isEmpty()) {
                if(i == 63 && j == 127) {
                    array.append("0x00 };");
                } else {
                    array.append("0x00, ");
                }
            } else {
                auto p = pixels.first();
                //count++;
                if(i == 63 && j == 127) {
                    if(p->getX() == i && p->getY() == j) {
                        array.append("0x01 };");
                        pixels.removeFirst();
                    } else {
                        array.append("0x00 };");
                    }
                } else {
                    int x = p->getX();
                    int y = p->getY();
                    if(x == j && y == i) {
                        array.append("0x01, ");
                        pixels.removeFirst();
                    } else {
                        array.append("0x00, ");
                    }
                }

            }

            if(count >= 16) {
                count = 0;
                array.append("\n");
            }

        }
    }
    */

    array.append("#define _width 128\n#define _height 64\nstatic char _bits[] = {\n");
    int countStr = 0;
    int countPixel = 0;

    for(int i = 0; i < 64; i++) {
        for(int j = 0; j < 16; j++) {
            count++;
            if(pixels.isEmpty()) {
                if(i == 63 && j == 15) {
                    array.append("0x00 };");
                } else {
                    array.append("0x00, ");
                }
            } else {
                foreach(auto p, pixels) {
                    int x = p->getX();
                    int y = p->getY();
                    if(i == 63 && j == 15) {
                        if(p->getX() == i && p->getY() == j) {
                            array.append("0xFF };");
                            pixels.removeFirst();
                        } else {
                            array.append("0x00 };");
                        }
                    } else {
                        int x = p->getX();
                        int y = p->getY();
                        if(x == j && y == i) {
                            array.append("0xFF, ");
                            pixels.removeFirst();
                        } else {
                            array.append("0x00, ");
                        }
                    }
                }
            }
            if(count >= 16) {
                count = 0;
                array.append("\n");
            }
        }
    }
/*
    for(int i = 0; i < 64; i++) {
        for(int j = 0; j < 16; j++) {
            count++;
            if(pixels.isEmpty()) {
                if(i == 63 && j == 15) {
                    array.append("0x00 };");
                } else {
                    array.append("0x00, ");
                }
            } else {
                auto p = pixels.first();
                //count++;
                if(i == 63 && j == 15) {
                    if(p->getX() == i && p->getY() == j) {
                        array.append("0xFF };");
                        pixels.removeFirst();
                    } else {
                        array.append("0x00 };");
                    }
                } else {
                    int x = p->getX();
                    int y = p->getY();
                    if(x == j && y == i) {
                        array.append("0xFF, ");
                        pixels.removeFirst();
                    } else {
                        array.append("0x00, ");
                    }
                }

            }

            if(count >= 16) {
                count = 0;
                array.append("\n");
            }

        }
    }
*/
    return array;
}


void MainWindow::slotExit() {

}

void MainWindow::slotSetScale() {
    double sx = ui->xEdit->text().toDouble();
    double sy = ui->yEdit->text().toDouble();
    ui->graphicsView->scale(sx, sy);
}

/*void MainWindow::slotSetText() {
    if(wgt == nullptr) {
        return;
    }

    m_scene->setText(wgt->getText());
}*/

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
    getScene()->gridActivate(true);

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
    getScene()->gridActivate(false);
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
    getScene()->gridActivate(true);
    ui->graphicsView->scale(4, 4);
}

void MainWindow::slotClearBut() {
    getScene()->clearAll();
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
    if(m_type == ItemType::Point) {

    }
    wgt = new WorkWidget(this, m_type);
   // wgt = new PointInputWgt();
  //  auto l = ui->widget->layout();
    //l->addWidget(wgt);
    connect(wgt, SIGNAL(stop()), this, SLOT(slotStop()));
    connect(wgt, SIGNAL(create()), this, SLOT(slotCreate()));
    connect(getScene(), SIGNAL(getPointSignal(const QPointF&)), this, SLOT(slotSceneGetPoint(const QPointF&)));
    connect(getScene(), SIGNAL(endInputSignal()), this, SLOT(slotSceneEndInput()));

    workWgtDock = new QDockWidget("Dock", this);
    addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, workWgtDock);
    workWgtDock->setWidget(wgt);


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
        case TextType: data = qgraphicsitem_cast<TextItem*>(currentItem)->getData();
         //   qgraphicsitem_cast<Text*>(currentItem)->edit();
            break;
        }
        currentData = data;
        //wgt->setItemData(data);
    } /*else if(m_type == ItemType::Text) {
        TextItemDialog dialog(0, QPoint(), getScene());
        if(dialog.exec()) {
            currentItem = dialog.textItem();
        }
    }*/
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
    getScene()->update();
    startInput();
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

    auto w = mMdiArea->currentSubWindow();
    if(w == nullptr) {
        return;
    }
    auto wgt = w->widget();

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
        getScene()->removeItem(currentItem);
      //  wgt->checkData();
        getScene()->addShape(currentData);
    }
    delete wgt;
    delete workWgtDock;
    changeMode(Mode::Normal);
}

void MainWindow::slotCancel() {
    if(m_mode == Mode::Edit) {
        getScene()->removeItem(currentItem);
        //wgt->checkData();
        getScene()->addShape(currentData);
    }
    changeMode(Mode::Normal);
}

void MainWindow::slotCreate() {

    auto scene = getScene();
    if(m_mode == Mode::Input) {
        auto data = wgt->getData(m_type);
        //m_scene->setTypeMode(Mode::Input);
        //m_scene->addShape(data);
        scene->setTypeMode(Mode::Input);
        scene->addShape(data);

    } else if(m_mode == Mode::Edit) {
        scene->removeItem(currentItem);
        auto data = wgt->getData(m_type);
        scene->setTypeMode(Mode::Edit);
        scene->addShape(data);
        //m_scene->setTypeMode(Mode::Edit);
        //m_scene->addShape(data);
        delete wgt;
        delete workWgtDock;
        changeMode(Mode::Normal);
    }
}

void MainWindow::slotEditItem(QGraphicsItem *item) {
    changeMode(Mode::Edit);
    currentItem = item;
    startInput();
    getScene()->update();
    //m_scene->update();
}

void MainWindow::changeMode(const Mode mode) {
    getScene()->setTypeMode(mode);
    m_mode = mode;
    //m_scene->setTypeMode(mode);
}

void MainWindow::changeType(const ItemType type) {
    getScene()->setTypeShape(type);
    m_type = type;
   // m_scene->setTypeShape(type);
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
            getScene()->addItem(m);

    }

   getScene()->update();
}

void MainWindow::slotMoveItem() {
    if(m_mode != Mode::Normal || getScene()->items().count() == 0) {
        return;
    }

    //Q_ASSERT(!m_scene->selectedItems().isEmpty());
    if(getScene()->selectedItems().isEmpty()) {
        return;
    }
    auto items = getScene()->selectedItems();
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
    getScene()->update();
}

void MainWindow::slotCheck() {
    auto allItems = getScene()->items();
    foreach(auto item, allItems) {
    //    item->setSelected(true);
    }
    auto items = getScene()->selectedItems();

    int d = 5;
}

void MainWindow::slotCheckRect() {
    RectItem *item = new RectItem();
    item->setPos(randomBetween(0, 128),
                 randomBetween(0, 64));
    getScene()->addItem(item);
    changeType(ItemType::Rect);
}

void MainWindow::slotCheckCircle() {
   // CircleItem *item = new CircleItem();
   auto item = getScene()->addEllipse(QRect(randomBetween(0, 128),
                             randomBetween(0, 64), 30, 30));
   // item->setPos(randomBetween(0, 128),
     //             randomBetween(0, 64));
    //m_scene->addItem(item);
   item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
   item->setSelected(true);
    changeType(ItemType::Circle);
}

void MainWindow::slotDeleteSelected() {
    auto items = getScene()->selectedItems();
    foreach(auto item, items) {
        getScene()->removeItem(item);
        delete item;
    }
}

void MainWindow::slotBind(bool state) {
    if(mScenes.isEmpty()) {
        return;
    }
    getScene()->bindActivate(state);
}

void MainWindow::slotGrid(bool state) {
    qDebug() << state;
    getScene()->gridActivate(state);

    /*switch (state) {
    case Qt::Checked: getScene()->gridActivate(true);
        break;
    case Qt::Unchecked: getScene()->gridActivate(false);
        break;
    }*/
}

void MainWindow::slotSceneGetPoint(const QPointF &p) {
    if(wgt == nullptr) {
        return;
    }

    //wgt->setPoint(p);
}

void MainWindow::slotSceneEndInput() {
    getScene()->update();
    if(wgt != nullptr) {
      //  wgt->endInput();
    }
}
