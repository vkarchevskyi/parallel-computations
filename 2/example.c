#include <stdio.h>

#define N 15.0
#define n 6.0
#define WIDTH 3.0

int main()
{
    double beta = n / N;
    double S = WIDTH / (WIDTH * beta + (1 - beta));
    double E = S / WIDTH;

    printf("Частка послідовних обчислень beta = %f\n", beta);
    printf("Максимальне можливе прискорення S(%.0f) = %f\n", WIDTH, S);
    printf("Ефективність системи E(%.0f) = %.2f%%\n", WIDTH, E * 100);
}