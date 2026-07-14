#include <iostream>

using namespace std;

int main()
{
    // Character types
    char middle_initial {'x'};
    
    cout << "My middle_initial is " << middle_initial << endl;
    
    // Integer types
    unsigned short int exam_score = {55};
    
    cout << exam_score << endl;
    
    int countries_represented = {65};
    
    cout << countries_represented << endl;
    
    long people_in_florida = {20610000};
    
    cout << people_in_florida << endl;
    
    long long people_on_earth {7'600'000'000}; // c++14 '
    
    cout << people_on_earth << endl;
    
    long long distance_to_alpha_centauri {9'461'000'000'000};
    
    cout << distance_to_alpha_centauri << endl;

	return 0;
}
