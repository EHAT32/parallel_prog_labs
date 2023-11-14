#include <iostream>
#include <omp.h>
#include <vector>
#include <numeric>

template <typename T>
T reductionAtomic(const std::vector<T>& vec){
    T sum = 0;
    double startTime = omp_get_wtime();
    #pragma parallel for reduction(+ : sum)
    {
        for (int i = 0; i < vec.size(); i++) {
            sum += vec[i];
        }
    }
    double endTime = omp_get_wtime();
    std::cout << "Elapsed time is " << endTime - startTime << " seconds" << std::endl;
    return sum;
}

template<typename T>
T reductionCritical(const std::vector<T>& vec){
    T sum = 0;
    return sum;
}

template<typename T>
T reductionLock(const std::vector<T>& vec){
    T sum = 0;
    return sum;
}

int main(){
    int num_threads = 2;
    omp_set_num_threads(num_threads);
    std::vector<int> vec(100000000);
    std::iota(vec.begin(), vec.end(), 1);
    reductionAtomic(vec);
    return 0;
}