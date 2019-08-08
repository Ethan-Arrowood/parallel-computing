// #include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

double f(double x)
{
    return x * x;
}

int main(int argc, char *argv[])
{
    int N = 10000000;
    double a = 1, b = 2, area, sum = 0;

    for (int i = 1; i <= N; i++)
    {
        sum += f(a + (i - 1) * ((b - a) / N));
    }

    area = ((b - a) / N) * sum;

    printf("f(x) = x^2, a = %f, b = %f, N = %d, area = %f\n", a, b, N, area);

    return 0;
}