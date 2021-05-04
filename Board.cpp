#include "Board.hpp"
using namespace std;

namespace pandemic
{
    Board::Board(){}

    Board::~Board(){}

    int &Board::operator[](const City &city)
    {
        int *x = new int(1);
        return *x;
    }

    ostream &operator<<(std::ostream &os, Board &board)
    {
        return os;
    }

    bool Board::is_clean()
    {
        return false;
    }

    void Board::read_cities() {}

    void Board::remove_cures() {}
}