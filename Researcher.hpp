#include "Player.hpp"


namespace pandemic
{
    class Researcher : public Player
    {
    public:
        using Player::Player;

        Player &discover_cure(const Color &color);
    };
}