#include <iostream>

using namespace std;

int *apply_all(int*, int, int*, int);

int main() 
{
    int arr1[] = {1, 2, 3, 4, 5};
    int arr2[] = {10, 20, 30};
    int *result = apply_all(arr1, 5, arr2, 3);

    for (int i {0}; i < 15; ++i) {
        cout << result[i] << endl;
    }
    return 0;
}

int *apply_all(int* arr1, int s1, int* arr2, int s2)
{
    int *result = new int[s1 * s2];
    for (int i {0}; i < s1; ++i) {
        for (int j {0}; j < s2; ++j) {
            result[i + j * s1] = arr1[i] * arr2[j];
        }
    }

    return result;
}