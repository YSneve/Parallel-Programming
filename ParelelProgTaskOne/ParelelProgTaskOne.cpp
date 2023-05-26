#include <iostream>
#include <mpi.h>
#include <chrono>

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
        /*std::cout << "Sorted array:\n";
        for (int i = 0; i < n; i++) {
            std::cout << sorted_arr[i] << " ";
        }
        std::cout << std::endl;*/
        delete[] sorted_arr;
    }
}

const long int numOfNums = 30000;

int arr[numOfNums];

void fillRandNums() {
    srand(198921);

    for (long int i = 0; i < numOfNums; i++) {
        arr[i] = rand() % 1000;
    }

}
int main(int argc, char** argv) {

    //std::string ass;
    //std::cin >> ass;

    fillRandNums();

    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

   
    int n = sizeof(arr) / sizeof(arr[0]);

    /*std::cout << "Original array:\n";
    for (int i = 0; i < n; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;*/

    auto start = std::chrono::high_resolution_clock::now();

    bubbleSort(arr, n, rank, size);

    auto sortDuration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);

    std::printf("Time passed: %d\n", sortDuration);

    MPI_Finalize();
    return 0;
}

  


//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------





//#include <iostream>
//#include <mpi.h>
//
//
//    void bubbleSort(int* array, int size) {
//        for (int i = 0; i < size - 1; ++i) {
//            for (int j = 0; j < size - i - 1; ++j) {
//                if (array[j] > array[j + 1]) {
//                    int temp = array[j];
//                    array[j] = array[j + 1];
//                    array[j + 1] = temp;
//                }
//            }
//        }
//    }
//
//    void mergeArrays(int* array, int* temp, int size, int* recvCounts, int* displs, int rank, int numProcesses) {
//        int* sortedArray = new int[size];
//        MPI_Gatherv(array, size, MPI_INT, sortedArray, recvCounts, displs, MPI_INT, 0, MPI_COMM_WORLD);
//
//        if (rank == 0) {
//            for (int i = 0; i < size; ++i) {
//                array[i] = sortedArray[i];
//            }
//
//            for (int i = 1; i < numProcesses; ++i) {
//                MPI_Recv(temp, size, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//                for (int j = 0; j < size; ++j) {
//                    array[j + i * size] = temp[j];
//                }
//            }
//        }
//        else {
//            MPI_Send(array, size, MPI_INT, 0, 0, MPI_COMM_WORLD);
//        }
//
//        delete[] sortedArray;
//    }
//
//    int main(int argc, char** argv) {
//        MPI_Init(&argc, &argv);
//        int rank, numProcesses;
//        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//        MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
//
//        const int size = 1000;
//        int* array = new int[size];
//        int* temp = new int[size];
//        int* recvCounts = new int[numProcesses];
//        int* displs = new int[numProcesses];
//
//        if (rank == 0) {
//            // Инициализация массива случайными значениями на процессе 0
//            srand(time(NULL));
//            for (int i = 0; i < size; ++i) {
//                array[i] = rand() % 100;
//              /*  std::cout << array[i] << " ";*/
//            }
//           /* std::cout << std::endl;*/
//        }
//
//        // Рассылка массива на все процессы
//        MPI_Bcast(array, size, MPI_INT, 0, MPI_COMM_WORLD);
//
//        //std::cout << "STARTED SORTING " << std::endl;
//        // Разделение массива на равные части для каждого процесса
//        int blockSize = size / numProcesses;
//        int remainingElements = size % numProcesses;
//        int* localArray = new int[blockSize + remainingElements];
//        MPI_Scatter(array, blockSize, MPI_INT, localArray, blockSize, MPI_INT, 0, MPI_COMM_WORLD);
//
//        // Сортировка своей части массива
//        bubbleSort(localArray, blockSize);
//
//        // Сбор всех частей массива
//        MPI_Gather(&blockSize, 1, MPI_INT, recvCounts, 1, MPI_INT, 0, MPI_COMM_WORLD);
//
//        if (rank == 0) {
//            displs[0] = 0;
//            for (int i = 1; i < numProcesses; ++i) {
//                displs[i] = displs[i - 1] + recvCounts[i - 1];
//            }
//        }
//
//        mergeArrays(localArray, temp, blockSize, recvCounts, displs, rank, numProcesses);
//
//        if (rank == 0) {
//            /*std::cout << "Sorted array: ";
//            for (int i = 0; i < size; ++i) {
//                std::cout << array[i] << " ";
//            }
//            std::cout << std::endl;
//        */}
//
//        delete[] array;
//        delete[] temp;
//        delete[] recvCounts;
//        delete[] displs;
//        delete[] localArray;
//
//        MPI_Finalize();
//        return 0;
//    }
