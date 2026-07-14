#include <iostream>

using namespace std;

int main()
{
    cout << "Enter number of small rooms: ";
    int nr_small_rooms {0};
    cin >> nr_small_rooms;
    cout << "Enter number of large rooms: ";
    int nr_large_rooms {0};
    cin >> nr_large_rooms;
    
    const double price_per_small_room {25};
    const double price_per_large_room {35};
    
    const double tax {0.06};
    const double small_rooms_price {price_per_small_room * nr_small_rooms};
    const double large_rooms_price {price_per_large_room * nr_large_rooms};
    const double price_total {small_rooms_price + large_rooms_price};
    const double tax_total {tax * price_total};

    cout << "Price per small room: $" << price_per_small_room << endl;
    cout << "Price per large room: $" << price_per_large_room << endl;
    cout << "Estimate:" << endl;
    cout << "Rooms: small := " << nr_small_rooms << ", large := " << nr_large_rooms << endl;
    cout << "Cost: $" << price_total << endl;
    cout << "Tax: $" << tax_total << endl;
    cout << "Total with tax: $" << price_total + tax_total << endl;

	
	return 0;
}
