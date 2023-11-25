#include <iostream>
#include <vector>
#include <cassert>
#include <xmmintrin.h> 
#include <omp.h>

float linearSum(float* array, const int& size){
    double startTime = omp_get_wtime();
    float sum = 0;
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
    int sum = 0;

    __m128i result = _mm_setzero_si128();

    for (int i = 0; i < size; i += 4) {
        __m128i chunk = _mm_loadu_si128((__m128i*)(arr + i));
        result = _mm_add_epi32(result, chunk);
    }

    int temp[4];
    _mm_storeu_si128((__m128i*)temp, result);

    sum = temp[0] + temp[1] + temp[2] + temp[3];
    return sum;
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

float forSum(float* array, const int& size){
    double startTime = omp_get_wtime();
    float sum = 0;
    #pragma omp parallel for
    {
        for (int i = 0; i < size; i++) {
            sum += array[i];
        }
    }
    double endTime = omp_get_wtime();
    std::cout << "For elapsed time is " << endTime - startTime << " seconds" << std::endl;
    return sum;
}

int main(){
    int size = 64;
    float array[size];
    for (int i = 0; i < size; i++) {
        array[i] = i + 1;
    }
    int resLin = linearSum(array, size);
    int resFor = forSum(array, size);
    int resSSE = newsseSum(array, size);
    return 0;
}