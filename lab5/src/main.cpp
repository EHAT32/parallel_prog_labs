#include <omp.h>
#include <iostream>

int main() {
    int num_threads = 13;
    omp_set_num_threads(num_threads);

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        std::cout << "Hello from thread " << tid << std::endl;
    }

    return 0;
}