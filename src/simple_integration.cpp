#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "integration.h"

// Метод прямоугольников для численного интегрирования
IntegrationResult rectangle_integration_simple(double (*func)(double), double a, double b, int n)
{
    double width = (b - a) / n; // Ширина каждого прямоугольника
    double sum = 0.0;

    for (int i = 0; i < n; i++)
    {
        double x = a + i * width; // Левый край прямоугольника
        sum += func(x) * width;   // Площадь прямоугольника
    }

    return {sum};
}
