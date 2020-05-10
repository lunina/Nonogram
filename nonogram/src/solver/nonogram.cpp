#include "nonogram.h"

#include <iostream>
#include <memory>
#include <fstream>	// File reading
#include <string.h>
#include <sstream>

#include <future>
#include <memory>
#include <thread>
#include <threadpoolapiset.h>
#include <condition_variable>

#include "solver.h"
#include <QDebug>

Game::Nonogram::Nonogram(NonogramDataPtr data, std::mutex &mutex):
    _data(data),
    _isLoaded{false},
    _isSolved{false},
    _isStoped{false},
    _isRunning{true},
    _solver(std::unique_ptr<Game::Solver>(new Game::Solver(_isStoped))),
    _mutex{mutex}
{
    run();
}

Game::Nonogram::~Nonogram()
{
    _isStoped = false;
    _isLoaded = false;
    _isRunning = false;
    _thread.join();
}

void Game::Nonogram::run()
{
    _thread = std::thread([=]() {
        while(_isRunning){
            //проверяем флаги загрузки и отсутствия решения
            if(_isLoaded && !_isSolved){
                std::cout<<std::this_thread::get_id()<<std::endl;
                _mutex.lock();
                _solver->setTask(std::move(_data->getRows()), std::move(_data->getColumns()));
                _mutex.unlock();
                bool res =  _solver->solve();
                _mutex.lock();
                _data->setResult(_solver->getResult());
                _mutex.unlock();
                //ставим флаг, что решение есть
                _isSolved  = res;
            }
        }
    });
}

bool Game::Nonogram::loadData(const std::string &fileName)
{
    if(fileName.empty()){
        return false;
    }

    //Читаем файл
    std::ifstream myfile(fileName, std::ifstream::out);
    if(!myfile){
        return false;
    }

    //устанавливаем флаг, что решения еще нет
    _isSolved = false;
    _isLoaded = false;
    _isStoped = true;

    std::unique_lock<std::mutex> lk(_mutex);
    if(!_data)
        return false;

    _data->clear();
    //Парсим
    try {
        _isLoaded = parse(&myfile);
    }
    catch(const std::invalid_argument& ex){
        std::cout<<ex.what()<<std::endl;
    }

    myfile.close();

    if(!_isLoaded){
        _data->clear();
    }

    return _isLoaded;
}

bool Game::Nonogram::parse(std::ifstream *myfile)
{
    if(!_data)
        return false;

    std::string line;
    bool isColumns = false;
    bool isError = false;

    //лямбда для чтения строк и столбцов
    auto readData = [&line, &isError](std::vector<std::vector<int>> &vec)
    {
        std::string buf;
        std::stringstream ss(line);
        std::vector<std::string> tokens;
        while (ss >> buf)
            tokens.push_back(buf);

        size_t tokensSize = tokens.size();
        if(!tokensSize){
            isError = true;
        }

        std::vector<int> values(tokensSize);
        for (size_t i = 0; i < tokensSize; i++) {
            values[i] = stoi(tokens[i]);
        }

        vec.emplace_back(values);
    };

    while (std::getline(*myfile, line))
    {
        if(line[0] == '*') {
            continue;
        }
        if(line[0] == '&') {
            isColumns = true;
            continue;
        }

        //Читаем столбцы
        if(isColumns) {
            readData(_data->getColumns());
        }
        //Читаем строки
        else{
            readData(_data->getRows());
        }
        if(isError)
            break;
    }

    return !isError;
}

const std::atomic<bool> &Game::Nonogram::isSolved() const
{
    return _isSolved;
}

void Game::Nonogram::setIsSolved(bool isSolved)
{
    _isSolved = isSolved;
}

void Game::Nonogram::setIsLoaded(bool isLoaded)
{
    _isLoaded = isLoaded;
}

const std::atomic<bool> &Game::Nonogram::isLoaded() const
{
    return _isLoaded;
}


