#include <iostream>
#include "City.hpp"
#pragma once

namespace pandemic
{
    class Board
    {

    public:
        Board();

        ~Board();

        int& operator[](const City &city);

        friend std::ostream &operator<<(std::ostream &os, Board &board);

        bool is_clean();

        void read_cities();

        void remove_cures();
    };
}