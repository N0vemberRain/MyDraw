#ifndef SCENEMOMENTO_H
#define SCENEMOMENTO_H

#include <QObject>
#include <QGraphicsItem>
#include <QAction>

#include "types.h"
#include "scenestate.h"
//#include "scene.h"

class Scene;
class SceneState;
class AddingState;
class MoveState;

class Momento {
public:
    //Momento(SceneState *state);
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
    explicit AddingMomento(QGraphicsItem*);
    explicit AddingMomento(AddingState*);
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
    explicit MoveMomento(QGraphicsItem *item, const qreal dx, const qreal dy);
    explicit MoveMomento(MoveState*);
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
  //  explicit MomentoFactory() {}
    //~MomentoFactory() {}

    Momento* createMomento(const MomentoType type, SceneState *state) {
       /* Momento *m;
        switch(type) {
        case MomentoType::MoveMomento: m = new MoveMomento(state);
            break;
        case MomentoType::AddingItemMomento: state = new AddingState(state);
            break;
        //case MomentoType::ResizeMomento: return new ResizeState(item, dx, dy);
          //  break;
        default: state = nullptr;
        }
        return state;*/
    }

    Momento* createMomento(const MomentoType type, QGraphicsItem *item,
                           const qreal dx = 0, const qreal dy = 0) {
        Momento *m;
        switch(type) {
        case MomentoType::MoveMomento: m = new MoveMomento(item, dx, dy);
            break;
        case MomentoType::AddingItemMomento: m = new AddingMomento(item);
            break;
        //case MomentoType::ResizeMomento: return new ResizeState(item, dx, dy);
          //  break;
        default: m = nullptr;
        }
        return m;
    }
};


#endif // SCENEMOMENTO_H
