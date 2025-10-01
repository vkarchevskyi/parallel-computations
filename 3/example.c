#include <stdio.h>
#include <math.h>

#define QUANTITY_OF_PARALLEL_COMPUTATIONS 5.0 / 6.0
#define l 5.0
#define PERCENT 0.85

int main()
{
    double beta = 1 - QUANTITY_OF_PARALLEL_COMPUTATIONS;
    double S = l / (l * beta + 1 - beta);
    /*
        S(i) >= 0.8 * S(max)
        l / (l / 6 + 5 / 6) >= 1 / (1 / 6)
        6l / (l + 5) >= 0.85 * 6
        l >= 0.85 * (l + 5)
        0.5l >= 4.25
        l >= 85 / 3 == 28.333
    */
    double min_l = ceil(PERCENT * 100 / 3);

    printf("Частка послідовних обчислень beta = %f\n", beta);
    printf("Максимальне можливе прискорення S(%.0f) = %f\n", l, S);
    printf("Мінімальна кількість процесорів, використання який може забезпечити %.0f%% графічного прискорення %.0f\n", PERCENT * 100, min_l);
}