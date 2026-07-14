#include <iostream>

using namespace std;

void reverse_array(int* arr, int size) {
    //-- Write your code below this line
    // cout << size / 2;
    for (int i {0}; i < size / 2; i++) {
        cout << i << ": " << arr[i] << endl;
        cout << i << ": " << arr[size - i - 1] << endl;
        arr[i] = arr[i] + arr[size - i - 1];
        arr[size - i - 1] = arr[i] - arr[size - i - 1];
        arr[i] = arr[i] - arr[size - i - 1];
    }
    //-- Write your code above this line
}

int main()
{
    int *p {nullptr};

    p = new int;

    // cout << p << endl;

    delete [] p;

    int arr[] = {1,2,3,4,5};

    cout << *arr << endl;

    reverse_array(arr, 5);

    cout << *arr << endl;

    return 0;
}