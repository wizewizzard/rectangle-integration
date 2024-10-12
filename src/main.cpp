#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "integration.h"
#include <chrono>

// Функция, которую мы хотим интегрировать
double f(double x)
{
    return x * x * x * x - x * x - x + 5; // Пример: f(x) = x^2
}

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        printf("Использование: %s <a> <b> <n> <n>\n", argv[0]);
        return 1;
    }

    // Преобразование аргументов командной строки в числа
    double a = atof(argv[1]);       // Нижний предел интегрирования
    double b = atof(argv[2]);       // Верхний предел интегрирования
    int n = atoi(argv[3]);          // Количество прямоугольников
    int threadsNum = atoi(argv[4]); // Количество потоков

    printf("Запуск для параметров:\n\tНижний предел интегрирования=%f\n\tВерхний предел интегрирования=%f\n\tКоличество прямоугольников=%d\n\tКоличество потоков=%d\n", a, b, n, threadsNum);
    double time_taken;
    IntegrationResult res;

    printf("Запуск однопоточного алгоритма...\n");
    auto start = std::chrono::steady_clock::now();
    res = rectangle_integration_simple(f, a, b, n);
    auto end = std::chrono::steady_clock::now();
    printf("Однопоточный алгоритм закончил работу. Результат=%f. Время=%f\n", res.result, std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 10e5);

    printf("Запуск алгоритма на openmp потоках...\n");
    start = std::chrono::steady_clock::now();
    res = rectangle_integration_openmp(f, a, b, n, {threadsNum});
    end = std::chrono::steady_clock::now();
    printf("Алгоритм на openmp потоках закончил работу. Результат=%f. Время=%f\n", res.result, std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 10e5);

    printf("Запуск алгоритма на cpp потоках...\n");
    start = std::chrono::steady_clock::now();
    res = rectangle_integration_cpp(f, a, b, n, {threadsNum});
    end = std::chrono::steady_clock::now();
    printf("Алгоритм на cpp потоках закончил работу. Результат=%f. Время=%f\n", res.result, std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 10e5);

    printf("Запуск алгоритма на pthread потоках...\n");
    start = std::chrono::steady_clock::now();
    res = rectangle_integration_pthread(f, a, b, n, {threadsNum});
    end = std::chrono::steady_clock::now();
    printf("Алгоритм на pthread потоках закончил работу. Результат=%f. Время=%f\n", res.result, std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 10e5);
}
