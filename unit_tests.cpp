#include "strassen.cpp"

/*
    Print matrix function:
    print all elements of the matrix passed into the argument
*/
void printMatrix(int dimension, int* matrix_ptr){
    for (int r = 0; r < dimension; r++) {
        for (int c = r*dimension; c < r*dimension + dimension; c++) {
            printf("%d\t", matrix_ptr[c]);
        };
        printf("\n");
    };
    printf("\n");
};

int main(){
    // Test reading from input file
    printf("Small input test--------------------\n");
    Matrix M1(3, 1);
    auto res1 = M1.read_input("input1small.txt");
    if (res1.first == NULL){
        printf("Error with reading.\n");
    }
    printf("A matrix from small input file:--------\n");
    printMatrix(M1.n, M1.A);
    printf("B matrix from small input file:--------\n");
    printMatrix(M1.n, M1.B);
    printf("the end\n");
    
    printf("Big input test--------------------\n");
    Matrix M2(22, 1);
    auto res2 = M2.read_input("input1big.txt");
    if (res2.first == NULL){
        printf("Error with reading.\n");
    }
    printf("A matrix from small input file:--------\n");
    printMatrix(M2.n, M2.A);
    printf("B matrix from small input file:--------\n");
    printMatrix(M2.n, M2.B);
};

