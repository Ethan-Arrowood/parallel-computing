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

    double start, end, total;

    if (rank == 0)
    {
        for (int i = 1; i < ntasks; i++)
        {
            // time the ping pong operation from rank 0 node only
            start = MPI_Wtime();
            MPI_Send(bigboi, numInts, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Recv(bigboi, numInts, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            end = MPI_Wtime();

            total = end - start;

            printf("Total time by processors %d & %d to ping-pong array of %d ints: %f\n", rank, i, numInts, total);
        }
    }
    else
    {
        MPI_Recv(bigboi, numInts, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(bigboi, numInts, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}