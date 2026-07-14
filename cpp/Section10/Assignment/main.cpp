#include <iostream>
#include <string>

using namespace std;

int main()
{
	cout << "Enter a string: ";
    string str_for_pyramid {};
    
    // we dont whant a space characters to break the pyramid, read will end on first one
    cin >> str_for_pyramid;
    
    size_t length = str_for_pyramid.length();
    
    for (size_t i = 0; i < length; ++i) {
        // num of spaces needed for every line
        for (size_t s = length - i - 1; s > 0; --s) {
            cout << ' ';
        }
        // left side
        for (size_t j = 0; j <= i; ++j) {
            cout << str_for_pyramid.at(j);
        }
        // right side
        for (size_t j = i; j > 0; --j) {
            cout << str_for_pyramid.at(j - 1);
        }
        
        cout << endl;
    }
    
	return 0;
}
