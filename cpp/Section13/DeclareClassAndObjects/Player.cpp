#include "Player.h"

void Player::talk(std::string text_to_say) 
{ 
    std::cout << name << " says: " << text_to_say << std::endl; 
};

Player::Player()
    : name{"None"}, health{100}, xp{3} {
}