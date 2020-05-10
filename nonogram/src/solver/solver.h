#pragma once

#include <stdint.h>
#include <vector>
#include <sstream>
#include <numeric>
#include <atomic>
#include "solution.h"

namespace Game {
    //Кдасс нахождения решения нанограммы
    class Solver {

    public:
        Solver(std::atomic<bool> &stoped);

        void setTask(const std::vector<std::vector<int>> &rows,
                     const std::vector<std::vector<int>> &cols);

        bool solve();

        std::vector<std::vector<State>> getResult();

    private:
        void setCell(unsigned int n, unsigned int i, bool g);

    private:
        std::atomic<bool> &_isStoped;
        size_t En;
        size_t rowSize;
        size_t colSize;
        std::vector<Solution> row_solution;
        std::vector<Solution> col_solution;
        std::vector<std::vector<State>> _result;
    };
}//Game

