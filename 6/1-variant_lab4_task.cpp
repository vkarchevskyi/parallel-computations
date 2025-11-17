#include <stdio.h>
#include "/opt/homebrew/opt/libomp/include/omp.h"
#include <iostream>
#include <fstream>

using namespace std;

double dot_serial(double *a, double *b, int size)
{
    double sum = 0;
    for (int i = 0; i < size; i++)
    {
        sum += a[i] * b[i];
    }
    return sum;
}

double dot_parallel(double *a, double *b, int size, int thread_count)
{
    double sum = 0;

#pragma omp parallel for num_threads(thread_count) reduction(+ : sum)
    for (int i = 0; i < size; i++)
    {
        sum += a[i] * b[i];
    }

#pragma omp critical

    return sum;
}

int main()
{
    const int THREADS_COUNT = 6;
    const int MAX_LENGTH = 100000;

    const int DIM_COUNT = 7;
    int dims[DIM_COUNT] = {100, 500, 1000, 2000, 3000, 5000, MAX_LENGTH};
    int threads[THREADS_COUNT] = {2, 4, 6, 8, 10, 20};
    double a[MAX_LENGTH];
    double b[MAX_LENGTH];
    cout << "Initialization...\n";
    // Ініціалізація векторів
    for (int i = 0; i < MAX_LENGTH; i++)
    {
        a[i] = 2 * (double)rand() / RAND_MAX + 1;
        b[i] = 2 * (double)rand() / RAND_MAX + 1;
    }

    fstream fs = fstream("result-lab4.txt", ios::out);
    for (int s = 0; s < DIM_COUNT; s++)
    {
        double start = omp_get_wtime();
        double dotProduct = dot_serial(a, b, dims[s]);

        double duration = omp_get_wtime() - start;
        cout << "Size: " << dims[s] << '\n';
        printf("\tIn serial mode a * b = %f; duration is %f\n ", dotProduct, duration);
        for (int t = 0; t < THREADS_COUNT; t++)
        {
            start = omp_get_wtime();
            dotProduct = dot_parallel(a, b, dims[s], threads[t]);
            double speedup = duration / (omp_get_wtime() - start);
            printf("\tFor %d threads a * b = %f; speedup is %f\n ", threads[t], dotProduct, speedup);
            fs << speedup << '\t';
        }
        fs << '\n';
    }
    fs.close();
}