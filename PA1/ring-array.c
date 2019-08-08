#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int ntasks, rank, len;
    char hostname[MPI_MAX_PROCESSOR_NAME];

    // initialize MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int bytes = 1073741824;
    int numInts = bytes / sizeof(int);
    int *bigboi = malloc(bytes); // 1 gigabyte array of ints

    // both nodes will be timed
    double start, end, total;

    if (rank == 0)
    {
        start = MPI_Wtime();
        MPI_Send(bigboi, numInts, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(bigboi, numInts, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        end = MPI_Wtime();
    }
    else if (rank == 1)
    {
        start = MPI_Wtime();
        MPI_Recv(bigboi, numInts, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(bigboi, numInts, MPI_INT, 0, 0, MPI_COMM_WORLD);
        end = MPI_Wtime();
    }
    total = end - start;

    printf("Total time by processor %d to send and receive the 1gb array payload of %d ints: %f\n", rank, numInts, total);

    MPI_Finalize();

    return 0;
}