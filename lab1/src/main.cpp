#include <emmintrin.h>
#include <cmath>
#include <iostream>
#include <vcruntime.h>

void sqrt_array(float* arr, float* result, int size) {
    int i = 0;
    for (; i < size - 3; i += 4) {
        __m128 x = _mm_loadu_ps(&arr[i]);
        x = _mm_sqrt_ps(x);
        _mm_storeu_ps(&result[i], x);
    }
    for (; i < size; i++) {
        result[i] = sqrt(arr[i]);
    }
}

int main(){
    int size = 2048;
    float arr[size], result[size];
    for (int i = 0; i < size; i++) {
        arr[i] = i;
    }
    sqrt_array(arr, result, size);
    return 0;
}