#include "Player.hpp"


namespace pandemic
{
    class FieldDoctor : public Player
    {
    public:
        using Player::Player;

        Player &treat(const City &city);
    };
}