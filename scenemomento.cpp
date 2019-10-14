#include "scenemomento.h"
#include "scenestate.h"

/*Momento::Momento(SceneState *state) {

}*/

AddingMomento::AddingMomento(QGraphicsItem *item)
    : mItem(item) {
    Momento::setType(MomentoType::AddingItemMomento);
    mState = new AddingState(mItem);
     //Momento (mState),
}

AddingMomento::AddingMomento(AddingState *state)
    : mState(state) {
    Momento::setType(MomentoType::AddingItemMomento);
    mItem = mState->getItem();
}

AddingMomento::~AddingMomento() {
    delete mItem;
    delete mState;
}

MoveMomento::MoveMomento(QGraphicsItem *item, const qreal dx, const qreal dy)
    : mItem(item), x(dx), y(dy) {
    Momento::setType(MomentoType::MoveMomento);
    mState = new MoveState(mItem, x, y);
}

MoveMomento::MoveMomento(MoveState *state)
    : mState(state) {
    Momento::setType(MomentoType::MoveMomento);
    mItem = mState->getItem();
    x = mState->getX();
    y = mState->getY();
}

MoveMomento::~MoveMomento() {
    delete mItem;
    delete mState;
}
