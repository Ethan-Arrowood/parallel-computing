#include "mpi.h"
#include <stdio.h>

int main(int argc, char **argv)
{
    int rank, numranks;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int size = 5;
    int data[size + 2][size]; // 7 by 5 array of ints

    for (int i = 0; i < size + 2; i++)
    {
        for (int j = 0; j < size; j++)
        {
            data[i][j] = rank; // assign the rank of the node to every cell
            // these would be the edges of the grid
            if (i == size + 1) // if the row iterator is 6 set the cell value to -1
                data[i][j] = -1;
            if (i == 0) // if the row iterator is 0 set the cell value to -1
                data[i][j] = -1;
        }
    }

    // set next and prev rank but make sure they are not out of bounds
    int next = rank + 1;
    int prev = rank - 1;
    if (next > numranks - 1)
        next = MPI_PROC_NULL;
    if (prev < 0)
        prev = MPI_PROC_NULL;

    // The following data exchange takes the last row from the previous grid and inserts it as the first row in the current grid.
    // It also takes the first row from the next grid and inserts it as the last row in the current grid.
    // For the first and last rank the first and last row of the respective grids do not get modified and remain as -1.

    // Send current rank's last row to the next process (for it to be inserted as the first row in that process' grid)
    MPI_Sendrecv(data[size], size, MPI_INT, next, 0, data[0], size, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    // Send current rank's first row to the prev process (for it to be inserted as the last row in that process' grid)
    MPI_Sendrecv(data[1], size, MPI_INT, prev, 0, data[size + 1], size, MPI_INT, next, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // print out the grid
    for (int k = 0; k < numranks; k++)
    {
        if (rank == k)
        {
            printf("RANK: %d\n", rank);
            for (int i = 0; i < size + 2; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    printf("%d", data[i][j]);
                }
                printf("\n");
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    MPI_Finalize();
}