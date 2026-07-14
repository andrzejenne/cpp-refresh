#pragma once
#include <string>
#include <iostream>

class Account {
public:
    // properties
    std::string name;
    double balance;

    bool deposit(double bal);

    bool withdraw(double bal);
};