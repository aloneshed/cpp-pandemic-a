#include "Player.hpp"

namespace pandemic
{
    class Scientist : public Player
    {
    public:
        using Player::Player;

        Scientist(const Board &board, const City &city, int num_of_cards);

        Player &discover_cure(const Color &color);
    };
}