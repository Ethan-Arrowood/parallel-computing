#include <stdlib.h>
#include <stdio.h>
//#define __USE_MISC
#include <math.h>
#include "mpi.h"

extern int *imageToMat(char *name, int *dims);
extern void matToImage(char *name, int *mat, int *dims);
void createFilter(double **gKernel, int size);

int main(int argc, char **argv)
{
    // Initialize MPI
    int nranks, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Initialize Timers
    double s_t, e_t, t_t;
    double tempCommTime, commTime = 0;
    double tempCalcTime, calcTime = 0;

    // Start total application time
    s_t = MPI_Wtime();

    // initialize matrix and name
    int *matrix;
    char *name = "image.jpg";

    //read image, conver to matrix, and get dimensions
    int height, width;
    int *dims = (int *)malloc(2 * sizeof(int));
    // matrix only needs to be created on rank 0, it will be scattered to all other ranks
    if (rank == 0)
    {
        matrix = imageToMat(name, dims);
        printf("Dims %d %d\n", dims[0], dims[1]);
        height = dims[0];
        width = dims[1];
    }

    // time communication
    tempCommTime = MPI_Wtime();
    // broadcast height and width to all ranks
    MPI_Bcast(&height, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&width, 1, MPI_INT, 0, MPI_COMM_WORLD);
    commTime += MPI_Wtime() - tempCommTime;

    int hSplit = height / nranks;
    // allocate sub matrix - its size is the evenly distributed matrix from rank 0
    int *subMatrix = (int *)malloc(hSplit * width * sizeof(int));

    // time communication
    tempCommTime = MPI_Wtime();
    // scatter matrix evenly accross processes
    MPI_Scatter(matrix, hSplit * width, MPI_INT, subMatrix, hSplit * width, MPI_INT, 0, MPI_COMM_WORLD);
    commTime += MPI_Wtime() - tempCommTime;

    // allocate temp array for convolve calculation
    int *temp = (int *)malloc(hSplit * width * sizeof(int));

    //allocate kernel on all ranks -- 2D arrays are hard to broadcast, this is small enough to do on every rank
    int size = 31;
    int range = size / 2;
    double **gKernel = (double **)malloc(size * sizeof(double));
    for (int i = 0; i < size; i++)
    {
        gKernel[i] = (double *)malloc(size * sizeof(double));
    }

    //create blur filter
    createFilter(gKernel, size);

    if (size <= 5)
    { //then display filter
        for (int i = 0; i < size; ++i)
        {
            for (int j = 0; j < size; ++j)
            {
                printf("%5.2f  ", gKernel[i][j]);
            }
            printf("\n");
        }
    }

    //convolve
    double sum = 0;
    int matval = 0;

    // time calculation
    tempCalcTime = MPI_Wtime();
    //loop over all pixels
    for (int i = 0; i < hSplit; i++) // loop over subMatrix dimensions hSplit and width
    {
        for (int j = 0; j < width; j++)
        {
            sum = 0;
            int count = 0;

            //loop over kernel
            for (int ii = -range; ii <= range; ii++)
            {
                for (int jj = -range; jj <= range; jj++)
                {
                    if (i + ii < 0 || j + jj < 0 || i + ii > hSplit - 1 || j + jj > width - 1) // do not access out-of-bounds array values
                    {                                                                          //if the kernel position is outside of the image
                        continue;
                    }
                    else
                    {
                        matval = subMatrix[(i + ii) * width + (j + jj)]; // get the submatrix value
                    }
                    count++;
                    sum = sum + gKernel[ii + range][jj + range] * matval;
                }
            }
            temp[i * width + j] = sum / count; //sum divided by normalization
        }
    }
    calcTime += MPI_Wtime() - tempCalcTime;

    // result array to gather the scattered submatrices
    int *result = (int *)malloc(height * width * sizeof(int));

    // time communication
    tempCommTime = MPI_Wtime();
    // gather the temp arrays into result in rank order
    MPI_Gather(temp, hSplit * width, MPI_INT, result, hSplit * width, MPI_INT, 0, MPI_COMM_WORLD);
    commTime += MPI_Wtime() - tempCommTime;

    // finalize the image from the gathered result array
    if (rank == 0)
    {
        //save image
        matToImage("processedImage.jpg", result, dims);

        // end timing
        e_t = MPI_Wtime();

        t_t = e_t - s_t;

        // print number of ranks and total application time
        printf("Ranks: %d | Time: %f\n", nranks, t_t);
    }

    // collect the comm and calc time of all ranks
    double totalCommTime, totalCalcTime;

    // reduce for average computation
    MPI_Reduce(&commTime, &totalCommTime, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&calcTime, &totalCalcTime, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // on rank 0, average the total communication and calculation times
    if (rank == 0)
    {
        printf("Average Communication Time: %f\n", totalCommTime / nranks);
        printf("Average Calculation Time: %f\n", totalCalcTime / nranks);
    }

    // Finalize MPI and free memory
    MPI_Finalize();

    if (rank == 0)
    {
        free(matrix);
    }

    free(subMatrix);
    free(temp);
    free(result);

    for (int i = 0; i < size; i++)
    {
        free(gKernel[i]);
    }
    free(gKernel);

    return 0;
}

void createFilter(double **bKernel, int size)
{
    double sum = 0.0;
    int range = size / 2;
    for (int x = -range; x <= range; x++)
    {
        for (int y = -range; y <= range; y++)
        {
            bKernel[x + range][y + range] = 1;
        }
    }
}