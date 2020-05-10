#pragma once

#include <memory>

namespace Game {
    //Вспомогательный класс для работы с состоянием ячеек
    class StateSet{
    public:
        StateSet(int sz);

        StateSet &flip();

        bool test(unsigned int n) const;
        void set(unsigned int n, bool v);
        void set();
        void reset();

        StateSet &operator&=(StateSet &other);
        bool &operator[](int n) const;

        unsigned int _sz;
        std::unique_ptr<bool[]> arr;
    };
}//Game
