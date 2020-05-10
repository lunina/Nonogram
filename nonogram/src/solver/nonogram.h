#pragma once

#include "nonogram_data.h"
#include <atomic>
#include <mutex>
#include <memory>
#include <thread>
#include <solver.h>

namespace Game{
    //Класс для загрузки и решения нонограммы
    class Nonogram
    {
    public:
        Nonogram(NonogramDataPtr data, std::mutex &mutex);
        ~Nonogram();

        void run();

        //Загрузить файл
        bool loadData(const std::string &fileName);

        //Рарпарсить файл
        bool parse(std::ifstream *myfile);

        const std::atomic<bool> &isLoaded() const;
        void setIsLoaded(bool isLoaded);

        const std::atomic<bool> &isSolved() const;
        void setIsSolved(bool isSolved);

    private:
        NonogramDataPtr _data;       //Данные для решения

        std::atomic<bool> _isLoaded;//Флаг о загрузке данных
        std::atomic<bool> _isSolved;//Флаг о наличии решения
        std::atomic<bool> _isStoped;
        std::atomic<bool> _isRunning;

        std::unique_ptr<Game::Solver> _solver;

        std::mutex &_mutex;
        std::thread _thread;
    };

    using NonogramPtr = std::unique_ptr<Game::Nonogram>;
}//Game

