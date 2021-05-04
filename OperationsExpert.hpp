#include "Player.hpp"

namespace pandemic
{
    class OperationsExpert : public Player
    {
    public:
        using Player::Player;
        
        Player &build();
    };
}