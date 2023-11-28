#include <errno.h>
#include <iostream>
#include <omp.h>
#include <ostream>
#include <vector>

template<typename T>
double trapInt(const T& function, const float& a = 0, const float& b = 1, const int& N = 100000){
    double startTime = omp_get_wtime();
    float res = 0;
    float x = a;
    float h = (b - a) / N;
    for (int i = 0; i < N - 1; i++) {
        res += (function(x + i * h) + function(x + (i+1) * h)) / 2 * h;
    }
    double duration = omp_get_wtime() - startTime;
    // std::cout << "Linear time is: " << duration << " seconds" << std::endl;
    return duration;
}

template<typename T>
float OMPTrapInt(const T& function, const float& a = 0, const float& b = 1, const int& N = 100000){
    double startTime = omp_get_wtime();
    float res = 0;
    float x = a;
    float h = (b - a) / N;
    #pragma omp parallel
    {
        #pragma omp sections
        {
            #pragma omp section
            {
                float int1 = 0;
                for (int i = 0; i < N / 2; i ++) {
                    int1 += (function(x + i * h) + function(x + (i+1) * h)) / 2 * h;
                }
                #pragma omp critical
                {
                    res += int1;
                }
            }
            #pragma omp section
            {
                float int2 = 0;
                for (int i = N / 2; i < N - 1; i++) {
                    int2 += (function(x + i * h) + function(x + (i + 1) * h)) / 2 * h;
                }
                #pragma omp critical
                {
                    res += int2;
                }
            }
        }
    }
    double duration = omp_get_wtime() - startTime;
    // std::cout << "OMP time is: " << duration << " seconds" << std::endl;
    return duration;
}

template<typename T>
float OMPForTrapInt(const T& function, const float& a = 0, const float& b = 1, const int& N = 100000){
    double startTime = omp_get_wtime();
    float x = a;
    float h = (b - a) / N;
    float res = 0;
    #pragma omp parallel
    {
        float locRes = 0;
        #pragma omp for
        {
            for (int i = 0; i < N - 1; i++) {
                locRes += (function(x + i * h) + function(x + (i+1) * h)) / 2 * h;
            }
        }
        #pragma omp critical
        {
            res += locRes;
        }
    }
    double duration = omp_get_wtime() - startTime;
    // std::cout << "OMP for time is: " << duration << " seconds" << std::endl;
    return duration;
}

float forPi(const float& x){
    return 4 / (1 + x * x);
}

int main(){
    int num_threads = 2;
    omp_set_num_threads(num_threads);
    int N = 1000;
    double linTime = 0;
    double forTime = 0;
    std::cout << "Threads num: " << num_threads << std::endl;
    for (int i = 0; i < N; i++) {
        linTime += trapInt(forPi);
        forTime += OMPForTrapInt(forPi);
    }
    linTime /= N;
    forTime /= N;
    std::cout << "Lin time: " << linTime << std::endl;
    std::cout << "OMP For time: " << forTime  << std::endl;
    float speedUp = linTime / forTime;
    std::cout << "SpeedUp: " << speedUp << std::endl;
    float efficiency = speedUp / num_threads;
    std::cout << "Efficiency: " << efficiency << std::endl;
    return 0;
}