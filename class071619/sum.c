#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char **argv)
{
    int x = 0;
#pragma omp barrier
    {
        x = x + 1;
    }
    printf("X: %d\n", x);
}