#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>
#include "integration.h"

// Структура для передачи параметров в поток
typedef struct
{
    double a;
    double b;
    int n;
    double (*func)(double);
    double result;
} ThreadData;

// Функция для интегрирования в потоке
void *rectangle_integration(void *arg)
{
    ThreadData *data = (ThreadData *)arg;
    double width = (data->b - data->a) / data->n; // Ширина каждого прямоугольника
    double sum = 0.0;

    for (int i = 0; i < data->n; i++)
    {
        double x = data->a + i * width; // Левый край прямоугольника
        sum += data->func(x) * width;   // Площадь прямоугольника
    }

    data->result = sum; // Сохранение результата в структуре
    return NULL;
}

IntegrationResult rectangle_integration_pthread(double (*func)(double), double a, double b, int n, Config config)
{
    pthread_t threads[config.threadsNum];
    ThreadData thread_data[config.threadsNum];
    int num_threads = config.threadsNum;
    // Разделение работы между потоками

    for (int i = 0; i < num_threads; i++)
    {
        int rectangles_per_thread = n / num_threads;
        if ((n % num_threads) - i > 0)
        {
            rectangles_per_thread++;
        }
        thread_data[i] = {a + i * rectangles_per_thread * (b - a) / n,
                          a + (i + 1) * rectangles_per_thread * (b - a) / n,
                          rectangles_per_thread,
                          func,
                          0};

        pthread_create(&threads[i], NULL, rectangle_integration, &thread_data[i]);
    }

    // Ожидание завершения всех потоков и суммирование результатов
    double total_result = 0.0;

    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
        total_result += thread_data[i].result;
    }

    return {total_result};
}