#pragma once


#include <QGraphicsView>
#include <array>
#include "nonogram_data.h"

class QGraphicsGridLayout;
class QGraphicsScene;
namespace Game {
    class Cell;
    class Board : public QGraphicsView
    {
        Q_OBJECT
    public:
        Board(Game::NonogramDataPtr data, QWidget *parent = nullptr);
        ~Board();

        void createBoard();

        //Показать решение
        void showResult();
        //Показать подсказку
        void showPromt();
        //Проверить шаги решения
        void checkResult();
        //Проверить шаги решения
        void clearResult();

    private:
        //Очисть поле
        void clearBoard();
        //Создать вертикальный заголовок
        void drawVHeader();
        //Создать горизонтальный заголовок
        void drawHHeader();
        //Создать игровое поле
        void drawBoard();

    private:
        QGraphicsScene *_scene;

        QGraphicsGridLayout *_board;
        QGraphicsGridLayout *_vHeader;
        QGraphicsGridLayout *_hHeader;

        NonogramDataPtr _data;
        std::vector<std::vector<Game::Cell *>> _items;
        QList<Game::Cell *>_itemsForDelete;//Список элементов для очистки

        bool _isSolveShown; //Решение отображено
    };
}//Nonogram
