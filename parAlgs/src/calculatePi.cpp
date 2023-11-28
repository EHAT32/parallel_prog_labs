#include <iostream>

template<typename T>
float rectInt(const T& function, const float& a = 0, const float& b = 1, const int& N = 100000){
    float res = 0;
    float x = a;
    float h = (b - a) / N;
    for (int i = 0; i < N - 1; i++) {
        res += (function(x + i * h) + function(x + (i+1) * h)) / 2 * h;
    }
    return res;
}

float forPi(const float& x){
    return 4 / (1 + x * x);
}

int main(){
    std::cout << rectInt(forPi) << std::endl;
    return 0;
}