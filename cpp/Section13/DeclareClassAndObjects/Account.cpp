#include "Account.h"

bool Account::deposit(double bal) 
{
    balance += bal;
    std::cout << "In deposit, balance is: " << balance << std::endl;
    return true;
}

bool Account::withdraw(double bal) 
{
    if (balance - bal >= 0) {
        balance -= bal;
        std::cout << "In withdraw, balance is: " << balance << std::endl;
        return true;
    } else {
        std::cout << "Not enough balance" << std::endl;
        return false;
    }
}