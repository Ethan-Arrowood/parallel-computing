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

    double t_start, t_end, t_total;
    t_start = MPI_Wtime(); // Get start time

    // define constants
    int N;
    double a, b;

    N = 500000000;
    a = 0;
    b = 3;

    double width = (b - a) / N;
    double result = 0;

    // iteration automatically splits workload over ranks
    /*
        Starting from the rank's value + 1, the process
        increases by the number of ranks. This methodology
        evenly splits up the work done by the application
        over all of the processes. Each process will calculate
        a similar number of small and large values of `i`. 
     */
    for (int i = rank + 1; i <= N; i += numranks)
    {
        result += func(a + (i - 1) * width);
    }

    // MPI Reduce - sums the separate processes results into one result on rank 0
    double reducedResult = 0;
    MPI_Reduce(&result, &reducedResult, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    //multiply by the width
    reducedResult *= width;

    t_end = MPI_Wtime(); // end timer

    if (rank == 0)
    {
        // get results and print
        // calculate error from exact
        t_total = t_end - t_start;
        double iExact = 4.14801;
        double error = (reducedResult - iExact) / iExact;
        printf("Exact: %.8f\n", iExact);
        printf("The numerical approximation is: %.8f\n", reducedResult);
        printf("Error: %f\n", error);
        printf("Num Ranks: %d | Time: %f\n", numranks, t_total);
    }

    MPI_Finalize(); // clean up

    return 0;
}

double func(double x)
{
    return 3. * exp(-x) * sin(x) * sin(x) + 1.0;
}
