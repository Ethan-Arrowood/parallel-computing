#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#define SIZE 4

int main(int argc, char *argv[])
{
    int ntasks, rank, sendcount, recvcount, source;
    double startS, endS, tot_timeS;
    float sendbuf[SIZE][SIZE] = {
        {1.0, 2.0, 3.0, 4.0},
        {5.0, 6.0, 7.0, 8.0},
        {9.0, 10.0, 11.0, 12.0},
        {13.0, 14.0, 15.0, 16.0}};
    float recvbuf[SIZE];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);

    startS = MPI_Wtime();
    if (ntasks == SIZE)
    {
        source = 1;
        sendcount = SIZE;
        recvcount = SIZE;

        MPI_Scatter(sendbuf, sendcount, MPI_FLOAT, recvbuf, recvcount, MPI_FLOAT, source, MPI_COMM_WORLD);

        printf("rank = %d  Results: %f %f %f %f\n", rank, recvbuf[0], recvbuf[1], recvbuf[2], recvbuf[3]);
    }
    else
    {
        printf("Must specify %d processors. Terminating.\n", SIZE);
    }
    endS = MPI_Wtime();
    tot_timeS = endS - startS;
    printf("Total time to scatter all %f\n", tot_timeS);

    MPI_Finalize();
    return 0;
}