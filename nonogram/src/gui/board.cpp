#include "board.h"
#include "cell.h"
#include "text_cell.h"

#include <QGraphicsScene>
#include <QGraphicsGridLayout>
#include <QGraphicsWidget>

Game::Board::Board(Game::NonogramDataPtr data, QWidget *parent):
    QGraphicsView(parent), _data(data), _isSolveShown(false)
{
    _scene = new QGraphicsScene;
    QMargins margins(0, 0, 0, 0);

    QGraphicsWidget *form = new QGraphicsWidget;
    QGraphicsGridLayout *grid = new QGraphicsGridLayout(0);
    grid->setSpacing(0);
    form->setContentsMargins(margins);

    QGraphicsWidget *board = new QGraphicsWidget;
    QGraphicsWidget *vHeader = new QGraphicsWidget;
    QGraphicsWidget *hHeader = new QGraphicsWidget;

    _board = new QGraphicsGridLayout(0);
    board->setLayout(_board);
    board->setContentsMargins(margins);
    _board->setContentsMargins(0, 0, 0, 0);

    _vHeader = new QGraphicsGridLayout(0);
    _vHeader->setContentsMargins(0, 0, 0, 0);

    vHeader->setLayout(_vHeader);
    vHeader->setContentsMargins(margins);

    _hHeader = new QGraphicsGridLayout(0);
    _hHeader->setContentsMargins(0, 0, 0, 0);
    hHeader->setLayout(_hHeader);
    hHeader->setContentsMargins(margins);

    _board->setSpacing(0);
    _vHeader->setSpacing(0);
    _hHeader->setSpacing(0);

    grid->addItem(hHeader, 0, 1);
    grid->addItem(board,   1, 1);
    grid->addItem(vHeader, 1, 0);

    form->setLayout(grid);
    _scene->addItem(form);

    setScene(_scene);
}

Game::Board::~Board()
{
    for(QGraphicsItem * item : _scene->items()){
        delete item;
    }
}

void Game::Board::createBoard()
{
    clearBoard();
    drawHHeader();
    drawVHeader();
    drawBoard();
}

void Game::Board::clearBoard()
{
    _isSolveShown = false;
    qDeleteAll(_itemsForDelete);
    _itemsForDelete.clear();
}

void Game::Board::drawBoard(){

    const int nRows = _data->getRows().size();
    const int nCols = _data->getColumns().size();
    _items.clear();
    _items.resize(nRows);

    Cell *item = nullptr;
    for(int i = 0; i < nRows; i++){
        for(int j = 0; j < nCols; j++){
            item = new Cell(i, j);
            _itemsForDelete.append(item);
            _items[i].push_back(item);
            _board->addItem(item, i, j);
        }
    }
}

void Game::Board::drawHHeader()
{
    if(!_hHeader)
        return;
    size_t maxColSize = 0;
    for(const std::vector<int> & vect : _data->getColumns()){
        maxColSize = std::max(maxColSize, vect.size());
    }

    int i = 0;
    int j = 0;
    TextCell *item = nullptr;
    for(const std::vector<int> & vect : _data->getColumns()){
        for(const int & val : vect) {
            item = new TextCell(val);
            _itemsForDelete.append(item);
            _hHeader->addItem(item, maxColSize - i, j, Qt::AlignBottom);
            i++;
        }
        j++;
        i = 0;
    }
}

void Game::Board::drawVHeader()
{
    if(!_vHeader)
        return;

    size_t maxRowSize = 0;

    for(const std::vector<int> & vect : _data->getRows()){
        maxRowSize = std::max(maxRowSize, vect.size());
    }

    int j = 0;
    int i = 0;
    TextCell *item = nullptr;
    for(const std::vector<int> & vect : _data->getRows()){
        for(const int & val : vect){
            item = new TextCell(val);
            _itemsForDelete.append(item);
            _vHeader->addItem(item, i, maxRowSize - j, Qt::AlignRight);
            j++;
        }
        i++;
        j = 0;
    }
}

void Game::Board::showResult()
{
    if(!_data)
        return;

    const ResultData &result = _data->getResult();
    for(size_t i = 0; i < result.size(); i++){
        for(size_t j = 0; j < result[i].size(); j++){
            _items[i][j]->setState(result[i][j]);
        }
    }

    _isSolveShown = true;
}

void Game::Board::showPromt()
{
    if(!_data)
        return;

    if(_isSolveShown){
        return;
    }

    State state;
    bool isReady = false;

    const ResultData &result = _data->getResult();
    while(!isReady){
        size_t i = rand()%result.size();
        size_t j = rand()%result[i].size();
        state = result[i][j];
        if(state == State::Unchecked || _items[i][j]->state() == state)
            continue;

        isReady = true;
        _items[i][j]->setState(state);
    }
}

void Game::Board::checkResult()
{
    if(!_data)
        return;

    if(_data->getResult().empty()){
        return;
    }

    State state;
    bool isError = false;

    const ResultData &result = _data->getResult();

    for(size_t i = 0; i < result.size(); i++){
        for(size_t j = 0; j < result[i].size(); j++){
            state = _items[i][j]->state(); ;
            if(state != State::Unknown
                    && state != result[i][j]){
                _items[i][j]->setState(State::Error);
                isError = true;
            }
        }
    }

    _isSolveShown = !isError;
}

void Game::Board::clearResult()
{
    for(size_t i = 0; i < _items.size(); i++){
        for(size_t j = 0; j < _items[i].size(); j++){
            _items[i][j]->setState(State::Unknown);
        }
    }

    _isSolveShown = false;
}
