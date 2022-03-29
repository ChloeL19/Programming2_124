#include "strassen.cpp"

/*
    Print matrix function:
    print all elements of the matrix passed into the argument
*/
void printMatrix(int dimension, int* matrix_ptr){
    for (int r = 0; r < dimension; r++) {
            printf("\n");
        for (int c = r*dimension; c < dimension*dimension; c++) {
            printf("%d\t");
        };
    };
};

int main(){
    // Test reading from input file
    Matrix M(3, 1);
    M.read_input("./input1small.txt");
    printf("A matrix from small input file:--------\n");
    printMatrix(M.n, M.A);
    printf("B matrix from small input file:--------\n");
    printMatrix(M.n, M.B);
    printf("the end");
};

