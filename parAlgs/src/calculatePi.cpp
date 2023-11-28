#include <iostream>
#include <omp.h>

template<typename T>
float trapInt(const T& function, const float& a = 0, const float& b = 1, const int& N = 100000){
    double startTime = omp_get_wtime();
    float res = 0;
    float x = a;
    float h = (b - a) / N;
    for (int i = 0; i < N - 1; i++) {
        res += (function(x + i * h) + function(x + (i+1) * h)) / 2 * h;
    }
    std::cout << "Linear time is: " << omp_get_wtime() - startTime << " seconds" << std::endl;
    return res;
}

float forPi(const float& x){
    return 4 / (1 + x * x);
}

int main(){
    std::cout << trapInt(forPi) << std::endl;
    return 0;
}