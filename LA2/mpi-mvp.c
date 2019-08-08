#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include "mvp.h"

int main(int argc, char **argv)
{
    // Initialize MPI
    int ntasks, rank, len;
    char hostname[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double start_prog, end_prog, total_prog;          // Total Program Time
    double start_scatter, end_scatter, total_scatter; // Scatter Time
    double start_gather, end_gather, total_gather;    // Gather Time
    double start_mvp, end_mvp, total_mvp;             // MVP Time

    start_prog = MPI_Wtime();

    // Initialize counts
    // N is the size of the matrix
    // *_count refer to the number of values that will distributed over the nodes for parallelization
    int N = 40000;
    int matrix_count = (N * N) / ntasks;
    int vector_count = N / ntasks;

    // Initialize, allocate, and assign under rank 0
    double *mat = NULL;
    double *vec = NULL;
    if (rank == 0)
    {
        allocMat(&mat, N, N);
        assignMat(mat, N, N);

        allocVec(&vec, N);
        assignVec(vec, N);
    }

    // Initialize and allocate the sub arrays
    double *sub_mat = (double *)malloc(matrix_count * sizeof(double));
    double *sub_vec = (double *)malloc(vector_count * sizeof(double));

    start_scatter = MPI_Wtime();
    // scatter over the available nodes. This implementation only works when N is evenly divisable by number of nodes
    MPI_Scatter(mat, matrix_count, MPI_DOUBLE, sub_mat, matrix_count, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(vec, vector_count, MPI_DOUBLE, sub_vec, vector_count, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    end_scatter = MPI_Wtime();

    start_mvp = MPI_Wtime();
    // Find the mvp of the sub matrix and vector
    double *sub_res = mvp(sub_mat, sub_vec, vector_count, N);
    end_mvp = MPI_Wtime();

    // Allocate final result under rank 0
    double *res = NULL;
    if (rank == 0)
    {
        allocVec(&res, N);
    }

    start_gather = MPI_Wtime();
    // Gather all results to rank 0 (and do so in order)
    MPI_Gather(sub_res, vector_count, MPI_DOUBLE, res, vector_count, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    end_gather = MPI_Wtime();

    // Free all arrays in rank 0
    if (rank == 0)
    {
        // Uncomment these lines if you want to print out the result.
        // printf("\nmvp:\n");
        // printMatVec(mat, vec, res, N, N);
        free(mat);
        free(vec);
        free(res);
    }

    // Free sub arrays
    free(sub_mat);
    free(sub_vec);
    free(sub_res);

    end_prog = MPI_Wtime();

    // total_prog = end_prog - start_prog;
    total_mvp = end_mvp - start_mvp;
    total_scatter = end_scatter - start_scatter;
    total_gather = end_gather - start_gather;

    if (rank == 0)
    {
        printf("%d\n", N);
        printf("%f\n", total_mvp);
        printf("%f\n", total_scatter + total_gather);
    }
    // printf("rank %d | N %d | prog %f | mvp %f | scatter %f | gather %f \n", rank, N, total_prog, total_mvp, total_scatter, total_gather);

    // Finalize
    MPI_Finalize();

    return 0;
}
