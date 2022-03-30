#include "strassen.cpp"
#include <chrono>

using namespace std::chrono;

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

    // test regular matrix multiplication
    printf("Tiny matrix result:--------------------\n");
    Matrix M3(2, 1);
    auto res3 = M3.read_input("input1tiny.txt");
    int* C = M3.standard_mult(M3.n, false, std::pair<int,int>(0,0), std::pair<int,int>(0,0));
    for (int r = 0; r < M3.n; r++){
        for (int c = r*M3.n; c < (r+1)*M3.n; c++){
            printf("%d\t", C[c]);
        }
        printf("\n");
    }
    printf("Medium matrix result:--------------------\n");
    Matrix M5(3, 1);
    auto res5 = M5.read_input("input1small.txt");
    int* C3 = M5.standard_mult(M5.n, false, std::pair<int,int>(0,0), std::pair<int,int>(0,0));
    for (int r = 0; r < M5.n; r++){
        for (int c = r*M5.n; c < (r+1)*M5.n; c++){
            printf("%d\t", C3[c]);
        }
        printf("\n");
    }

    // test timing of large regular matrix multiplication
    printf("Big matrix result:--------------------\n");
    Matrix M4(22, 1);
    auto res4 = M4.read_input("input1big.txt");
    auto start = high_resolution_clock::now();
    int* C2 = M4.standard_mult(M4.n, false, std::pair<int,int>(0,0), std::pair<int,int>(0,0));
    auto stop = high_resolution_clock::now();
    for (int r = 0; r < M4.n; r++){
        for (int c = r*M4.n; c < (r+1)*M4.n; c++){
            printf("%d\t", C2[c]);
        }
        printf("\n");
    }
    auto duration = duration_cast<seconds>(stop - start);
    printf("Total mult time in seconds: %ld\n", duration.count());

    // test regular multiplication with padding
    printf("Medium matrix result with padding:--------------------\n");
    Matrix M6(4, 1);
    auto res6 = M6.read_input("input1small_pad.txt");
    int* C4 = M6.standard_mult(M6.n, false, std::pair<int,int>(0,0), std::pair<int,int>(0,0));
    for (int r = 0; r < M6.n; r++){
        for (int c = r*M6.n; c < (r+1)*M6.n; c++){
            printf("%d\t", C4[c]);
        }
        printf("\n");
    }
};

