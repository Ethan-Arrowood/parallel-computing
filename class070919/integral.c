#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

double func(double x);

int main(int argc, char **argv)
{

    // Initialize MPI
    int rank, numranks;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int N = 100000;
    int d = N / numranks;
    // 100
    // 4 ranks: 0-25, 25-50, 50-75, 75-100
    // 5 ranks: 0-20, 20-40, 40-60, 60-80, 80-100
    double a = 0, b = 3;
    double width = (b - a) / N;

    int start, end;
    int totArea = 0;
    int area, stat;

    if (rank == 0)
    {
        for (int i = 1; i < numranks; i++)
        {
            start = i * d;
            end = start + d;
            if (rank == numranks - 1)
                end = N;
            MPI_Send(&start, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&end, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        for (int i = 1; i < numranks; i++)
        {
            MPI_Recv(&area, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &stat);
            totArea += area;
        }
        printf("Area: %f\n", totArea);
    }
    else
    {
        MPI_Recv(&start, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);
        MPI_Recv(&end, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);
        for (int i = start; i < end; i++)
        {
            area += func(a + (i - 1) * width);
        }
        MPI_Send(&area, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    if (rank == 0)
    {
        printf("Result: %f | Num Ranks: %d", totArea, numranks);
    }

    MPI_Finalize(); // clean up

    return 0;
}

double func(double x)
{
    return 3. * exp(-x) * sin(x) * sin(x) + 1.0;
}
