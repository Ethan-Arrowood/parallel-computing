#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char **argv)
{
    int ntasks, rank, N = 50000000;
    double start, end, total;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    start = MPI_Wtime();

    int *a = (int *)malloc(N * sizeof(int));
    int *b = (int *)malloc(N * sizeof(int));

    if (rank == 0)
    {
        for (int i = 0; i < N; i++)
        {
            a[i] = b[i] = 2;
        }
    }

    int stride = N / ntasks;
    int *sendcounts = (int *)malloc(ntasks * sizeof(int));
    int *displs = (int *)malloc(ntasks * sizeof(int));

    for (int i = 0; i < ntasks; i++)
    {
        displs[i] = i * stride;
        sendcounts[i] = stride;
    }

    if (N % ntasks != 0)
    {
        sendcounts[ntasks - 1] += N % ntasks;
    }

    int res = 0, count = sendcounts[rank];

    int *ra = (int *)malloc(count * sizeof(int));
    int *rb = (int *)malloc(count * sizeof(int));

    MPI_Scatterv(a, sendcounts, displs, MPI_INT, ra, count, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatterv(b, sendcounts, displs, MPI_INT, rb, count, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < count; i++)
    {
        res += (ra[i] * rb[i]);
    }

    // printf("Rank: %d | Result: %d\n", rank, res);

    int result;
    MPI_Reduce(&res, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    end = MPI_Wtime();
    total = end - start;
    if (rank == 0)
    {
        printf("Result: %d | Time: %f | Ranks: %d\n", result, total, ntasks);
    }

    MPI_Finalize();
}