#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

double dot(double *a, double *b, int n);

int main(int argc, char **argv)
{
    int i;
    double res, *a, *b;
    int n = 100;

    a = (double *)malloc(n * sizeof(*a));
    b = (double *)malloc(n * sizeof(*b));

    for (i = 0; i < n; i++)
    {
        a[i] = b[i] = 1.0 * i;
    }
    res = 0.0;

#pragma omp parallel shared(res) // find the dot product accross all ranks
    {
        res = dot(a, b, n);
    }

    printf("Result:  %f\n", res);
}

double dot(double *a, double *b, int n)
{
    int i, tid;
    double res = 0;
    tid = omp_get_thread_num();
#pragma omp parallel shared(res) // mark res as shared so it can be reduced
#pragma omp for reduction(+ \
                          : res) // reduce res variable over all ranks (summing it together)
    for (i = 0; i < n; i++)
    {
        res = res + (a[i] * b[i]);
        printf("  tid= %d i=%d\n", tid, i);
    }
    return res;
}
