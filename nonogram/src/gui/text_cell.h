#pragma once
#include "cell.h"

namespace Game {

    class TextCell : public Cell
    {
    public:
        TextCell(char num, QGraphicsItem *parent = nullptr);

        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget */*widget*/);
    private:
        char _number;
    };
}//Game

