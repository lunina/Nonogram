#pragma once

namespace Game {
    //Состояние ячейки
    enum class State : char{
        Unchecked = 0,
        Checked = 1,
        Unknown = -1,
        Error = -2
    };

    constexpr int cellSide = 30;
}//Game

