#include "mainwindow.h"

#include "gui/board.h"
#include "solver/nonogram.h"

#include <QApplication>
#include <QFileDialog>
#include <QLayout>
#include <QMenuBar>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      _data{std::make_shared<Game::NonogramData>()}
{
    _nonogram = std::unique_ptr<Game::Nonogram>(new Game::Nonogram(_data, _mutex));
    _board = new Game::Board(_data, this);

    setCentralWidget(_board);
    setMenuBar(createMenu());
}

QMenuBar * MainWindow::createMenu()
{
    QMenuBar * menuBar = new QMenuBar(this);

    QMenu * menu = menuBar->addMenu("Файл");
    QAction * openAct  = menu->addAction("Открыть");
    QAction * closeAct = menu->addAction("Закрыть");

    menu = menuBar->addMenu("Решение");
    QAction * promtAct = menu->addAction("Подсказка");
    QAction * solvoAct = menu->addAction("Показать решение");
    QAction * checkAct = menu->addAction("Проверить решение");
    QAction * clearAct = menu->addAction("Очистить решение");

    menu = menuBar->addMenu("Справка");
    QAction * aboutAct = menu->addAction("О программе");

    connect(openAct, &QAction::triggered, this, &MainWindow::openFile);
    connect(closeAct, &QAction::triggered, qApp, &QApplication::quit);

    connect(promtAct, &QAction::triggered, this, &MainWindow::showPromt);
    connect(solvoAct, &QAction::triggered, this, &MainWindow::showResult);
    connect(checkAct, &QAction::triggered, this, &MainWindow::checkResult);
    connect(clearAct, &QAction::triggered, this, &MainWindow::clearResult);

    connect(aboutAct, &QAction::triggered, this, [this]()
    {
        QMessageBox::about(this, qApp->applicationDisplayName(),
                           "Экспорт с http://webpbn.com/export.cgi\n\n"
                           "Формат: Format for Andrew Makhorin's pbnsol nonogram solver.\n\n"
                           "Управление:\n"
                           " - поставить решение - левая кнопка мыши;\n"
                           " - снять решение - правая кнопка мыши.\n");
    });

    return menuBar;
}

void MainWindow::openFile()
{
    //Помечаем, что пока новые данные не загружены
    const QString filter("*.txt");
    const QString path = QDir::currentPath();
    const QString filename = QFileDialog::getOpenFileName(this, "Открыть новый файл",
                                                          path, filter);
    if(!_nonogram)
        return;
    //Загружаем новые данные
    _nonogram->loadData(filename.toStdString());
    //Если данные не удалось загрузить, выводим сообщение
    if(!_nonogram->isLoaded()){
        QMessageBox::warning(this, qApp->applicationDisplayName(),
                             "Ошибка загрузки\n"
                             "Не удалось загрузить файл");
        return;
    }

    std::unique_lock<std::mutex> lk(_mutex);
    //Вывод заголовка
    int width = _data->getColumns().size();
    int height = _data->getRows().size();

    const QString titleName(filename
                            + QString::asprintf(" (%d x %d)", width, height));

    setWindowTitle(titleName);
    if(!_board)
        return;
    _board->createBoard();
}

void MainWindow::showResult()
{
    if(!_board || !_nonogram)
        return;
    if(_nonogram->isSolved()) {
        std::unique_lock<std::mutex> lk(_mutex);
        _board->showResult();
    }
    else{
        showMessage();
    }
}

void MainWindow::showPromt()
{
    if(!_board || !_nonogram)
        return;
    if(_nonogram->isSolved()) {
        std::unique_lock<std::mutex> lk(_mutex);
        _board->showPromt();
    }
    else{
        showMessage();
    }
}

void MainWindow::checkResult()
{
    if(!_board || !_nonogram)
        return;
    if(_nonogram->isSolved()) {
        std::unique_lock<std::mutex> lk(_mutex);
        _board->checkResult();
    }
    else{
        showMessage();
    }
}

void MainWindow::clearResult()
{
    if(!_board || !_nonogram)
        return;
    if(_nonogram->isLoaded()) {
        std::unique_lock<std::mutex> lk(_mutex);
        _board->clearResult();
    }
}

void MainWindow::showMessage()
{
    if(!_nonogram)
        return;
    //Если данные загружены
    if(_nonogram->isLoaded())
       QMessageBox::information(this, qApp->applicationDisplayName(),
                             "Подождите, выполняется решение");
}
