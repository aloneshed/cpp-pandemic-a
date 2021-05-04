#include "Player.hpp"


namespace pandemic
{
    class GeneSplicer : public Player
    {
    public:
        using Player::Player;

        Player &discover_cure(const Color &color);
    };
}