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

    int nx = 6000; // cols
    int ny = 4000; // rows
    int maxiter = 255;
    // create matrix
    int *mat = (int *)malloc(nx * ny * sizeof(*mat));
    // determine split
    int split = ny / numranks;
    printf("Numranks: %d, split: %d\n", numranks, split);
    // create split matrix 
    int *split_mat = (int *)malloc(split * nx * sizeof(*mat));
    int numoutside = 0;

    // vars for window size 3:2 ratio
    double r_start = -2;
    double r_end = 1;
    double i_start = -1;
    double i_end = 1;

    #pragma omp parallel // omp parallelize
    {
        double i_c, r_c, i_z, r_z;
        double start = omp_get_wtime();
        int tid = omp_get_thread_num();
        // Loop works by processing row by row
        #pragma omp for reduction(+:numoutside) nowait schedule(dynamic) // reduce the numoutside of all threads per node
        for (int i = 0; i < split; i++) // loop over rows
        {
            int fi = i + (rank * split); // i translation factor
            for (int j = 0; j < nx; j++) // loop over columns
            {
                // derivative calculations
                i_c = i_start + fi / (ny * 1.0) * (i_end - i_start); // calculation uses i translation for valid i value per index
                r_c = r_start + j / (nx * 1.0) * (r_end - r_start);
                i_z = i_c;
                r_z = r_c;
                int iter = 0;

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
                split_mat[i * nx + j] = iter; // split matrix makes use of the iterator i 
            }
        }

        double end = omp_get_wtime(); 
        printf("rank: %d | tid: %d | time: %f\n", rank, tid, end-start); // end thread timer
    }
    MPI_Gather(split_mat, split * nx, MPI_INT, mat, split * nx, MPI_INT, 0, MPI_COMM_WORLD); // gather split matrices into matrix
    int reducednumoutside;
    MPI_Reduce(&numoutside, &reducednumoutside, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD); // reduce the numoutside from all ranks
    if (rank == 0) {
        double area = (r_end - r_start) * (i_end - i_start) * (1.0 * nx * ny - reducednumoutside) / (1.0 * nx * ny); // find area
        printf("Area of Mandelbrot set = %f\n", area); // print area
        int dims[2] = {ny, nx};
        matToImage("image.jpg", mat, dims); // print image
    }

    MPI_Finalize(); // finalize 

    return 0;
}
