#pragma once

#include <QMainWindow>

#include "nonogram_data.h"
#include "nonogram.h"

class QGraphicsView;

namespace Game {
    class Board;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    //Открыть файл
    void openFile();
    //Показать результат
    void showResult();
    //Показать подсказку
    void showPromt();
    //Показать проверку решения
    void checkResult();
    //Очистить решения
    void clearResult();

private:
    QMenuBar *createMenu();
    void showMessage();

private:
    Game::NonogramDataPtr _data;
    Game::NonogramPtr _nonogram;
    Game::Board *_board;
    std::mutex _mutex;
};
