#include <iostream>
#include <mpi.h>

void bubbleSort(int arr[], int n, int rank, int size) {
    int i, j;

    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }

    int* sorted_arr = nullptr;
    if (rank == 0) {
        sorted_arr = new int[n];
    }

    MPI_Gather(arr, n / size, MPI_INT, sorted_arr, n / size, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "Sorted array:\n";
        for (int i = 0; i < n; i++) {
            std::cout << sorted_arr[i] << " ";
        }
        std::cout << std::endl;
        delete[] sorted_arr;
    }
}

int main(int argc, char** argv) {
    std::string ass;
    std::cin >> ass;


    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int arr[] = { 64, 34, 25, 12, 22, 11, 90 };
    int n = sizeof(arr) / sizeof(arr[0]);

    std::cout << "Original array:\n";
    for (int i = 0; i < n; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    bubbleSort(arr, n, rank, size);

    MPI_Finalize();
    return 0;
}