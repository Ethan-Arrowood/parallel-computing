#include <stdio.h>

int main() {
    // double a[] = {1.5, 1.5, 1.5};
    // double b[] = {2.25, 2.25, 2.25};
    double a[] = {1.0, 2.0, 3.0};
    double b[] = {4.0, 5.0, 6.0};

    double p = 0;
    for(int i=0; i<3; i++) {
        p += (a[i] * b[i]);
    }
    printf("Dot Product = %f\n", p);
    return 0;
}
