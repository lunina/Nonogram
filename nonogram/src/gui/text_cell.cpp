#include "text_cell.h"
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

Game::TextCell::TextCell(char num, QGraphicsItem *parent):
    Cell(0, 0, parent),
    _number(num)
{

}

void Game::TextCell::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        setState((state() == State::Checked) ?
                     State::Unknown : State::Checked);
    }

    QGraphicsItem::mousePressEvent(event);
}

void Game::TextCell::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //Cell::paint(painter, option, widget);
    painter->save();
    switch (state()) {
    case State::Checked:
        painter->setBrush(Qt::darkGray);
        break;
    case State::Unknown:
        painter->setBrush(Qt::lightGray);
        break;
    default:
        break;
    }
    painter->setPen(Qt::black);
    painter->drawRect(option->rect);
    painter->drawText(option->rect, QString::number(_number), QTextOption(Qt::AlignCenter));
    painter->restore();
}

