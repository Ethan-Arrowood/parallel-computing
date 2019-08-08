
#include "LA3.c.opari.inc"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int is_prime(int n);

int main(int argc, char **argv)
{

    int ntasks, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n = 10000000;
    int numprimes = 0;
    int i;
    for (i = 1; i <= n; i++)
    {
        if (is_prime(i) == 1)
            numprimes++;
    }

    printf("Number of Primes: %d\n", numprimes);

    MPI_Finalize();
}

int is_prime(int n)
{
    /* handle special cases */
    if (n == 0)
        return 0;
    else if (n == 1)
        return 0;
    else if (n == 2)
        return 1;

    int i;
    for (i = 2; i <= (int)(sqrt((double)n)); i++)
        if (n % i == 0)
            return 0;

    return 1;
}
