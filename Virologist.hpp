#include "Player.hpp"

namespace pandemic
{
    class Virologist : public Player
    {
    public:
        using Player::Player;

        Player &treat(const City &city);
    };
}