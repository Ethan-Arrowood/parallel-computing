#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    double startS, endS, tot_timeS;
    double startR, endR, tot_timeR;
    startS, startR, endS, endR = 0;

    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int numranks;
    MPI_Comm_size(MPI_COMM_WORLD, &numranks);

    int n;
    if (rank == 0)
    {
        n = -1;
        startS = MPI_Wtime();
        MPI_Send(&n, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        endS = MPI_Wtime();

        tot_timeS = endS - startS;
        printf("Total time to send %f\n", tot_timeS);
    }
    else if (rank == 1)
    {
        startR = MPI_Wtime();
        MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        endR = MPI_Wtime();

        printf("Process 1 received n %d from proess 0\n", n);
        tot_timeR = endR - startR;
        printf("Total time to receive %f\n", tot_timeR);
    }
    MPI_Finalize();
    return 0;
}