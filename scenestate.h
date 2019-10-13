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
protected:
    //void changeState(Scene *scene, SceneState *state);
};

class AddingState : public SceneState {
public:
    explicit AddingState(QGraphicsItem *item);
    virtual ~AddingState();

    virtual QGraphicsItem* getItem() const { return mItem; }
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

#endif // SCENESTATE_H
