#include <iostream>

using namespace std;

int main()
{
	/*
     * 1 dollar is 100 cents
     * 1 quarter is 25 cents
     * 1 dime is 10 cents
     * 1 nickel is 5 cents
     * 1 penny is 1 cent
     */
     
    cout << "Enter an amount in cents: ";
    int amount_cents;
    cin >> amount_cents;
    
    if (amount_cents <= 0) {
        cout << "You are broke" << endl;
        return 0;
    }
    
    const int dollar_conversion {100};
    const int quarter_conversion {25};
    const int dime_conversion {10};
    const int nickel_conversion {5};

    const int amount_dollars = amount_cents / dollar_conversion;
    cout << "dollars: " << amount_dollars << endl;
    amount_cents %= dollar_conversion;
    
    const int amount_quarters = amount_cents / quarter_conversion;
    amount_cents %= quarter_conversion;
    cout << "quarters: " << amount_quarters << endl;
    
    const int amount_dimes = amount_cents / dime_conversion;
    amount_cents %= dime_conversion;
    cout << "dimes: " << amount_dimes << endl;

    const int amount_nickels = amount_cents / nickel_conversion;
    amount_cents %= nickel_conversion;
    cout << "nickels: " << amount_nickels << endl;
    
    cout << "pennies: " << amount_cents << endl;
    
	return 0;
}
