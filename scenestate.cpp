#include "scenestate.h"
#include "scene.h"

AddingState::AddingState(QGraphicsItem* item) : mItem(item) {}
AddingState::~AddingState() { delete mItem; }

MoveState::MoveState(QGraphicsItem *item, const qreal dx, const qreal dy)
    :mItem(item), mHorizontalShear(dx), mVerticalShear(dy) {}
MoveState::~MoveState() { delete mItem; }
