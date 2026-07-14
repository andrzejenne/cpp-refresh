#include <iostream>
#include <vector>

using namespace std;

int main()
{

    vector <char> vowels {'a', 'e', 'i', 'o', 'u'};
    
    // array native access
    cout << "Array syntax" << endl;
    cout << vowels[0] << endl;
    cout << vowels[4] << endl;
    
    // vector method

    cout << "Vector syntax" << endl;
    cout << vowels.at(1) << endl;
    cout << vowels.at(3) << endl;
    
    cout << "There are " << vowels.size() << " vowels total" << endl;
    
    
    // push back
    vector <int> score;
    
    score.push_back(1);
    
    cout << "We have push back call " << score.at(0) << endl;
    
    // dimensions
    
    vector <vector<int>> dim { {1,2}, {3,4} };
    cout << "We have a dimension and first value is " << dim.at(0).at(0) << endl;
    
    
	return 0;
}
