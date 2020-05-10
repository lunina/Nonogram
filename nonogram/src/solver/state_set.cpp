#include "state_set.h"



Game::StateSet::StateSet(int sz):_sz(sz), arr(std::unique_ptr<bool[]>(new bool[sz]))
{
    reset();
}

Game::StateSet &Game::StateSet::flip(){
    for(unsigned int i = 0; i < _sz; i++){
        arr[i] = !arr[i];
    }
    return *this;
}

bool Game::StateSet::test(unsigned int n) const{
    return arr[n];
}

void Game::StateSet::set(unsigned int n, bool v){
    arr[n] = v;
}

void Game::StateSet::set(){
    for(unsigned int i = 0; i < _sz; i++){
        arr[i] = true;
    }
}

void Game::StateSet::reset(){
    for(unsigned int i = 0; i < _sz; i++){
        arr[i] = false;
    }
}

Game::StateSet &Game::StateSet::operator&=(Game::StateSet &other){
    for(unsigned int i = 0; i < _sz; i++){
        arr[i] &= other.arr[i];
    }
    return *this;
}

bool &Game::StateSet::operator[](int n) const{
    return arr[n];
}
