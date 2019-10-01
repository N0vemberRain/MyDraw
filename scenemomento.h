#ifndef SCENEMOMENTO_H
#define SCENEMOMENTO_H

#include <QObject>
#include <QGraphicsItem>
#include <QAction>

//#include "scene.h"

class Scene;

class SceneMomento// : public QObject
{
public:
    SceneMomento(/*QList<QGraphicsItem*>*/ QGraphicsItem* state);
    ~SceneMomento();

private:
    friend class Scene;

    //QList<QGraphicsItem*> mState;
    QGraphicsItem *mState;
};

class SceneCommand {
public:
    SceneCommand(Scene *receiver, QAction *action)
        : mReceiver(receiver), mAction(action) {

    }

    void undo() {
        if (mNumCommands == 0) {
            return;
        }
        mCommandList[mNumCommands - 1]->mReceiver->reinstateMemento(
                    mMementoList[mNumCommands - 1]
                );
        mNumCommands--;

    }

private:
    Scene *mReceiver;
    QAction *mAction;
    static QList<SceneCommand*> mCommandList;
    static QList<SceneMomento*> mMementoList[20];
    static int mNumCommands;
    static int mHighWater;
    QList<int> l;
};

#endif // SCENEMOMENTO_H
