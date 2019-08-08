#include <stdio.h>
#include <stdlib.h>
#include "mvp.h"

/* 
 * Matrix-Vector Product by Ethan Arrowood
 *
 * This lab utilizes both singular and double pointers to represent matrices for
 * calculating the matrix-vector product. Methods denoted as `1` utilize single
 * pointer arrays and methods denoted as `2` utilize double pointer arrays.
 *
 * Throughout this document, an algorithm is referred to as the standardized
 * index function. This function converts the 2d coordinate pair `(i, j)` to a
 * 1d index for array access purposes. The algorithm is as follows:
 *
 * index = (i * r) + j
 *
 * r is the row count of the 2d matrix
 * */

/*
 * Allocate Matrix 1
 *
 * This function allocates a 1d array representing a 2d matrix bounded by the
 * parameters `r` and `c` representing `rows` and `columns` respectively. It
 * utilizes dynamic allocation to create the array.
 * */
void allocMat1(double **mat, int r, int c)
{
    *mat = malloc(r * c * sizeof(double));
}

/*
 * Allocate Matrix 2
 *
 * This function allocates an array of arrays representing a 2d matrix bounded
 * by the `r` and `c` parameters. It utilizes dynamic allocation and double
 * pointers to create the data structure.
 * */
void allocMat2(double ***mat, int r, int c)
{
    *mat = (double **)malloc(r * sizeof(double *));
    for (int i = 0; i < r; i++)
        (*mat)[i] = (double *)malloc(c * sizeof(double));
}

/*
 * Allocate Vector
 *
 * This function allocates a 1d array representing a vector of length `l` denoted by
 * the `vec` pointer parameter. It uses dynamic allocation to create the array.
 * */
void allocVec(double **vec, int l)
{
    *vec = malloc(l * sizeof(double));
}

/*
 * Assign Matrix 1
 * 
 * Assigns values to the matrix given the following pattern:
 * | 2 1 0 0 0 |
 * | 1 2 1 0 0 |
 * | 0 1 2 1 0 |
 * | 0 0 1 2 1 |
 * | 0 0 0 1 2 |
 * 
 * The matrix parameter `mat` is a 1d array; thus, this function utilizes the standarized index function.
 * */
void assignMat1(double *mat, int r, int c)
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

/*
 * Assign Matrix 2
 *
 * Assigns values to the matrix using the pattern from Assign Matrix 1
 *
 * The matrix parameter `mat` is a 2d array; thus, this function utilizes double
 * bracket notation for matrix access.
 * */
void assignMat2(double **mat, int r, int c)
{
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            if (i == j)
            {
                mat[i][j] = 2;
            }
            else if (i - j == 1 || j - i == 1)
            {
                mat[i][j] = 1;
            }
            else
            {
                mat[i][j] = 0;
            }
        }
    }
}

/*
 * Assign Vector
 *
 * Assigns the value `1` to every index in a vector length `l`
 * */
void assignVec(double *vec, int l)
{
    for (int i = 0; i < l; i++)
    {
        vec[i] = 1;
    }
}

/*
 * Matrix-Vector Product 1
 *
 * Calculates the product of a matrix represented by a 1d array and a vector.
 * The vector length must be equivalent to the row count of the matrix denoted
 * by parameter `r`.
 *
 * Refer to the Allocate Matrix 1 function to learn more about the 1d array
 * representation of a matrix.
 * */
double *mvp1(double *mat, double *vec, int r, int c)
{
    double *res = (double *)malloc(r * sizeof(double));

    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            res[i] += (mat[i * r + j] * vec[i]);
        }
    }

    return res;
}

/*
 * Matrix-Vector Product 2
 *
 * Calculates the product of a matrix represented by an array of arrays and a
 * vector. The vector length must be equivalent to the row count of the matrix
 * denoted by parameter `r`.
 *
 * Refer to the Allocate Matrix 2 function to learn more about the array of
 * arrays representation of a matrix.
 * */
double *mvp2(double **mat, double *vec, int r, int c)
{
    double *res = (double *)malloc(r * sizeof(double));

    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            res[i] += (mat[i][j] * vec[i]);
        }
    }

    return res;
}

/*
 * Print Matrix-Vector 1
 *
 * Prints the matrix-vector product operation to stdout. The vector and res
 * lengths should be equal to the row count of the matrix represented by
 * parameter `r`. The matrix should be represented by a 1d array.
 *
 * Refer to the Allocate Matrix 1 function to learn more about the 1d array
 * representation of a matrix.
 * */
void printMatVec1(double *mat, double *vec, double *res, int r, int c)
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

/*
 * Print Matrix-Vector 2
 *
 * Prints the matrix-vector product operation to stdout. The vector and res
 * lengths should be equal to the row count of the matrix represented by
 * parameter `r`. The matrix should be represented by an array of arrays.
 *
 * Refer to the Allocate Matrix 2 function to learn more about the array of
 * arrays representation of a matrix.
 * */
void printMatVec2(double **mat, double *vec, double *res, int r, int c)
{
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            printf("%.2f ", mat[i][j]);
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

/*
 * Free Matrix
 * 
 * This method deallocates an array of arrays representation of a 2d matrix.
 * */
void freeMat(double **mat, int r)
{
    for (int i = 0; i < r; i++)
    {
        free(mat[i]);
    }
    free(mat);
}

int main(int argc, char **argv)
{
    // initialize row and column count
    int r, c;
    printf("Enter number of rows:\n");
    scanf("%d", &r);

    printf("Enter number of columns:\n");
    scanf("%d", &c);

    printf("\n");

    // initialize matrix 1 as a single pointer array
    double *mat1 = NULL;
    allocMat1(&mat1, r, c);
    assignMat1(mat1, r, c);

    // initialize matrix 2 as a double pointer array
    double **mat2 = NULL;
    allocMat2(&mat2, r, c);
    assignMat2(mat2, r, c);

    // initialize vector as a single pointer array
    double *vec = NULL;
    allocVec(&vec, r);
    assignVec(vec, r);

    // calculate matrix-vector product 1
    double *res1 = mvp1(mat1, vec, r, c);

    // calculate matrix-vector product 2
    double *res2 = mvp2(mat2, vec, r, c);

    // print results to stdout
    printf("mvp1:\n");
    printMatVec1(mat1, vec, res1, r, c);
    printf("\nmvp2:\n");
    printMatVec2(mat2, vec, res2, r, c);

    // free all dynamically allocated data
    free(mat1);
    freeMat(mat2, r);
    free(vec);
    free(res1);
    free(res2);

    return 0;
}