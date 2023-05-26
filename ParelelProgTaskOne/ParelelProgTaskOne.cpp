#include <iostream>
#include <omp.h>
#include <chrono>

using namespace std;
using namespace chrono;

void bubbleSort(int arr[], int n) {
    int i, j;
#pragma omp parallel for shared(arr) private(i, j)
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void fillRandNums();
const long int numOfNums = 200000;
int arr[numOfNums];
int main() {
    
    fillRandNums();
    cout << "Nums Generated!" << endl;

    std::cout << "Original array:\n";
    for (int i = 0; i < numOfNums; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    auto start = high_resolution_clock::now();
    omp_set_num_threads(12);
    bubbleSort(arr, numOfNums);

    auto duration = duration_cast<milliseconds>( high_resolution_clock::now() - start);
    
    printf("TIme in ms: %d", duration);

    return 0;
}


void fillRandNums() {
    srand(198921);

    for (long int i = 0; i < numOfNums; i++) {
        arr[i] = rand() % 100000;
    }

}
