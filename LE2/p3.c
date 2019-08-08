#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{

    int nthreads, i, tid;
    double total;

#pragma omp parallel private(tid, total, i) // make variables private so script can execute without conflict
    {
        tid = omp_get_thread_num();
        if (tid == 0)
        {
            nthreads = omp_get_num_threads();
            printf("Number of threads: %d\n", nthreads);
        }

#pragma omp barrier

        total = 0;
        for (i = 0; i < 10; i++)
        {
            total = total + i;
        }

        printf("Thread %d is done! Total= %f\n", tid, total);
    }
}
