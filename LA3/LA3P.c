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

    int subCount = 0;

    // int i;
    // for (i = (rank * (n / ntasks)); i <= ((rank + 1) * (n / ntasks)); i++)
    // {
    //     if (is_prime(i) == 1)
    //         subCount++;
    // }
    // 0 -> 1, 9, 17, 25
    // 1 -> 3, 11, 19, 27
    // 2 -> 5, 13, 21, 29
    // 3 -> 7, 15, 23, 31
    int start[4] = {1, 3, 5, 7};
    for (int i = start[rank]; i <= n; i += (ntasks * 2))
    {
        if (is_prime(i) == 1)
            subCount++;
    }

    int numprimes = 1; // account for value 2
    MPI_Reduce(&subCount, &numprimes, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0)
    {
        printf("Number of Primes: %d\n", numprimes);
    }
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
