#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <mpi.h>

extern void matToImage(char *filename, int *mat, int *dims);

int main(int argc, char **argv)
{
    // Initialize MPI
    int rank, numranks;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int nx = 600; // cols
    int ny = 400; // rows
    int maxiter = 255;
    int *mat;
    double area, i_c, r_c, i_z, r_z;
    double r_start, r_end, i_start, i_end;

    int numoutside = 0;
    int iter;
    // vars for window size 3:2 ratio
    r_start = -2;
    r_end = 1;
    i_start = -1;
    i_end = 1;

    // allocate matrix
    mat = (int *)malloc(nx * ny * sizeof(*mat));

#pragma omp parallel
    {
        double start = omp_get_wtime();
        int tid = omp_get_thread_num();
        // Loop works by processing row by row
        #pragma omp for reduction(+:numOutside) nowait schedule(dynamic)
        for (int i = 0; i < ny; i++) // loop over rows
        {
            for (int j = 0; j < nx; j++) // loop over columns
            {
                i_c = i_start + i / (ny * 1.0) * (i_end - i_start);
                r_c = r_start + j / (nx * 1.0) * (r_end - r_start);
                i_z = i_c;
                r_z = r_c;
                iter = 0;

                while (iter < maxiter)
                {
                    iter += 1;
                    double r_t = r_z * r_z - i_z * i_z; // calc real Z
                    double i_t = 2.0 * r_z * i_z;       // calc imaginary Z
                    i_z = i_t + i_c;
                    r_z = r_t + r_c;
                    if (r_z * r_z + i_z * i_z > 4) // test against threshold
                    {
                        numoutside += 1;
                        break;
                    }
                }
                mat[i * nx + j] = iter;
            }
        }

        double end = omp_get_wtime();
        printf("tid: %d | time: %f\n", tid, end-start);
    }

    area = (r_end - r_start) * (i_end - i_start) * (1.0 * nx * ny - numoutside) / (1.0 * nx * ny);
    printf("Area of Mandelbrot set = %f\n", area);
    int dims[2] = {ny, nx};
    matToImage("image.jpg", mat, dims);

    MPI_Finalize();
}