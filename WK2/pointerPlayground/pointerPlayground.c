/*************************************************
Pointer Playground Source Code
Author: Micah Schuster

This code is ment to illustrate pointer usage
in C/C++ in a variety of ways. Run the program,
observe the output, and look at this source code
to see how to use pointers in different 
situations.

*************************************************/
#include <stdio.h>
#include <stdlib.h>

//function prototypes
void allocatePointers();
void functionCalls();
void ptrFunction(int *ptr);
int* ptrFunction2(int *ptr);
void ptrFunction3(int **ptr);


int main(int argc, char **argv){

    
    //pointer allocation
    allocatePointers();

    functionCalls();



}


/***************************
Function: allocatePointers

input: void
output: void

This function shows how to
allocate memory for 1 and 
2D structures 
***************************/
void allocatePointers(){

    printf("\n*************************\nSingle Pointer Fun\n*************************\n\n");

    printf("Reference operator: &, Dereference operator: *\n");
    int *ptr=NULL;
    printf("Value of ptr: %p\n",ptr);
    int a=1;
    printf("Value of a: %d, Pointer to value of a (reference to) - &a: %p\n",a,&a);
    printf("Setting ptr to reference to a: ptr=&a\n");
    ptr=&a;
    printf("Value of ptr: %p, Dereference of ptr - *ptr: %d\n",ptr,*ptr);




    printf("\n*************************\nArray Allocation\n*************************\n");

    //dimension of structures
    int n=5;

    //allcoate 1d - array
    int *array = (int*) malloc(n*sizeof(int));
    printf("\n=====Fun with Pointers and Arrays...=====\n\n");
    printf("Array Pointer: %p",array);
    printf("\n  Note: This will point to the first element of the array!\n");

    printf("\nArray Element Memory Locations:\n");
    printf("i   &array[i]   array+i    array[i] (value)\n");
    for(int i=0;i<n;i++){
        printf("%d:  %p    %p   %d\n",i,&array[i],array+i,array[i]);
    }
    
    printf("  Note that the array elements are contiguous in memory!\n");



    int **matrix = (int**) malloc(n*sizeof(int*));
    for(int i=0;i<n;i++){
        matrix[i]=(int*) malloc(n*sizeof(int));
    }
    //set a value in the matrix
    matrix[2][2]=5;
    printf("\n\n=====Fun with Pointers and Matrices...=====\n");
    printf("     =====A bit more complicated=====\n");

    printf("\nMatrix Memory Locations:\n");
    printf("&matrix[0]: %p  matrix:  %p\n",&matrix[0],matrix);


    printf("\nMatrix Row Memory Locations: (pointers are 8 bytes in size)\n");
    printf("Row   &matrix[i]   matrix+i    value of matrix[i]\n");
    for(int i=0;i<n;i++){
        printf("%d:  %p    %p    %p\n",i,&matrix[i],matrix+i,matrix[i]);
    }


    printf("\nMatrix Element Memory Locations and Values: (C is row-major)\n");
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            printf("%p: %d  ",&matrix[i][j],matrix[i][j]);
        }
        printf("\n");
    }


    
    printf("  Note: the individual rows are contiguous in memory\n");
    printf("        but the full matrix is not!\n");


}

/***************************
Function: functionCalls

input: void
output: void

This function shows how 
function calls work
with pointers
***************************/
void functionCalls(){

    printf("\n*************************\nFunction Calls\n*************************\n\n");


    //create a null pointer
    int *ptr = NULL;
    printf("Null Pointer ptr: %p\n",ptr);
    printf("Calling Function...\n");
    ptrFunction(ptr);
    printf("ptr After Function Call: %p  (C is pass by value)\n\n",ptr);


    ptr=NULL;
    printf("Null Pointer ptr: %p\n",ptr);
    printf("Calling Function with int* return value...\n");
    ptr=ptrFunction2(ptr);
    printf("ptr After Function Call: %p\n\n",ptr);

    ptr=NULL;
    printf("NULL Pointer ptr: %p\n",ptr);
    printf("Calling Function with void return and &ptr argument...\n");
    ptrFunction3(&ptr);
    printf("ptr After Function Call: %p\n",ptr);


}

/***************************
Function: ptrFunction

input: *ptr, integer pointer
output: void

This function shows that
even if you set the input
to a value, it will not
get returned with that value.
C is pass by value!
***************************/
void ptrFunction(int *ptr){
    int num;
    ptr=&num;
    //this could also represent allocating memory
    //and assigning it to a pointer
    printf("Changed ptr in Function: %p\n",ptr);
}

/***************************
Function: ptrFunction2

input: *ptr, integer pointer
output: int*, integer pointer

This function shows that
you can return a pointer
after creating or setting
it in the function.
***************************/
int*  ptrFunction2(int *ptr){
    int num;
    ptr=&num;
    //this could also represent allocating memory
    //and assigning it to a pointer
    printf("Changed ptr in Function: %p\n",ptr);
    return ptr;
}

/***************************
Function: ptrFunction3

input: **ptr, integer pointer to a pointer
output: void

This function is a bit more
complicated. You can send
in a reference to the (&ptr)
and change the value (*ptr)
so that you don't have to return
a new pointer.
***************************/
void ptrFunction3(int **ptr){
    int num;
    *ptr=&num;
    //note that we need a pointer to a pointer (**) in the 
    //parameter list so that we can change what our pointer
    //points to (by using *ptr=)
    printf("Changed ptr in Function: %p\n",*ptr);
}
