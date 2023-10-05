#include <Windows.h>
#include <cstdlib>
#include <iostream>
#include <synchapi.h>
// #include <unistd.h>
// #include <io.h>


struct ThreadData {
    int cols;
    int* row;
    int product;
    int seed;
};

DWORD WINAPI threadProc(LPVOID lpParameter) {
    ThreadData* data = (ThreadData*)lpParameter;
    srand(data->seed); 
    int* row = data->row;
    int product = 1;
    int flag = -1;
    for (int i = 0; i < data->cols; i++)
    {
        int value = rand() % 10;
        row[i] = value;
        if (value % 2 == 1)
        {
            flag = 1; 
            product *= value;
        }
    }
    if (flag == -1) product = -1;//no vals
    data->product = product;
    return 0;
}

int main() {
    int rows = 6;
    int cols = 6;

    int matrix[rows][cols];
    HANDLE threads[rows];
    ThreadData data[rows];
    for (int i = 0; i < rows; i++) // set initial data for threads 
    {
        data[i].row = matrix[i];
        data[i].cols = cols;
        data[i].seed = rand();
    }
    std::cout << "initiating treads..." << std::endl;
    for (int i = 0; i < rows; i++) // create threads
    {

        threads[i] = CreateThread(NULL, 0, threadProc, &data[i], 0, NULL);
        if (threads[i] == NULL) {
            std::cout << "Couldn't create a thread " << i << std::endl;
            return 1;
        }
    }

    WaitForMultipleObjects(rows, threads, 1, 1e4);
    // CloseHandle(threads);
    for (int i = 0; i < rows; i++) {
        // WaitForSingleObject(threads[i], 1e3); // wait till threads[i] is done 
        std::cout << "Row " << i << ": ";
        for (int j = 0; j < cols; j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << "Product: " << data[i].product << std::endl;
    }

    return 0;
}