#include "Player.hpp"
#include "City.hpp"

namespace pandemic
{
    class Medic : public Player
    {
    public:
        using Player::Player;

        Player &drive(const City &city);

        Player &fly_direct(const City &city);

        Player &fly_charter(const City &city);

        Player &fly_shuttle(const City &city);

        Player &treat(const City &city);
    };
}