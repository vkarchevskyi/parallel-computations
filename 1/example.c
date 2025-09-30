#include <stdio.h>
#include <stdarg.h>

#define DEVICES_IN_FIRST_SYSTEM 5
#define DEVICES_IN_SECOND_SYSTEM 4
#define DEVICES_IN_THIRD_SYSTEM 4
#define DEVICES_SUM DEVICES_IN_FIRST_SYSTEM + DEVICES_IN_SECOND_SYSTEM + DEVICES_IN_THIRD_SYSTEM

int min(int, ...);
int getSumPi(int pi[], int n);
int getMaxPi(int pi[], int n);

int main()
{
    int pi[] = {10, 5, 8, 6, 7, 9, 12, 8, 10, 4, 6, 4, 6};
    int pi1 = min(DEVICES_IN_FIRST_SYSTEM, pi[0], pi[1], pi[2], pi[3], pi[4]);
    int pi2 = min(DEVICES_IN_SECOND_SYSTEM, pi[5], pi[6], pi[7], pi[8]);
    int pi3 = min(DEVICES_IN_THIRD_SYSTEM, pi[9], pi[10], pi[11], pi[12]);

    printf("Реальна продуктивність, з якою працюють усі пристрої 1 системи pi(1) = %d\n", pi1);
    printf("Реальна продуктивність, з якою працюють усі пристрої 2 системи pi(2) = %d\n", pi2);
    printf("Реальна продуктивність, з якою працюють усі пристрої 3 системи pi(3) = %d\n", pi3);

    double p[] = {
        (double)pi1 / pi[0],
        (double)pi1 / pi[1],
        (double)pi1 / pi[2],
        (double)pi1 / pi[3],
        (double)pi1 / pi[4],

        (double)pi2 / pi[5],
        (double)pi2 / pi[6],
        (double)pi2 / pi[7],
        (double)pi2 / pi[8],

        (double)pi3 / pi[9],
        (double)pi3 / pi[10],
        (double)pi3 / pi[11],
        (double)pi3 / pi[12],
    };

    for (int i = 0; i < DEVICES_SUM; i++)
    {
        printf("Завантаженість пристрою %2d p(%d) = %f\n", i, i, p[i]);
    }

    int r1 = DEVICES_IN_FIRST_SYSTEM * pi1;
    int r2 = DEVICES_IN_SECOND_SYSTEM * pi2;
    int r3 = DEVICES_IN_THIRD_SYSTEM * pi3;
    int r = r1 + r2 + r3;

    printf("Реальна продуктивність підсистеми 1 r1 = %d\n", r1);
    printf("Реальна продуктивність підсистеми 2 r2 = %d\n", r2);
    printf("Реальна продуктивність підсистеми 3 r3 = %d\n", r3);
    printf("Реальна продуктивність системи r = %d\n", r);

    int sumPi = getSumPi(pi, DEVICES_SUM);
    printf("Пікова продуктивність системи pi = %d\n", sumPi);

    double load = (double)r / sumPi;
    printf("Завантаженість системи = %f\n", load);

    int maxPi = getMaxPi(pi, DEVICES_SUM);
    printf("Максимальна реальна продуктивність пристрою = %d\n", maxPi);

    double S = (double)r / maxPi;
    printf("Прискорення системи S = %f\n", S);
}

int min(int n, ...)
{
    va_list args;
    va_start(args, n);

    int temp;
    int min = __INT_MAX__;

    for (int i = 0; i < n; i++)
    {
        temp = va_arg(args, int);
        if (temp < min)
        {
            min = temp;
        }
    }

    va_end(args);

    return min;
}

int getMaxPi(int pi[], int n)
{
    int maxPi = -1;
    for (int i = 0; i < n; i++)
    {
        if (pi[i] > maxPi)
        {
            maxPi = pi[i];
        }
    }

    return maxPi;
}

int getSumPi(int pi[], int n)
{
    int sumPi = 0;
    for (int i = 0; i < DEVICES_SUM; i++)
    {
        sumPi += pi[i];
    }

    return sumPi;
}
