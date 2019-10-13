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

class SceneCommand {
public:
    SceneCommand(Scene *receiver)
        : mReceiver(receiver)/*, mAction(action)*/ {
        mNumCommands = 0;
        mCurrentMomento = 0;
    }

    void undo() {
        if (mNumCommands == 0) {
            return;
        }

        auto m = mMomentoList.at(0);
        mMomentoList.removeFirst();
        mRemoveMomentoList.prepend(m);
        mReceiver->reinstateMomentoUndo(m);
        mNumCommands--;
    }

    void redo() {
        if (mNumCommands > mHighWater || mRemoveMomentoList.isEmpty()) {
            return ;
        }

        auto m = mRemoveMomentoList.at(0);
        mRemoveMomentoList.removeFirst();
        mMomentoList.prepend(m);
        mReceiver->reinstateMomentoRedo(m);
        mNumCommands++;
    }

    void addMomento(SceneMomento *momento) {
        //mMomentoList.append(momento);
        //mCurrentMomento = mMomentoList.count();
        mMomentoList.prepend(momento);
        mNumCommands++;
    }
private:
    Scene *mReceiver;
    QAction *mAction;
    QList<SceneCommand*> mCommandList;
    QList<Momento*> mMomentoList;
    QList<Momento*> mRemoveMomentoList;
    int mNumCommands;
    int mCurrentMomento;
    int mHighWater = 5;
    QList<int> l;
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
    void createConnection();
    void createDockInputWgt();
    void createInputWgt();
    void startInput(const ItemType type);
    void startInput();
    void setItemData(const QStringList &data);
    void changeMode(const Mode mode);
    void changeType(const ItemType type);

    void undo();
    void redo();

    InputWgt* createInputWgt(InputFactory *factory) {
        return factory->factoryMethod();
    }

    Scene* getScene();
    QGraphicsView* getView();


    //void setRectData()
    Ui::MainWindow *ui;
    //QGraphicsScene *m_scene;
    //Scene *m_scene;

    QVector<Scene*> mScenes;
   // DrawFactory *m_factory;
    ItemType m_type;
    int mCurrentType;
    Mode m_mode;
    WorkWidget *wgt;
    QGraphicsItem *currentItem;
    QStringList currentData;
    QDockWidget *dockInputWgt;
    InputWgt *inWgt;

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
    QAction *redoAction;
    QAction *undoAction;
    QAction *checkAction;

    QDockWidget *workWgtDock;

    QMdiArea *mMdiArea;

    QList<QAction*> mActionList;
    QList<SceneMomento*> mMomentoList;
    int mNumAction;

    SceneCommand* mSceneComd;
    bool wgtInput;

private slots:
    void slotTextBut();
    void slotPointBut();
    void slotLineBut();
    void slotCircleBut();
    void slotRectBut();
    void slotPolylineBut();
    void slotRemove();
    void slotRemoveAll();

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
    void slotGrid(int state);

    void slotSetScale();
    void slotSceneGetPoint(const QPointF &p);
    void slotSceneEndInput();
    //void slotSetText();
};

#endif // MAINWINDOW_H
