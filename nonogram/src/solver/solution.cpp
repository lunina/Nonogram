#include "solution.h"

Game::Solution::Solution(int sz, std::vector<int> ni, const int l) :
    X{sz}, B{sz}, T(sz), Tx(sz), Tb{sz}, ng(ni), En{}, gNG(l)
{

}

void Game::Solution::fn(const int n, const int i,
                        const int g, const int e, const int l)
{
    if(fe(g, l, false) && fe(g+l, e, true)) {
        if(static_cast<size_t>(n+1) < ng.size()) {
            if(fe(g+e+l,1,false)) {
                fn(n + 1, i - e - 1, g + e + l + 1, ng[n + 1], 0);
            }
        }
        else {
            if(fe(g + e + l, gNG - (g + e + l), false)) {
                Tb &= T.flip();
                Tx &= T.flip();
                ++En;
            }
        }
    }

    if(l <= gNG - g - i - 1)
        fn(n, i, g, e, l+1);
}

void Game::Solution::fi(const int n, const bool g)
{
    X.set(n, g);
    B.set(n, !g);
}

Game::State Game::Solution::getState(const int n) const
{
    State state = State::Unknown;
    if(X.test(n)) {
        state = State::Checked;
    }
    else if(B.test(n)) {
        state = State::Unchecked;
    }

    return state;
}

bool Game::Solution::fe(const int n, const int i, const bool g)
{
    for (int e = n; e < n+i; ++e) {
        if((g && getState(e) == State::Unchecked)
                || (!g && getState(e) == State::Checked)){
            return false;
        }
        else {
            T[e] = g;
        }
    }
    return true;
}

int Game::Solution::fl()
{
    if(En == 1) {
        return 1;
    }

    Tx.set();
    Tb.set();
    En = 0;

    fn(0, std::accumulate(ng.cbegin(), ng.cend(), 0) + ng.size() - 1, 0, ng[0], 0);

    return En;
}

bool Game::Solution::getTx(int n)
{
    return Tx[n];
}

bool Game::Solution::getTb(int n)
{
    return Tb[n];
}
