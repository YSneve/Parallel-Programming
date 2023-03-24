// paralelProgCpp.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include <omp.h>
#include <iomanip>
#include <chrono>

using namespace std;
using namespace chrono;
void startTest(long long int);
int main (int argc, const char * argv[])

{
    setlocale(LC_ALL, "Russian");

    auto stayAlive = true;
    while (stayAlive) {
        cout << "Введите число потоков:" << endl;
        int threadsNum;
        cin >> threadsNum;
        cout << "Введите число итераций (степень 10): " << endl;
        long long int iterAmount;
        cin >> iterAmount;
        iterAmount = pow(10, iterAmount);
        cout << "Кол-во итераций = " << iterAmount << endl;
        omp_set_num_threads(threadsNum);

        startTest(iterAmount);

        cout << "Continue? Y/N" << endl;
        string text;
        cin >> text;
        if (text == "n" || text == "N")
            stayAlive = false;

    }
}

void startTest(long long int iterAmount) {

    long double summ = 3.0;

    double PosGrowth = (double)iterAmount / (double)omp_get_max_threads(); // start pos growth(exmp: for 10 threads with 1000 iters will be 100
    
    auto start = high_resolution_clock::now();

#pragma omp parallel for schedule(dynamic)

    for (int i = 0; i < omp_get_max_threads(); i++) {
        
        int startPos = PosGrowth * i; //start pos for cycle (exemp: for 2 - nd thread out of 10 with 1000 iters will be 100
        int precisionStartPos = startPos + startPos % 2; // making cycle going through only 0, 2, 4 etc.

        int endPos = PosGrowth * (i + 1); //end pos for cycle (exemp: for 2 - nd thread out of 10 with 1000 iters will be 200
        int precisionEndPos = endPos + endPos % 2; // making cycle stop on 0, 2, 4 etc.

        if (precisionStartPos == 0) precisionStartPos = 2;
        
        long double forSumm = 0.0;
        
        cout << "Start: " << precisionStartPos << " End: " << precisionEndPos << " Growth: " << PosGrowth << endl;
        for (int iter = precisionStartPos; iter < precisionEndPos; iter += 2)
            forSumm += 4.0 / ((double)iter * ((double)iter + 1.0) * ((double)iter + 2.0)) * (double)pow(-1, iter % 4 == 0 ? 1 : 2);
        summ += forSumm;
    }

    
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << setprecision(60);
    cout << "RPi: 3,14159265358979323846264338327950288419716939937510" << endl;
    cout << " Pi: " << summ << ". Time passed = " << duration.count() << " ms." << endl;
    cout << "Amount of threds used: " << omp_get_max_threads() << endl;
}