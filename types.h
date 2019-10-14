#ifndef TYPES_H
#define TYPES_H

#include <QGraphicsItem>

const int PointType = QGraphicsItem::UserType + 1;
const int LineType = QGraphicsItem::UserType + 2;
const int PolylineType = QGraphicsItem::UserType + 3;
const int CircleType = QGraphicsItem::UserType + 4;
const int RectType = QGraphicsItem::UserType + 5;
const int GridType = QGraphicsItem::UserType + 6;
const int MoveType = QGraphicsItem::UserType + 7;
const int CheckType = QGraphicsItem::UserType + 10;
const int SelectType = QGraphicsItem::UserType + 8;
const int TextType = QGraphicsItem::UserType + 9;
const int DotType = QGraphicsItem::UserType + 11;


enum class ItemType { Point = 0, Line = 1, Ellipse = 2, Rect = 3, Circle = 4,
                    Polyline = 5, Text = 6 };

enum class Mode { Normal = 0, Input = 1, Edit = 2, Select = 4 };
enum class MomentoType { AddingItemMomento = 0, MoveMomento = 1, ResizeMomento = 2 };
enum class SceneStateType { AddingStateType = 0, MoveStateType = 1, ResizeStateType = 2 };

#endif // TYPES_H

