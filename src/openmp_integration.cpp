#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <time.h>
#include "integration.h"

// Метод прямоугольников для численного интегрирования с использованием OpenMP
double rectangle_integration(double (*func)(double), double a, double b, int n)
{
    double width = (b - a) / n; // Ширина каждого прямоугольника
    double sum = 0.0;

// Параллельный блок OpenMP
#pragma omp parallel
    {
        double local_sum = 0.0;

// Каждый поток вычисляет свою часть суммы
#pragma omp for
        for (int i = 0; i < n; i++)
        {
            double x = a + i * width;     // Левый край прямоугольника
            local_sum += func(x) * width; // Площадь прямоугольника
        }

// Суммируем результаты от всех потоков
#pragma omp atomic
        sum += local_sum;
    }

    return sum;
}

IntegrationResult rectangle_integration_openmp(double (*func)(double), double a, double b, int n, Config config)
{

    double result = rectangle_integration(func, a, b, n);
    return {result};
}