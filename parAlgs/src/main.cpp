#include <iostream>
#include <ostream>
#include <vcruntime.h>
#include <vcruntime_string.h>
#include <vector>
#include <cassert>
#include <xmmintrin.h> 
#include <omp.h>
#include <cstring>
#include <cmath>

float linearSum(const float* array, const size_t& size){
    double startTime = omp_get_wtime();
    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum += array[i];
    }
    double duration = omp_get_wtime() - startTime;
    return duration;
}

float sseSum_(const float* arr, const size_t& size){
    double startTime = omp_get_wtime();
    __m128 sum = _mm_setzero_ps();

    for (int i = 0; i < size; i += 4) {
        __m128 data = _mm_loadu_ps(&arr[i]);
        sum = _mm_add_ps(sum, data);
    }

    float result[4];
    _mm_storeu_ps(result, sum);

    float finalSum = result[0] + result[1] + result[2] + result[3];
    double duration = omp_get_wtime() - startTime;
    return duration;
}

float forSum(const float* arr, const size_t& size){
    float sum = 0;
    double startTime = omp_get_wtime();
    #pragma omp parallel shared(arr)
    {
        float localSum = 0;
        #pragma omp for
        {
            for (int i = 0; i < size; i++) {
                localSum += arr[i];
            }
        }
        #pragma omp critical
        {
            sum += localSum;
        }
    }
    double duration = omp_get_wtime() - startTime;
    return duration;
}

float sectionsSum(const float* arr, const size_t& size){
    double startTime = omp_get_wtime();
    float sum = 0;
    float sum1 = 0;
    float sum2 = 0;
    #pragma omp parallel shared(arr)
    {
        #pragma omp sections
        {
            #pragma omp section
            {
                for (int i = 0; i < size / 2; i++) {
                    sum1 += arr[i];
                }
            }
            #pragma omp section
            {
                for (int i = size / 2; i < size; i++) {
                    sum2 += arr[i];
                }
            }
        }
    }
    sum = sum1 + sum2;
    double duration = omp_get_wtime() - startTime;
    return duration;
}

//я сделал копирование на векторе, потому что со стандартным массивом на последней итерации почему-то y[1] становится -nan
float cascadeSum(const float* arr, const size_t& size){
    float startTime = omp_get_wtime();
    size_t n = size;
    std::vector<float> y(n, 1);
    while (n >= 2) {
        int prevN = n;
        n /= 2;
        for (size_t i = 0; i < n; i++) {
            y[i] = y[2 * i] + y[2*i + 1];
        if (prevN % 2) {
            y[0] += y[prevN - 1];
        }
        }
    }
    double duration = omp_get_wtime() - startTime;
    return duration;
}

void experiment(const size_t& size){
    float array[size];
    for (int i = 0; i < size; i++) {
        array[i] = 1;
    }
    int N = 1000;
    double linTime = 0;    
    double sseTime = 0;
    double sectionTime = 0;
    double forTime = 0;
    double cascadeTime = 0;
    for (int i = 0; i < N; i++) {
        linTime += linearSum(array, size);
        sseTime += sseSum_(array, size);
        cascadeTime += cascadeSum(array, size);
        forTime += forSum(array, size);
        sectionTime += sectionsSum(array, size);
    }
    linTime /= N;
    sseTime /= N;
    cascadeTime /= N;
    forTime /= N;
    sectionTime /= N;
    std::cout << "For size " << size << std::endl;
    std::cout << "SSE speed up is " << linTime / sseTime << std::endl;
    std::cout << "Cascade speed up is " << linTime / cascadeTime << std::endl;
    std::cout << "OMP for speed up is " << linTime / forTime << std::endl;
    std::cout << "OMP sections speed up is " << linTime / sectionTime << std::endl;
    std::cout << "-------------------" << std::endl;
}

int main(){
    size_t size = 8192;
    experiment(size);
    experiment(size * 2);
    experiment(size * 4);
    return 0;
}
//speed up
/*
| method\\size | 8192 | 16384 | 32768 |
| ------------ | ---- | ----- | ----- |
| linear       | 1    | 1     | 1     |
| sse          | 2,42 | 2,42  | 2,45  |
| cascade      | 0,02 | 0,03  | 0,13  |
| omp for      | 1,36 | 1,74  | 3,76  |
| omp sections | 0,44 | 0,49  | 0,49  |
*/