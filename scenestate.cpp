#include "scenestate.h"
#include "scene.h"

AddingState::AddingState(QGraphicsItem* item)
    : mItem(item) {
    SceneState::setType(SceneStateType::AddingStateType);
}
AddingState::~AddingState() { delete mItem; }

MoveState::MoveState(QGraphicsItem *item, const qreal dx, const qreal dy)
    :mItem(item), mHorizontalShear(dx), mVerticalShear(dy) {
    SceneState::setType(SceneStateType::MoveStateType);
}
MoveState::~MoveState() { delete mItem; }
