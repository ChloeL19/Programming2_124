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

void printSubMatrix(int dim, bool pad, int* ptr, std::pair<int,int>coord) {
    //printf("the dimension: %d", dim);
    // [0,2]
    int subdim = dim/2;
    if (pad){
        dim = dim - 1;
    }
    //printf("starting coordinates: %d, %d\n", coord.first, coord.second);
    for (int i = 0; i < subdim; i++){
        for (int j = 0; j < subdim; j++){
            //printf("Column: %d, row: %d\n", j, i);
            //printf("The dimension %d: the column iterator: %d\n", dim, j + coord.second);
            if (pad && coord.second + j > dim-1){
                //printf("t,dim:%d>=%dt",coord.second + j, dim-1);
                printf("0\n");
            } else {
                printf("%d\t", ptr[(i + coord.first)*dim + j + coord.second]);
            }
            //printf("%d\t", ptr[(i + coord.first)*dim + j + coord.second]);
        }
        printf("\n");
    }
}

int main(){
    std::vector<std::pair<int, std::pair<int, int>>> ccoord;
    ccoord.push_back(std::pair<int, std::pair<int,int>>(1, std::pair<int,int>(0,0)));
    // test timing of large regular matrix multiplication
    printf("Big matrix result for strassen:--------------------\n");
    Matrix M4(22, 1);
    auto res4 = M4.read_input("input1big.txt");
    // strassen computation now
    auto start1 = high_resolution_clock::now();
    auto C1 = M4.strassens_w_crossover(M4.n, false, M4.A, M4.B, 
        std::pair<int,int>(0,0), std::pair<int,int>(0,0), ccoord);
    auto stop1 = high_resolution_clock::now();
    // strassens version printed
    for (int i = 0; i < M4.n; i++){
        for (int j = 0; j < M4.n; j++){
            if (i == j){
                printf("%d\n", C1.second[(i*M4.n) + j]);
            }
        }
    }
    auto duration1 = duration_cast<seconds>(stop1 - start1);
    printf("Total mult time in seconds: %ld\n", duration1.count());
    // normal version computation now
    printf("Big matrix result for normal:--------------------\n");
    auto start = high_resolution_clock::now();
    int* C2 = M4.standard_mult(M4.n, false, M4.A, M4.B, M4.C, std::pair<int,int>(0,0), std::pair<int,int>(0,0),
            ccoord);
    auto stop = high_resolution_clock::now();
    for (int r = 0; r < M4.n; r++){
        for (int c = r*M4.n; c < (r+1)*M4.n; c++){
            printf("%d\t", C2[c]);
        }
        printf("\n");
    }
    auto duration = duration_cast<seconds>(stop - start);
    printf("Total mult time in seconds: %ld\n", duration.count());

    // normal version printed
    
    
    
    /*// Test reading from input file
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
    std::vector<std::pair<int, std::pair<int,int>>> ccoord;
    ccoord.push_back(std::pair<int, std::pair<int,int>>(1,std::pair<int,int>(0,0)));
    int* C = M3.standard_mult(M3.n, false, M3.A, M3.B, M3.C, std::pair<int,int>(0,0), std::pair<int,int>(0,0),
            ccoord);
    for (int r = 0; r < M3.n; r++){
        for (int c = r*M3.n; c < (r+1)*M3.n; c++){
            printf("%d\t", C[c]);
        }
        printf("\n");
    }
    printf("Medium matrix result:--------------------\n");
    Matrix M5(3, 1);
    auto res5 = M5.read_input("input1small.txt");
    int* C3 = M5.standard_mult(M5.n, false, M5.A, M5.B, M5.C, std::pair<int,int>(0,0), std::pair<int,int>(0,0),
            ccoord);
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
    int* C2 = M4.standard_mult(M4.n, false, M4.A, M4.B, M4.C, std::pair<int,int>(0,0), std::pair<int,int>(0,0),
            ccoord);
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
    // so it is necessary to invoke padding functionality
    // we pretend that the matrix we are passing into multiply has already been padded
    // its dimension including its padded border is n
    printf("Medium matrix result with padding:--------------------\n");
    Matrix M6(3, 1);
    auto res6 = M6.read_input("input1small.txt");
    printf("Printing Matrix A-----------------------------\n");
    printMatrix(M6.n, M6.A);
    printf("Printing Matrix B------------------------------\n");
    printMatrix(M6.n, M6.B);
    // we will always increment dimension to make it divisible by 2 if it is odd
    M6.n = 4;
    int* C4 = M6.standard_mult(M6.n, true, M6.A, M6.B, M6.C, std::pair<int,int>(0,0), std::pair<int,int>(0,0),
        ccoord);
    printf("Multiplication with padding result---------------\n");
    for (int r = 0; r < M6.n; r++){
        for (int c = r*M6.n; c < (r+1)*M6.n; c++){
            printf("%d\t", C4[c]);
        }
        printf("\n");
    }
    printf("Printing Matrix A-----------------------------\n");
    printMatrix(M6.n, M6.A);
    printf("Printing Matrix B------------------------------\n");
    printMatrix(M6.n, M6.B);

    // test subtraction of partial matrices with padding
    std::pair<int,int> F = std::pair<int,int>(0, M6.n/2);
    std::pair<int,int> H = std::pair<int,int>(M6.n/2, M6.n/2);
    auto f_min_h = M6.subtract(M6.n, true, -1, M6.B, M6.B, F, H);
    printf("Printing Matrix A-----------------------------\n");
    printMatrix(M6.n, M6.A);
    printf("Printing Matrix B------------------------------\n");
    printMatrix(M6.n, M6.B);
    printf("Printing Matrix F------------------------------\n");
    printSubMatrix(M6.n, true, M6.B, F);
    printf("Printing matrix H------------------------------\n");
    printSubMatrix(M6.n, true, M6.B, H);
    printf("Printing Matrix of Subtractions-------------------------------\n");
    //printf("%d\n", f_min_h);
    printMatrix(M6.n/2, f_min_h);
    // for (int i = 0; i < (M6.n/2)*(M6.n/2); i++){
    //     printf("%d\t", f_min_h[i]);
    // }
    // printf("\n");

    // test subtraction of partial matrices without padding
    printf("Testing subtraction on big matrix without padding------------------\n");
    std::pair<int,int> F2 = std::pair<int,int>(0, M4.n/2);
    std::pair<int,int> H2 = std::pair<int,int>(M4.n/2, M4.n/2);
    auto f_min_h2 = M6.subtract(M4.n, false, -1, M4.B, M4.B, F2, H2);
    printf("Printing Matrix A-----------------------------\n");
    printMatrix(M4.n, M4.A);
    printf("Printing Matrix B------------------------------\n");
    printMatrix(M4.n, M4.B);
    printf("Printing Matrix F------------------------------\n");
    printSubMatrix(M4.n, false, M4.B, F2);
    printf("Printing matrix H------------------------------\n");
    printSubMatrix(M4.n, false, M4.B, H2);
    printf("Printing Matrix of Subtractions-------------------------------\n");
    printMatrix(M4.n/2, f_min_h2);*/
};

