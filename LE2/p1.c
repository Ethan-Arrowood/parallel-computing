#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int numthreads, tid, i, j;
    int n = 1000;
    double **a; // initialize a only

// mark a as private allowing every thread to have its own copy of the variable to manipulate
#pragma omp parallel shared(numthreads) private(i, j, tid, a)
    {

        tid = omp_get_thread_num();
        if (tid == 0)
        {
            numthreads = omp_get_num_threads();
            printf("Number of threads = %d\n", numthreads);
        }

#pragma omp barrier

        // allocate a array
        a = (double **)malloc(n * sizeof(double));
        for (int i = 0; i < n; i++)
        {
            a[i] = (double *)malloc(n * sizeof(double));
        }

        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                a[i][j] = tid + i + j; // find values
            }
        }
        printf("ID: %d Last element= %f\n", tid, a[n - 1][n - 1]); // print result
    }
}
