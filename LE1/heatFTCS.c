//Hints:
// 1. You can't parallelize over the time dimension, only over the spatial grid
// 2. You need the previous time step to compute the next time step, Bcast it to every rank
// 3. After every rank computes its segment of the spatial grid, gather it back to rank 0
// 4. Any loop over nx is looping over the spatial grid, any loop over nt is looping over time
// 5. Don't mess with the A matrix, it configured to solve the PDE, just let all ranks have the full matrix

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

double norm(double *v, int n);
void freeMatrix(double **mat, int x);

int main(int argc, char **argv)
{

    // Initialize MPI
    MPI_Init(&argc, &argv);
    int rank, nranks;
    MPI_Comm_size(MPI_COMM_WORLD, &nranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Initialize Timing
    double t_s, t_e, t_t;

    t_s = MPI_Wtime(); // start timer

    //initialization
    double pi = acos(-1.0);
    double u0 = 0;
    double uL = 0;
    double alpha = 1;
    double L = 100;
    double tmax = 0.01;
    int nt = 10000; //Number of time steps: for timing runs use 10000, for testing runs use 10
    int nx = 3000;  //Size of spatial grid: for timing runs use 3000, for testing runs use 10

    double dx = L / (nx - 1);
    double dt = tmax / (nt - 1);
    double r = alpha * dt / (dx * dx);
    if (r >= 0.5)
        printf("\n\nWARNING: Solution may be unstable. r=%f\n\n", r);
    double r2 = 1 - 2 * r;

    // Allocate Arrays

    //U is a 2D array, dim one is time, dim two is space
    double **U = malloc(nt * sizeof(double *));
    // A is space by space
    double **A = malloc(nx * sizeof(double *));
    for (int i = 0; i < nt; i++)
    {
        U[i] = malloc(nx * sizeof(double));
    }
    for (int i = 0; i < nx; i++)
    {
        A[i] = malloc(nx * sizeof(double));
    }

    // Fill array A
    for (int i = 0; i < nx; i++)
    {
        for (int j = 0; j < nx; j++)
        {
            A[i][j] = 0.0;
        }
    }

    for (int i = 0; i < nx; i++)
    {
        double temp = dx * (i);
        U[0][i] = sin(pi * temp / L); //initial condition, time=0

        //configure the FTCS matrix solver
        if (i == 0 || i == nx - 1)
        {
            A[i][i] = 1.0;
        }
        else
        {
            A[i][i] = 1 - 2 * r;
            A[i][i + 1] = r;
            A[i][i - 1] = r;
        }
    }

    //Main Computation - Parallel This Section

    int spatialSplit = nx / nranks; // find the spatial array split for parallelization

    double *prevTimeStep = malloc(nx * sizeof(*prevTimeStep)); // initialize previous time step array

    //time step loop
    for (int t = 1; t < nt; t++)
    {
        // assign and broadcast prev time step array
        for (int s = 0; s < nx; s++)
        {
            prevTimeStep[s] = U[t - 1][s];
        }
        MPI_Bcast(prevTimeStep, nx, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        // Use Rank start increment parallelization
        // Start at the rank and increment evenly to cover all values evenly
        // This evenly spreads small and large values of i over the ranks
        for (int i = rank; i < nx; i += nranks)
        {
            double temp = 0.0;
            for (int j = 0; j < nx; j++)
            {
                temp += prevTimeStep[j] * A[i][j];
            }
            U[t][i] = temp;
        }
    }

    free(prevTimeStep); // free up prevTimeStep

    if (rank == 0)
    {
        // Handle expected calculation and normalize for error calculation

        double *Ue = malloc(nx * sizeof(*Ue));
        double *vnorm = malloc(nx * sizeof(*vnorm));
        //computing the exact solution and error
        for (int i = 0; i < nx; i++)
        {
            double temp = dx * (i);
            Ue[i] = sin(pi * temp / L) * exp(-tmax * alpha * (pi / L) * (pi / L));
            vnorm[i] = U[nt - 1][i] - Ue[i];
        }

        double err = norm(vnorm, nx);

        t_e = MPI_Wtime(); // end timer and print out

        t_t = t_e - t_s;
        printf("Num Ranks: %d | Execution time: %f\n", nranks, t_t);
        //error should be small
        printf("\nError ||approx-exact||: %f\n\n", err);

        free(vnorm); // free vectors
        free(Ue);

        //output is here. For your big runs, only output the last time step
        FILE *f = fopen("outputC.dat", "w");
        for (int i = 0; i < nx; i++)
        {
            fprintf(f, "%f    ", dx * i);
            if (nt > 10 || nx > 10) // only output the final time sig on non test runs
            {
                fprintf(f, "%f ", U[nt - 1][i]);
            }
            else
            {
                for (int t = 0; t < nt; t++)
                {
                    fprintf(f, "%f ", U[t][i]);
                }
            }
            fprintf(f, "    %f\n", Ue[i]);
        }

        fclose(f);
    }

    freeMatrix(U, nx); // free up matrix
    freeMatrix(A, nx);

    MPI_Finalize(); // finalize MPI

    return 0;
}

double norm(double *v, int n)
{

    double temp = 0;
    for (int i = 0; i < n; i++)
    {
        temp += v[i] * v[i];
    }
    return sqrt(temp);
}

void freeMatrix(double **mat, int x)
{
    for (int i = 0; i < x; i++)
    {
        free(mat[i]);
    }
    free(mat);
}
