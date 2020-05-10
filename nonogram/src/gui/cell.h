#pragma once

#include <QGraphicsItem>
#include <QGraphicsLayoutItem>

#include "constans.h"

namespace Game {
    class Cell : public QGraphicsLayoutItem, public QGraphicsItem
    {
        static QSize _size;

    public:
        Cell(int i, int j, QGraphicsItem *parent = nullptr);

        static QSize size() {return _size; };

        void setState(State state);
        State state() const;

    protected:
        virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                           QWidget *widget = nullptr) override;

    private:
        QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const override;
        void setGeometry(const QRectF &geom) override;
        virtual QRectF boundingRect() const override;
        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    private:
        int _i;
        int _j;
        State _state;
    };

}//Nonogram
