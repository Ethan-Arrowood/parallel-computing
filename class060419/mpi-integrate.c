#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

double f(double x)
{
    return x * x;
}

int main(int argc, char *argv[])
{
    int N, rank, nranks;
    double a, b;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nranks);

    if (rank == 0)
    {
        N = 100;
        a = 1;
        b = 2;
    }

    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&b, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double width = (b - a) / N;

    int numele = N / nranks + 1;
    int start, end;

    start = rank * numele + 1;
    end = start + numele - 1;
    if (end > N)
        end = N;

    int result = 0;

    for (int i = start; i < end; i++)
    {
        result += f(a + (i - 1) * width);
    }

    double total = 0;
    MPI_Reduce(&result, &total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("Total: %f\n", total);
    }

    MPI_Finalize();

    return 0;
}