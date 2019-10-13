#ifndef SCENEMOMENTO_H
#define SCENEMOMENTO_H

#include <QObject>
#include <QGraphicsItem>
#include <QAction>

#include "types.h"
//#include "scene.h"

class Scene;
class SceneState;
class AddingState;
class MoveState;

class Momento {
public:
    //Momento() {}
    virtual ~Momento() = 0;
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
    AddingMomento(QGraphicsItem*);
    AddingMomento(AddingState*);
    virtual ~AddingMomento();

    MomentoType type() const { return Momento::type(); }
private:
    friend class Scene;

    //QList<QGraphicsItem*> mState;
    QGraphicsItem *mItem;
    AddingState *mState;
};

class MoveMomento : public Momento {
public:
    MoveMomento(QGraphicsItem *item, const qreal dx, const qreal dy);
    MoveMomento(MoveState*);
    ~MoveMomento();

    MomentoType type() const { return Momento::type(); }

private:
    friend class Scene;

    MoveState *mState;
    QGraphicsItem *mItem;
    qreal x;
    qreal y;
};

class MomentoFactory {
public:
    explicit MomentoFactory() {}
    ~MomentoFactory() {}

    Momento* createMomento
};


#endif // SCENEMOMENTO_H
