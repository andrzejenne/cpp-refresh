#include <iostream>
#include <vector>

using namespace std;

int main()
{
	vector <int> vector1 {};
    vector <int> vector2 {};
    
    vector1.push_back(10);
    vector1.push_back(20);
    
    vector2.push_back(100);
    vector2.push_back(200);
    
    vector <vector<int>> vector_2d {};
    
    vector_2d.push_back(vector1);
    vector_2d.push_back(vector2);
    
    cout << "vec[0][0]" << vector_2d.at(0).at(0) << endl;
    cout << "vec[0][1]" << vector_2d.at(0).at(1) << endl;
    
    cout << "vec[1][0]" << vector_2d.at(1).at(0) << endl;
    cout << "vec[1][1]" << vector_2d.at(1).at(1) << endl;
    
    vector1[0] = 1000;

    cout << "vec[0][0]" << vector_2d.at(0).at(0) << endl;
    cout << "vec[0][1]" << vector_2d.at(0).at(1) << endl;
    
    cout << "vec[1][0]" << vector_2d.at(1).at(0) << endl;
    cout << "vec[1][1]" << vector_2d.at(1).at(1) << endl;

    cout << "v1[0]" << vector1.at(0) << endl;
    cout << "v1[1]" << vector1.at(1) << endl;
    
    vector1.size();
    
	return 0;
}
