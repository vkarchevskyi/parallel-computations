#include <stdio.h>
#include <stdlib.h>
#include "/opt/homebrew/opt/libomp/include/omp.h"
#include <limits.h>
#include <time.h>

#define M 4  // кількість рядків (і потоків)
#define N 6  // кількість стовпців

int main() {
    int A[M][N];
    int min_values[M];
    omp_lock_t lock;
    FILE *file;

    // Ініціалізація генератора випадкових чисел
    srand(time(NULL));

    // Створення матриці з випадковими елементами
    printf("Матриця A (%dx%d):\n", M, N);
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = rand() % 100;
            printf("%4d ", A[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    // Ініціалізація замка
    omp_init_lock(&lock);

    // Відкриття файлу для запису
    file = fopen("results.txt", "w");
    if (file == NULL) {
        printf("Помилка відкриття файлу!\n");
        return 1;
    }

    // Встановлення кількості потоків
    omp_set_num_threads(M);

    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();

        // Виведення інформації одним потоком (директива single)
        #pragma omp single
        {
            printf("=== Інформація про лабораторну роботу ===\n");
            printf("Номер лабораторної роботи: 8\n");
            printf("Назва: Використання функцій блокування та синхронізації технології OpenMP\n");
            printf("Група студента: КН-22\n");
            printf("ПІБ студента: Карчевський Володимир\n");
            printf("Номер варіанта: 1\n");
            printf("Завдання: Знайти мінімальне значення кожного рядка матриці\n");
            printf("==========================================\n\n");

            fprintf(file, "=== Інформація про лабораторну роботу ===\n");
            fprintf(file, "Номер лабораторної роботи: 8\n");
            fprintf(file, "Назва: Використання функцій блокування та синхронізації технології OpenMP\n");
            fprintf(file, "Група студента: КН-22\n");
            fprintf(file, "ПІБ студента: Карчевський Володимир\n");
            fprintf(file, "Номер варіанта: 1\n");
            fprintf(file, "Завдання: Знайти мінімальне значення кожного рядка матриці\n");
            fprintf(file, "==========================================\n\n");
        }

        // Кожен потік обробляє свій рядок - знаходить мінімум
        int min_val = INT_MAX;
        for (int j = 0; j < N; j++) {
            if (A[thread_id][j] < min_val) {
                min_val = A[thread_id][j];
            }
        }
        min_values[thread_id] = min_val;

        // Блокування для виведення результатів у файл
        omp_set_lock(&lock);
        fprintf(file, "Потік %d: Мінімум рядка %d = %d\n", thread_id, thread_id, min_val);
        printf("Потік %d: Мінімум рядка %d = %d\n", thread_id, thread_id, min_val);
        omp_unset_lock(&lock);

        #pragma omp barrier

        // Частина 4: Почергове виведення з перевіркою блокування
        int entered = 0;
        while (!entered) {
            if (omp_test_lock(&lock)) {
                fprintf(file, "Потік %d: Початок закритої секції...\n", thread_id);
                printf("Потік %d: Початок закритої секції...\n", thread_id);

                // Імітація роботи в закритій секції
                for (volatile int k = 0; k < 100000; k++);

                fprintf(file, "Потік %d: Кінець закритої секції...\n", thread_id);
                printf("Потік %d: Кінець закритої секції...\n", thread_id);

                omp_unset_lock(&lock);
                entered = 1;
            } else {
                // Невдала спроба увійти до закритої секції
                // Використовуємо інший замок для запису повідомлення про невдалу спробу
                #pragma omp critical
                {
                    fprintf(file, "Потік %d: Невдала спроба увійти до закритої секції\n", thread_id);
                    printf("Потік %d: Невдала спроба увійти до закритої секції\n", thread_id);
                }
                // Невелика затримка перед повторною спробою
                for (int k = 0; k < 50000; k++);
            }
        }
    }

    // Знищення замка
    omp_destroy_lock(&lock);

    // Закриття файлу
    fclose(file);

    printf("\nРезультати записано у файл results.txt\n");

    return 0;
}
