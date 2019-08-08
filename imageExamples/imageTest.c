#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern int *imageToMat(char *name, int *dims);
extern void matToImage(char *name, int *mat, int *dims);

int main(int argc, char **argv)
{

    int *mat;
    char *name = "image.jpg";
    int *dims;
    dims = (int *)malloc(sizeof(*dims) * 2);

    mat = imageToMat(name, dims);
    printf("Dims: %d %d\n", dims[0], dims[1]);

    srand(time(0));
    //do stuff with the matrix
    for (int i = 0; i < dims[0] * dims[1]; i++)
    {
        // mat[i] = mat[i] + 50;
        // if (mat[i] > 255)
        //     mat[i] = 255;
        mat[i] = (rand() % 255) + 1;
    }

    matToImage("processedImage.jpg", mat, dims);

    return 0;
}
