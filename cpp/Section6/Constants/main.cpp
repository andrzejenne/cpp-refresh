#include <iostream>

using namespace std;

int main()
{
	cout << "Enter number of rooms: " << endl;
    int number_of_rooms {0};
    cin >> number_of_rooms;
    
    const double price_per_room {30};
    const double tax {0.06};
    const double price_no_tax {price_per_room * number_of_rooms};
    const double price_tax {price_no_tax * tax};
    
    cout << "Nr of Rooms: " << number_of_rooms << endl;
    cout << "Cost per Room: " << price_per_room << endl;
    cout << "Cost: $" << price_no_tax << endl;
    cout << "Tax: $" << price_tax << endl;
    cout << "Total Estimate: $" << price_no_tax + price_tax << endl;
//    const 
    
	return 0;
}
