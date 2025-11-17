#include <stdio.h>
#include "/opt/homebrew/opt/libomp/include/omp.h"
#include <iostream>
#include <fstream>

using namespace std;

double norm_frobenius(double **matrix, int m, int n)
{
    double norm = 0;
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            norm += matrix[i][j] * matrix[i][j];
        }
    }
    return sqrt(norm);
}

double norm_frobenius_parallel(double **matrix, int m, int n, int thread_count)
{
    double norm = 0;
    // Semerenko p. 56-57
#pragma omp parallel for num_threads(thread_count) reduction(+ : norm)
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            norm += matrix[i][j] * matrix[i][j];
        }
    }
#pragma omp critical

    return sqrt(norm);
}

int main()
{
    const int DIM_COUNT = 7;
    int dims[DIM_COUNT] = {100, 500, 1000, 2000, 3000, 5000, 10000};
    const int THREADS_COUNT = 6;

    int threads[THREADS_COUNT] = {2, 4, 6, 8, 10, 20};
    double **matrix = new double *[dims[DIM_COUNT - 1]];
    cout << "Initialization...\n";
    for (int i = 0; i < dims[DIM_COUNT - 1]; i++)
    {
        matrix[i] = new double[dims[DIM_COUNT - 1]];
        for (size_t j = 0; j < dims[DIM_COUNT - 1]; j++)
        {
            matrix[i][j] = 2 * (double)rand() / RAND_MAX - 1;
        }
    }

    fstream fs = fstream("result-frobeni.txt", ios::out);
    for (int d = 0; d < DIM_COUNT; d++)
    {
        int m = dims[d], n = dims[d];

        double start = omp_get_wtime();
        double norm = norm_frobenius(matrix, m, n);

        double duration = omp_get_wtime() - start;
        cout << "Dimension: " << m << '\n';
        printf("\tIn serial mode ||A||_F = %f; duration is %f\n ", norm, duration);
        for (int t = 0; t < THREADS_COUNT; t++)
        {
            start = omp_get_wtime();
            norm = norm_frobenius_parallel(matrix, m, n, threads[t]);
            double speedup = duration / (omp_get_wtime() - start);
            printf("\tFor %d threads ||A||_F = %f; speedup is %f\n ", threads[t], norm, speedup);
            fs << speedup << '\t';
        }
        fs << '\n';
    }
    fs.close();
    for (size_t i = 0; i < dims[DIM_COUNT - 1]; i++)
        delete[] matrix[i];
    delete[] matrix;
}