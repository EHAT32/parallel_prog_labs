#include <numeric>
#include <omp.h>
#include <iostream>
#include <vector>

template<typename T>
T findMax(const std::vector<T>& vec){
    T maxVal = vec[0];
    #pragma omp parallel for
    {
        for (int i = 0; i < vec.size(); i++) {
            #pragma omp critical
                {
                    if (maxVal < vec[i]) {
                        maxVal = vec[i];
                    }
                }
        }
    }
    return maxVal;
}

int main(){
    int num_threads = 12;
    omp_set_num_threads(num_threads);
    std::vector<int> vec(10000000);
    std::iota(vec.begin(), vec.end(), 1);
    int maxVal = findMax(vec);
    std::cout << maxVal << std::endl;
    return 0;
}