#pragma once

#include <numeric>
#include <vector>

#include "state_set.h"
#include "constans.h"

namespace Game {
    //Класс нахождения решения строки|столбца
    class Solution {
    public:
        Solution(int sz, std::vector<int> ni,const int l);

        State getState (const int n) const;
        int fl();
        bool getTx(int n);
        bool getTb(int n);
        void fn(const int n, const int i, const int g, const int e, const int l);
        void fi(const int n,const bool g);
        bool fe(const int n,const int i, const bool g);

    private:
        StateSet  X;
        StateSet  B;
        StateSet  T;
        StateSet  Tx;
        StateSet  Tb;
        std::vector<int> ng;
        int En;
        int gNG;
    };

} // Game

