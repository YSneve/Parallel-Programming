#include <iostream>
#include <omp.h>
#include <iomanip>
#include <chrono>

using namespace std;
using namespace chrono;
void startTest(long long int);
int main(int argc, const char* argv[])

{
    auto stayAlive = true;
    while (stayAlive) {
        cout << "Enter threads count:" << endl;
        int threadsNum;
        cin >> threadsNum;
        cout << "Enter iterations amount (base and power of it): " << endl;
        int iterBase, iterPower;
        cin >> iterBase >> iterPower;

        long long int iterAmount;
        iterAmount = pow(iterBase, iterPower);
        cout << "Total iterations count  = " << iterAmount << endl;
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

#pragma omp parallel for reduction(+:summ)
    for (long long int i = 1; i < iterAmount / 2; i++) { 
        summ += 4.0 / (i * 2 * (i * 2 + 1) * (i * 2 + 2)) * pow(-1, i % 2 == 0 ? 1 : 2);
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    cout << setprecision(60);
    
    cout << "RPi: 3,14159265358979323846264338327950288419716939937510" << endl;
    
    cout << " Pi: " << summ << ". Time passed = " << duration.count() / 1000 << "s " << duration.count() % 1000 << "ms." << endl;

    cout << "Amount of threds used: " << omp_get_max_threads() << endl;
}