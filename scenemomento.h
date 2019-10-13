#ifndef SCENEMOMENTO_H
#define SCENEMOMENTO_H

#include <QObject>
#include <QGraphicsItem>
#include <QAction>

#include "types.h"
//#include "scene.h"

class Scene;
class SceneState;

class Momento {
public:
    Momento() {}
    ~Momento() {}
    MomentoType type() const { return mType; }

protected:
    void setType(const MomentoType type) {
        mType = type;
    }
private:
    MomentoType mType;
};

class AddingMomento : public Momento
{
public:
    AddingMomento(/*QList<QGraphicsItem*>*/ QGraphicsItem* state)
        : Momento(), mState(state) {
        Momento::setType(MomentoType::AddingItemMomento);
    }
    virtual ~AddingMomento();

    MomentoType type() const { return Momento::type(); }
private:
    friend class Scene;

    //QList<QGraphicsItem*> mState;
    QGraphicsItem *mState;
};

class MoveMomento : public Momento {
public:
    MoveMomento(QGraphicsItem *target, const qreal dx, const qreal dy)
        : Momento(), mTarget(target), x(dx), y(dy) {
        Momento::setType(MomentoType::MoveMomento);
    }
    ~MoveMomento() {
        delete mTarget;
    }

    MomentoType type() const { return Momento::type(); }

private:
    friend class Scene;

    QGraphicsItem *mTarget;
    qreal x;
    qreal y;
};



#endif // SCENEMOMENTO_H
