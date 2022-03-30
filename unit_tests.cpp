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
    Matrix M(3, 1);
    auto res = M.read_input("input1small.txt");
    if (res.first == NULL){
        printf("Error with reading.\n");
    }
    printf("A matrix from small input file:--------\n");
    printMatrix(M.n, M.A);
    printf("B matrix from small input file:--------\n");
    printMatrix(M.n, M.B);
    printf("the end\n");
};

