#pragma once
#include <string>
#include <iostream>

class Player {
public:
    // properties
    std::string name;
    int health;
    int xp;

    // methods
    bool is_dead();
    void talk(std::string text_to_say);
};