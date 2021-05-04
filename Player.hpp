#include "Board.hpp"
#include "City.hpp"
#include "Color.hpp"
#pragma once

namespace pandemic
{
    class Player
    {
    private:
        int num_of_card = 0;

    public:
        Player();

        Player(const Board &board, const City &city);

        ~Player();

        Player &drive(const City &city);

        Player &fly_direct(const City &city);

        Player &fly_charter(const City &city);

        Player &fly_shuttle(const City &city);

        Player &build();

        Player &discover_cure(const Color &color);

        Player &treat(const City &city);

        std::string role();

        Player &take_card(const City &city);

        int get_num_of_card()
        {
            return num_of_card;
        }

    };

}