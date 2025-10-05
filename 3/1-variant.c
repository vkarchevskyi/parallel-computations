#include <stdio.h>
#include <math.h>

#define QUANTITY_OF_PARALLEL_COMPUTATIONS 1.0 / 4.0
#define l 6.0
#define MIN_PERCENT 0.6
#define MAX_PERCENT 0.95

int main()
{
    double beta = 1 - QUANTITY_OF_PARALLEL_COMPUTATIONS;
    double S = l / (l * beta + 1 - beta);
    double min_l = 1.0; // Should be calculated manually

    printf("Частка послідовних обчислень beta = %f\n", beta);
    printf("Максимальне можливе прискорення S(%.0f) = %f\n", l, S);
    printf(
        "Мінімальна кількість процесорів, використання який може забезпечити від %.0f%% до %.0f%% графічного прискорення %.0f\n",
        MIN_PERCENT * 100,
        MAX_PERCENT * 100,
        min_l);
}