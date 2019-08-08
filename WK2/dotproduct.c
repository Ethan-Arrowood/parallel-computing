#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv) {
    // initialize variables
    // a and b are pointers that will be dynamically allocated
    int size;
    double *a;
    double *b;

    // Get user input. scanf uses the memory address of the size
    // variable to capture the user's input
    printf("Enter size of vectors:");
    scanf("%d", &size);

    // dynamically allocate the a and b arrays via malloc 
    a = malloc(size * sizeof(double));
    b = malloc(size * sizeof(double));
    //a=(double*) malloc(size*sizeof(*a));
    
    // fill the arrays
    for (int i=0; i<size; i++) {
        a[i] = 1.5;
        b[i] = 2.25;
    }

    // calculate dot product
    double p = 0;
    for(int i=0; i<size; i++) {
        p += (a[i] * b[i]);
    }

    // print the result and free the memory arrays
    printf("Dot Product = %f\n", p);
    free(a);
    free(b);

    return 0;
}
