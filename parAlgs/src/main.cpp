#include <iostream>
#include <vector>
#include <cassert>
#include <xmmintrin.h> 
#include <omp.h>

float linearSum(float* array, const int& size){
    double startTime = omp_get_wtime();
    double sum = 0;
    for (int i = 0; i < size; i++) {
        sum += array[i];
    }
    double endTime = omp_get_wtime();
    std::cout << "Linear elapsed time is " << endTime - startTime << " seconds" << std::endl;
    return sum;
}

float sseSum(float* arr, const int& size) {
    __m128i sum = _mm_setzero_si128();
    for (int i = 0; i < size; i += 4) {
        __m128i v = _mm_loadu_si128((__m128i*)(arr + i));
        sum = _mm_add_epi32(sum, v);
    }
    int result = 0;
    result += sum[0] + sum[1] + sum[2] + sum[3];
    return result;
}

float sseSum_(float* arr, const int& size){
    int n = sizeof(arr) / sizeof(arr[0]);
    __m128i sum = _mm_setzero_si128();

    for (int i = 0; i < sizeof(arr); i += 4) {
        __m128i data = _mm_loadu_ps(&arr[i]);
        sum = _mm_add_epi32(sum, data);
    }

    float result[4];
    _mm_storeu_si128((__m128i*)result, sum);

    float finalSum = result[0] + result[1] + result[2] + result[3];
    return finalSum;
}

float newsseSum(float* arr, const int& size){
    double startTime = omp_get_wtime();
    float sum = 0;
    int index = 0;

    int parts = size / sizeof(float);
    __m128* xmm_arr = (__m128*)arr;

    while (index < parts)
    {
        __m128 xmm_sum = _mm_setzero_ps();
        xmm_sum = _mm_add_ps(xmm_sum, xmm_arr[index]);
        _mm_store_ss(&sum, xmm_sum);
        index++;
    }
    double endTime = omp_get_wtime();
    std::cout << "SSE elapsed time is " << endTime - startTime << " seconds" << std::endl;
    return sum;
}

float forSum(float* arr, const int& size){
    double sum = 0;
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

float sectionsSum(float* arr, const int& size){
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
int main(){
    int size = 8192 * 4;
    float array[size];
    for (int i = 0; i < size; i++) {
        array[i] = 1;
    }
    double resLin = linearSum(array, size);
    double resSSE = sseSum_(array, size);
    double resFor = forSum(array, size);
    double resSection = sectionsSum(array, size);
    return 0;
}