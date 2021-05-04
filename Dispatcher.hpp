#include "Player.hpp"

namespace pandemic
{
    class Dispatcher : public Player
    {
    public:
        using Player::Player;
        
        Player &fly_direct(const City &city);
    };
}