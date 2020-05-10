#include "solver.h"
#include <algorithm>
#include <iostream>
#include <atomic>
#include "constans.h"

Game::Solver::Solver(std::atomic<bool> &stoped):
    _isStoped{stoped},
    En{0},
    rowSize{0},
    colSize{0},
    row_solution{},
    col_solution{},
    _result{0}
{

}

void Game::Solver::setTask(const std::vector<std::vector<int> > &rows,
                           const std::vector<std::vector<int> > &cols)
{
    _isStoped = false;
    En = 0;
    rowSize = rows.size();
    colSize = cols.size();
    row_solution.clear();
    col_solution.clear();
    _result.clear();
    _result.resize(rows.size());

    std::vector<std::vector<int>> g{};

    //Загружаем решение для кажного столбца
    for (size_t n = 0; n < colSize; n++)
        col_solution.push_back(Solution(rowSize, cols[n], rowSize));

    //Загружаем решение для каждой строки
    for (size_t i = 0; i < rowSize; i++) {
        row_solution.push_back(Solution(colSize, rows[i], colSize));

        if(i < g.size())
            for(size_t e = 0; e < colSize || e < g[i].size(); e++)
                if(g[i][e] == static_cast<int>(State::Checked))
                    setCell(i, e, true);
    }
}

bool Game::Solver::solve()
{
    //Прерываем решение
    if(_isStoped){
        std::cout<<__LINE__<<"stop"<<std::endl;
        return false;
    }

    size_t i{}, g{};
    for (size_t l = 0; l < rowSize; l++) {
        //Прерываем решение
        if(_isStoped){
            std::cout<<__LINE__<<"stop"<<std::endl;
            return false;
        }
        if((g = row_solution[l].fl()) == 0){
            return false;
        }
        else
            i+=g;

        for (size_t i = 0; i < colSize; i++)
            if(row_solution[l].getTx(i) != row_solution[l].getTb(i))
                setCell (l, i, row_solution[l].getTx(i));
    }
    for (size_t l = 0; l < colSize; l++) {
        //Прерываем решение
        if(_isStoped){
            std::cout<<__LINE__<<"stop"<<std::endl;
            return false;
        }
        if((g = col_solution[l].fl()) == 0)
            return false;
        else
            i += g;
        for (size_t i = 0; i < rowSize; i++)
            if(col_solution[l].getTx(i) != col_solution[l].getTb(i))
                setCell (i, l, col_solution[l].getTx(i));
    }

    if(i == En){
        return true;
    }
    else
        En = i;

    return (i == (rowSize + colSize)) ? true : solve();
}

void Game::Solver::setCell(unsigned int n, unsigned int i, bool g)
{
    row_solution[n].fi(i, g);
    col_solution[i].fi(n, g);
}

std::vector<std::vector<Game::State>> Game::Solver::getResult()
{
    for (size_t i = 0; i < rowSize; i++){
        for (size_t g = 0; g < colSize; g++){
            _result[i].push_back(row_solution[i].getState(g));
        }
    }
    return _result;
}
