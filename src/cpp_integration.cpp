#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include "integration.h"

// Структура для передачи параметров в потоки
struct ThreadData
{
    double a;
    double b;
    int n;
    double (*func)(double);
    std::atomic<double> result;

    ThreadData(double _a, double _b, int _n, double (*_func)(double))
    {
        a = _a;
        b = _b;
        n = _n;
        func = _func;
    }
};

// Функция для интегрирования в потоке
void rectangle_integration(ThreadData *data)
{
    double width = (data->b - data->a) / data->n; // Ширина каждого прямоугольника
    double local_sum = 0.0;

    for (int i = 0; i < data->n; i++)
    {
        double x = data->a + i * width;     // Левый край прямоугольника
        local_sum += data->func(x) * width; // Площадь прямоугольника
    }
    data->result = local_sum;
}

IntegrationResult rectangle_integration_cpp(double (*func)(double), double a, double b, int n, Config config)
{
    const int num_threads = config.threadsNum; // Получаем количество доступных потоков
    std::vector<std::thread> threads(num_threads);
    std::vector<ThreadData *> threadsData(num_threads);
    std::vector<int> rectangles_per_thread(num_threads);

    // Создание и запуск потоков
    for (int i = 0; i < num_threads; i++)
    {
        int rectangles_per_thread = n / num_threads;
        if ((n % num_threads) - i > 0)
        {
            rectangles_per_thread++;
        }
        ThreadData *data = new ThreadData{
            a + i * rectangles_per_thread * (b - a) / n,
            a + (i + 1) * rectangles_per_thread * (b - a) / n,
            rectangles_per_thread,
            func};
        threadsData[i] = data;
        threads[i] = std::thread(rectangle_integration, data);
    }

    // Ожидание завершения всех потоков
    for (auto &thread : threads)
    {
        thread.join();
    }

    // // Суммирование результатов от всех потоков
    double total_result = 0.0;
    for (const ThreadData *res : threadsData)
    {
        total_result += res->result;
    }

    return {total_result};
}