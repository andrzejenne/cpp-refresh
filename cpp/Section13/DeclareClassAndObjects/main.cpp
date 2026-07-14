#include <iostream>

#include "Player.h"
#include "Account.h"

int main()
{
    Account frank_account;
    frank_account.name = "Frank's account";
    frank_account.balance = 1000.0;

    frank_account.deposit(500.0);
    frank_account.withdraw(2000.0);


    Player frank;
    Player hero;

    frank.name = "Frank";
    frank.health = 100;
    frank.xp = 12;

    hero.name = "Hero";
    hero.health = 150;
    hero.xp = 20;

    frank.talk("Hi there!");

    Player *enemy = new Player; // dynamically allocated memory
    enemy->name = "Enemy";
    enemy->health = 100;
    enemy->xp = 15;
    enemy->talk("I will destroy you!");
    delete enemy; // don't forget to free the allocated memory

    return 0;
}