#include <Windows.h>
#include <cstdlib>
#include <handleapi.h>
#include <iostream>
#include <synchapi.h>

struct ThreadData {
    int* row;
    int col;
    int product;
    int seed;
};

DWORD WINAPI threadProc(LPVOID lpParameter) {
    ThreadData* data = (ThreadData*)lpParameter;
    srand(data->seed); 
    int* row = data->row;
    int product = 1;
    int flag = -1;
    for (int i = 0; i < data->col; i++)
    {
        int value = rand() % 100;
        row[i] = value;
        if (value % 2 == 1)
        {
            flag = 1; 
            product *= value;
        }
    }
    if (flag == -1) product = -1; //no vals
    data->product = product;
    return 0;
}

int main() {
    int row = 5; //max is 6
    int col = 6;

    int matrix[row][col];
    HANDLE threads[row];
    ThreadData data[row];
    for (int i = 0; i < row; i++) // set initial data
    {
        data[i].row = matrix[i];
        data[i].col = col;
        data[i].seed = rand();
    }
    std::cout << "initiating treads..." << std::endl;
    for (int i = 0; i < row; i++) // create threads
    {

        threads[i] = CreateThread(NULL, 0, threadProc, &data[i], 0, NULL);
        if (threads[i] == NULL) {
            std::cout << "couldn't create a thread " << i << std::endl;
            return 1;
        }
    }

    WaitForMultipleObjects(row, threads, 1, 1e4);
    for (int i = 0; i < row; i++) {
        CloseHandle(threads[i]);
    }
    // CloseHandle(threads);
    for (int i = 0; i < row; i++) {
        WaitForSingleObject(threads[i], 1e3);
        std::cout << "row " << i << ": ";
        for (int j = 0; j < col; j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << "--- " << "product: " << data[i].product << std::endl;
    }

    return 0;
}