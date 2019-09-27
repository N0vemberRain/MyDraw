#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QRubberBand>
#include <QMdiArea>

#include "scene.h"
#include "workwidget.h"

class DrawFactory : public QObject {
    Q_OBJECT
public:
    DrawFactory(QObject *parent);
    ~DrawFactory();

   // void draw(const ItemType type, QGraphicsScene *scene);
    void draw(const ItemType type, QGraphicsScene *scene, QGraphicsItem *item);


private:
    bool state;

    void workWithLine(QGraphicsScene *scene, QGraphicsItem *item);
};


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void createMenu();
    void createToolBar();
    void createMdiArea();
    void startInput(const ItemType type);
    void startInput();
    void setItemData(const QStringList &data);
    void changeMode(const Mode mode);
    void changeType(const ItemType type);

    QByteArray createXbm(QVector<Dot*> &array);

    Scene* getScene();
    QGraphicsView* getView();


    //void setRectData()
    Ui::MainWindow *ui;
    //QGraphicsScene *m_scene;
    //Scene *m_scene;

    QVector<Scene*> mScenes;
   // DrawFactory *m_factory;
    ItemType m_type;
    Mode m_mode;
    WorkWidget *wgt;
    //PointInputWgt *wgt;
    QGraphicsItem *currentItem;
    QStringList currentData;

    QString tmpText;

    QAction *newFileAction;
    QAction *openFileAction;
    QAction *saveFileAction;
    QAction *closeFileActions;
    QAction *exitAction;
    QAction *pointAction;
    QAction *lineAction;
    QAction *polylineAction;
    QAction *rectAction;
    QAction *circleAction;
    QAction *textAction;
    QAction *removeAction;
    QAction *removeAllAction;
    QAction *bindAction;
    QAction *shearAction;
    QAction *boundAction;
    QAction *gridAction;

    QDockWidget *workWgtDock;

    QMdiArea *mMdiArea;

    QVector<QPair<QGraphicsItem*, QGraphicsRectItem*>> bounds;

private slots:
    void slotTextBut();
    void slotPointBut();
    void slotLineBut();
    void slotCircleBut();
    void slotRectBut();
    void slotPolylineBut();
    void slotRemove();
    void slotRemoveAll();
    void slotShear();
    void slotBound(const bool state);

    void slotOpenFile();
    void slotNewFile();
    void slotCloseFile();
    void slotSaveFile();
    void slotExit();

    void slotSaveView();
    void slotSaveScene();
    void slotClearBut();

    void slotStop();
    void slotCreate();
    void slotOk(const int sh, const int sv);
    void slotCancel();
 //   void slotCreateEdit();
  //  void slotGetData();

    void slotEditItem(QGraphicsItem *item);
    void slotSelect(QGraphicsItem *item);
    void slotMoveItem();

    void slotCheck();
    void slotCheckRect();
    void slotCheckCircle();
    void slotDeleteSelected();
    void slotBind(bool state);
    void slotGrid(bool state);

    void slotSetScale();
    void slotSceneGetPoint(const QPointF &p);
    void slotSceneEndInput();
    //void slotSetText();
};

#endif // MAINWINDOW_H
