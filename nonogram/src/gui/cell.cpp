#include "cell.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <iostream>

#include <QDebug>

QSize Game::Cell::_size = QSize(cellSide, cellSide);

Game::Cell::Cell(int i, int j, QGraphicsItem *parent):
    QGraphicsLayoutItem(), QGraphicsItem(parent),
    _i(i), _j(j), _state(State::Unknown)
{
    setGraphicsItem(this);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

QRectF Game::Cell::boundingRect() const
{
    return QRectF(QPointF(0, 0), geometry().size());
}

void Game::Cell::setState(Game::State state)
{
    _state = state;
    update();
}

Game::State Game::Cell::state() const
{
    return _state;
}

void Game::Cell::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        _state = (_state==State::Checked)?
                     State::Unchecked:State::Checked;
    }
    if(event->button()==Qt::RightButton){
        _state = State::Unknown;
    }
    update();
    QGraphicsItem::mousePressEvent(event);
}

QSizeF Game::Cell::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    switch (which) {
    case Qt::MinimumSize:
    case Qt::PreferredSize:
        return Cell::size();
    case Qt::MaximumSize:
        return QSize(150,150);
    default:
        break;
    }
    return constraint;
}

void Game::Cell::setGeometry(const QRectF &geom)
{
    prepareGeometryChange();
    QGraphicsLayoutItem::setGeometry(geom);
    setPos(geom.topLeft());
}

void Game::Cell::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget */*widget*/)
{
    painter->save();
    painter->setPen(Qt::black);
    switch (_state) {
    case State::Checked:
        painter->setBrush(Qt::darkGray);
        break;
    case State::Unknown:
        painter->setBrush(Qt::white);
    case State::Unchecked:
        break;
    //    painter->setBrush(Qt::lightGray);
    //    break;
    case State::Error:
        painter->setBrush(Qt::red);
    default:
        break;
    }

    painter->drawRect(option->rect);

    if(_state == State::Unchecked){
        QRect rect = option->rect.adjusted(5,5,-5,-5);
        QVector<QPoint> pointPairs{rect.topLeft(),
                                   rect.bottomRight(),
                                   rect.bottomLeft(),
                                   rect.topRight()};

        painter->setPen(QPen(QBrush(Qt::lightGray), 4));
        painter->drawLines(pointPairs);
    }
    painter->restore();
}
