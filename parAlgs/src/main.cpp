#include <iostream>
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
    double endTime = omp_get_wtime();
    std::cout << "Linear elapsed time is " << endTime - startTime << " seconds" << std::endl;
    return sum;
}

float sseSum_(const float* arr, const size_t& size){
    __m128 sum = _mm_setzero_ps();

    for (int i = 0; i < size; i += 4) {
        __m128 data = _mm_loadu_ps(&arr[i]);
        sum = _mm_add_ps(sum, data);
    }

    float result[4];
    _mm_storeu_ps(result, sum);

    float finalSum = result[0] + result[1] + result[2] + result[3];
    return finalSum;
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
    double endTime = omp_get_wtime();
    std::cout << "For elapsed time is " << endTime - startTime << " seconds" << std::endl;
    return sum;
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
    double endTime = omp_get_wtime();
    std::cout << "Sections elapsed time is " << endTime - startTime << " seconds" << std::endl;
    return sum;
}

float cascadeSum(const float* arr, const size_t& size){
    float startTime = omp_get_wtime();
    size_t n = size;
    std::vector<float> y(n, 1);
    while (n >= 2) {
        n /= 2;
        for (size_t i = 0; i < n; i++) {
            y[i] = y[2 * i] + y[2*i + 1];
            if (std::isnan(y[1])) {
                std::cout << "bebe" << std::endl;
            }
        }
    }
    float duration = omp_get_wtime() - startTime;
    std::cout << "Cascade time is: " << duration << " seconds" << std::endl;
    return y[0];
}

int main(){
    size_t size = 8192 * 4;
    float array[size];
    for (int i = 0; i < size; i++) {
        array[i] = 1;
    }
    float resLin = linearSum(array, size);
    float resSSE = sseSum_(array, size);
    float resSection = sectionsSum(array, size);
    float resFor = forSum(array, size);
    float resCascade = cascadeSum(array, size);
    return 0;
}