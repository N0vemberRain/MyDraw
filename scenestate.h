#ifndef SCENESTATE_H
#define SCENESTATE_H

//#include "scene.h"
#include <QGraphicsItem>

class Scene;

class SceneState
{
public:
    //SceneState();
    virtual ~SceneState() = 0;

    virtual QGraphicsItem* getItem() const = 0;
    virtual qreal getX() const = 0;
    virtual qreal getY() const = 0;
    virtual void setItem(QGraphicsItem*) = 0;
    virtual void setX(const qreal dx) = 0;
    virtual void setY(const qreal dy) = 0;
protected:
    //void changeState(Scene *scene, SceneState *state);
};

class AddingState : public SceneState {
public:
    explicit AddingState(QGraphicsItem *item);
    virtual ~AddingState();

    virtual QGraphicsItem* getItem() const { return mItem; }
    virtual qreal getX() const { return 0.0; }
    virtual qreal getY() const { return 0.0; }
    virtual void setItem(QGraphicsItem* item) { mItem = item; }
    virtual void setX(const qreal dx) { Q_UNUSED(dx); }
    virtual void setY(const qreal dy) { Q_UNUSED(dy); }
private:
    QGraphicsItem *mItem;
};

class MoveState : public SceneState {
public:
    explicit MoveState(QGraphicsItem *item, const qreal dx, const qreal dy);
    virtual ~MoveState();

    virtual QGraphicsItem* getItem() const { return mItem; }
    virtual qreal getX() const { return mHorizontalShear; }
    virtual qreal getY() const { return  mVerticalShear; }
    virtual void setItem(QGraphicsItem* item) { mItem = item; }
    virtual void setX(const qreal dx) { mHorizontalShear = dx; }
    virtual void setY(const qreal dy) { mVerticalShear = dy; }
private:
    QGraphicsItem *mItem;
    qreal mHorizontalShear;
    qreal mVerticalShear;
};
/*
class ResizeState : public SceneState {
public:
    static SceneState* instance();
};*/

class StateFactory {
    public:
    SceneState* createState(const MomentoType type, QGraphicsItem *item) {
        return new AddingState(item);
    }
    
    SceneState* createState(const MomentoType type, QGraphicsItem *item, const qreal dx, const qreal dy)
        switch(type) {
        case MomentoType::MoveMomento: return new MoveState(item, dx, dy);
            break;
        case MomentoType::AddingMomento: return new AddingState(item);
            break;
        case MomentoType::ResizeMomento: return new ResizeState(item, dx, dy);
            break;
        }
    }
};

#endif // SCENESTATE_H
