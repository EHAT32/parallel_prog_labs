#include <immintrin.h>
#include <iostream>

float inner(float* arr1, float* arr2, int size)
{
    int i = 0;
    float res_sse[4];
    float res = 0;
    __m128 sum = _mm_setzero_ps();
    for(; i < size - 3; i += 4)
    {
        __m128 x = _mm_load_ps(&arr1[i]);
        __m128 y = _mm_load_ps(&arr2[i]);
        __m128 mul = _mm_mul_ps(x, y);
        sum = _mm_add_ps(sum, mul);

    }
    for (; i < size; i++)
    {
        res+=arr1[i]*arr2[i];
    }
    _mm_store_ps(res_sse, sum);
    res+=res_sse[0] + res_sse[1] + res_sse[2] + res_sse[3];
return res;
}

int main(){
    int size = 5;
    float a[size], b[size];
    for (int i = 0; i < size; i++) {
        a[i] = i * i;
        b[i] = i;
    }
    float inn = inner(a, b, size);
    std::cout << inn << std::endl;
    return 0;
}
//0, 1, 4, 9, 16
//0, 1, 2, 3, 4
//0+1+8+27+64