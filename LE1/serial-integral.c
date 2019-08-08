#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double func(double x);

int main(int argc, char **argv)
{
    int N;
    double a, b;

    N = 100;
    a = 0;
    b = 3;

    double width = (b - a) / N;
    double result = 0;

    //loop to determine the sum of the heights
    for (int i = 1; i <= N; i++)
    {
        result += func(a + (i - 1) * width);
    }

    //multiply by the width
    result = result * width;
    printf("The numerical approximation is: %.8f\n", result);

    return 0;
}

double func(double x)
{
    return 3. * exp(-x) * sin(x) * sin(x) + 1.0;
}
