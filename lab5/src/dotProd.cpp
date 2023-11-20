#include <omp.h>
#include <vector>
#include <iostream>

std::vector<int> dotProductOMP(const int& vecLen, const int& vectorsNum){
    std::vector<int> result;
    int vectors[vectorsNum][vecLen];
    double startTime = omp_get_wtime();
    #pragma omp parallel shared(vectors)
    {
        #pragma omp sections
        {
            //load vectors
            #pragma omp section
            {

                for (int i = 0; i < vectorsNum; i ++) {
                    for (int j = 0; j < vecLen; j ++) {
                        vectors[i][j] = i + j;
                    }
                
                }
            }
            #pragma omp section
            {
                //calculate dot product for each pair
                for (int i = 0; i < vectorsNum; i++) {
                    for (int j = 0; j < vectorsNum; j++) {
                        int prod = 0;
                        for (int c = 0; c < vecLen; c++) {
                            prod += vectors[i][c] * vectors[j][c];
                        }
                        result.push_back(prod);
                    }
                }
            }
        }
    }
    double duration = omp_get_wtime() - startTime;
    std::cout << "OMP elapsed time is " << duration << " seconds" << std::endl;
    return result;
}

std::vector<int> dotProduct(const int& vecLen, const int& vectorsNum){
    std::vector<int> result;
    int vectors[vectorsNum][vecLen];
    double startTime = omp_get_wtime();
    //load vectors
    for (int i = 0; i < vectorsNum; i ++) {
        for (int j = 0; j < vecLen; j ++) {
            vectors[i][j] = i + j;
        }
    
    }
    //calculate dot product for each pair
    for (int i = 0; i < vectorsNum; i++) {
        for (int j = 0; j < vectorsNum; j++) {
            int prod = 0;
            for (int c = 0; c < vecLen; c++) {
                prod += vectors[i][c] * vectors[j][c];
            }
            result.push_back(prod);
        }
    }
    double duration = omp_get_wtime() - startTime;
    std::cout << "standard elapsed time is " << duration << " seconds" << std::endl;
    return result;
}

int main(){
    auto resultOMP = dotProductOMP(300, 3);
    auto result = dotProduct(300, 10);
    for (int i = 0; i < resultOMP.size(); i++) {
        std::cout << resultOMP[i] << " " << result[i] << std::endl;
    }
    return 0;
}