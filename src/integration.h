struct IntegrationResult
{
    double result;
};

struct Config
{
    int threadsNum;
};

struct IntegrationResult rectangle_integration_simple(double (*func)(double), double a, double b, int n);
struct IntegrationResult rectangle_integration_cpp(double (*func)(double), double a, double b, int n, Config config);
struct IntegrationResult rectangle_integration_pthread(double (*func)(double), double a, double b, int n, Config config);
struct IntegrationResult rectangle_integration_openmp(double (*func)(double), double a, double b, int n, Config config);