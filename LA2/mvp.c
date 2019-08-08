#include <stdio.h>
#include <stdlib.h>
#include "mvp.h"

void allocMat(double **mat, int r, int c)
{
    *mat = malloc(r * c * sizeof(double));
}

void allocVec(double **vec, int l)
{
    *vec = malloc(l * sizeof(double));
}

void assignMat(double *mat, int r, int c)
{
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            if (i == j)
            {
                mat[i * r + j] = 2;
            }
            else if (i - j == 1 || j - i == 1)
            {
                mat[i * r + j] = 1;
            }
            else
            {
                mat[i * r + j] = 0;
            }
        }
    }
}

void assignVec(double *vec, int l)
{
    for (int i = 0; i < l; i++)
    {
        vec[i] = 1;
    }
}

/*
    Calculating the matrix-vector product in a parallel way requires a
    slight modification from the previous implementation. The index transform
    uses the `c` parameter as this refers to the square dimension of the matrix.

    The `r` parameter is now no longer the same as `c` because it is the length
    of the vector.
 */
double *mvp(double *mat, double *vec, int r, int c)
{
    double *res = (double *)malloc(r * sizeof(double));

    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            res[i] += (mat[i * c + j] * vec[i]);
        }
    }

    return res;
}

void printMatVec(double *mat, double *vec, double *res, int r, int c)
{
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            printf("%.2f ", mat[i * r + j]);
        }
        printf(" %.2f ", vec[i]);
        if (i == r / 2)
        {
            printf("=");
        }
        else
        {
            printf(" ");
        }
        printf(" %.2f\n", res[i]);
    }
}
