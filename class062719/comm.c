#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>

int main(int argc, char **argv)
{
    int rank, numranks;

    MPI_Comm comm = MPI_COMM_WORLD;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(comm, &numranks);
    MPI_Comm_rank(comm, &rank);

    int width = 3;
    int rows = numranks / width;
    int cols = width;

    int layout[rows * cols];
    for (int i = 0; i < rows * cols; i++)
    {
        layout[i] = 0;
    }
    layout[rank] = rank;
    MPI_Allreduce(MPI_IN_PLACE, layout, rows * cols, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("Global Layout:\n");
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                printf("%d ", layout[j + i * width]);
            }
            printf("\n");
        }
    }
    MPI_Comm row_comm;
    int color = rank / width;
    int key = rank;
    MPI_Comm_split(MPI_COMM_WORLD, color, key, &row_comm);
    int row_rank;
    MPI_Comm_rank(row_comm, &row_rank);

    for (int i = 0; i < rows * cols; i++)
    {
        layout[i] = 0;
    }
    layout[rank] = row_rank;
    MPI_Allreduce(MPI_IN_PLACE, layout, rows * cols, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("Row Layout:\n");
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                printf("%d ", layout[j + i * width]);
            }
            printf("\n");
        }
    }

    MPI_Comm col_comm;
    color = rank % width;
    key = rank;
    MPI_Comm_split(MPI_COMM_WORLD, color, key, &col_comm);
    int col_rank;
    MPI_Comm_rank(col_comm, &col_rank);
    for (int i = 0; i < rows * cols; i++)
    {
        layout[i] = 0;
    }
    layout[rank] = col_rank;
    MPI_Allreduce(MPI_IN_PLACE, layout, rows * cols, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("Col Layout:\n");
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                printf("%d ", layout[j + i * width]);
            }
            printf("\n");
        }
    }

    MPI_Finalize();
}