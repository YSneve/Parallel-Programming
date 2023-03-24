#include <iostream>
#include <algorithm>
#include <vector>
#include <thread>
#include <cmath>
#include <chrono>

void merge(std::vector<int>& v, int low, int mid, int high) {
    std::vector<int> temp(high - low + 1);
    int i = low, j = mid + 1, k = 0;

    while (i <= mid && j <= high) {
        if (v[i] <= v[j])
            temp[k++] = v[i++];
        else
            temp[k++] = v[j++];
    }
    while (i <= mid) temp[k++] = v[i++];
    while (j <= high) temp[k++] = v[j++];

    for (int i = low; i <= high; i++)
        v[i] = temp[i - low];
}

void batcher_sort(std::vector<int>& v, int low, int high) {
    if (low < high) {
        int mid = (low + high) / 2;
        batcher_sort(v, low, mid);
        batcher_sort(v, mid + 1, high);
        merge(v, low, mid, high);
    }
}

void parallel_batcher_sort(std::vector<int>& v, int num_threads) {
    int n = v.size();
    int chunk_size = ceil((double)n / num_threads);
    std::vector<std::thread> threads;

    for (int i = 0; i < num_threads; i++) {
        int low = i * chunk_size;
        int high = std::min((i + 1) * chunk_size - 1, n - 1);
        threads.push_back(std::thread(batcher_sort, std::ref(v), low, high));
    }

    for (int i = 0; i < num_threads; i++)
        threads[i].join();

    // Последний проход слияния
    for (int i = 1; i < num_threads; i++)
        merge(v, 0, i * chunk_size - 1, std::min((i + 1) * chunk_size - 1, n - 1));
}

int main() {
    setlocale(LC_ALL, "Russian");
    std::vector<int> v;

    // Инициализация генератора случайных чисел
    srand(time(NULL));

    // Заполнение вектора случайными числами
    for (int i = 0; i < 16000000; i++) {
        int randomNumber = rand() % 16000000;
        v.push_back(randomNumber);
    }

    int num_threads = 1;
    /*
    std::cout << "Исходный вектор: ";
    for (int i = 0; i < v.size(); i++)
        std::cout << v[i] << " ";
    std::cout << std::endl;S
    */

    auto start = std::chrono::high_resolution_clock::now();
    parallel_batcher_sort(v, num_threads);
    auto stop = std::chrono::high_resolution_clock::now();

    /*std::cout << "Отсортированный вектор: ";
    for (int i = 0; i < v.size(); i++)
        std::cout << i;*/

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Sort duration:" << duration.count() << std::endl;
}
        