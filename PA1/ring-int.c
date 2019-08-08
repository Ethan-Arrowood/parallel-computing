#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int ntasks, rank, len;
    char hostname[MPI_MAX_PROCESSOR_NAME];

    // initialize mpi
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int iloveyou = 3000; // Avengers!

    // time both nodes
    double start, end, total;

    if (rank == 0)
    {
        start = MPI_Wtime();
        MPI_Send(&iloveyou, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&iloveyou, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        end = MPI_Wtime();
    }
    else if (rank == 1)
    {
        start = MPI_Wtime();
        MPI_Recv(&iloveyou, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(&iloveyou, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        end = MPI_Wtime();
    }
    total = end - start;

    printf("Total time by processor %d to send and receive the integer payload: %f\n", rank, total);

    MPI_Finalize();

    return 0;
}