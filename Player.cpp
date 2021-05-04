#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
using namespace std;

#include "Player.hpp"

namespace pandemic
{
    Player::Player() {}

    Player::Player(const Board &board, const City &city) {}

    Player::~Player() {}

    Player &Player::drive(const City &city)
    {
        return *this;
    }

    Player &Player::fly_direct(const City &city)
    {
        return *this;
    }

    Player &Player::fly_charter(const City &city)
    {
        return *this;
    }

    Player &Player::fly_shuttle(const City &city)
    {
        return *this;
    }

    Player &Player::build()
    {
        return *this;
    }

    Player &Player::discover_cure(const Color &color)
    {
        return *this;
    }

    Player &Player::treat(const City &city)
    {
        return *this;
    }

    string Player::role()
    {
        return "";
    }

    Player &Player::take_card(const City &city)
    {
        return *this;
    }
}