#include <vector>
#include <string>
#include <stdio.h>
#include <cstdio>

/*
    A struct for representing a matrix object.
*/
struct Matrix {
    int n; // dimension of the matrix
    int cop; // cross-over-point
    //std::vector<std::vector<int>> elements; // elements stored as vector of vectors
                                            // each vector is a row
    int* A;
    int* B;
    Matrix(int dimension, int crossover){
        Matrix::n = dimension; Matrix::cop = crossover;
    }
    ~Matrix() = default;

    /*
        read_input
        Read the numbers from the input file into matrices A and B.
        NOTE: TF just said there is no need to actually store padding! I'll need to ask
    */
   std::pair<int*, int*> read_input(std::string in_file){
       // load everything at once from file 
       // goal is to take advantage of built-in caching optimization in stdio
       
       // store data as chars in a buffer
       auto felements = fopen(in_file.c_str(), "r");
       int total_size = 2 * 2 * Matrix::n * Matrix::n - 1; // account for \n chars!
       int num_el = 2 * 2 * Matrix::n * Matrix::n - 1; // again account for \n chars
       //char elbuf [total_size];
       auto elbuf = (char*) malloc (sizeof(char)*total_size);

       if (felements == NULL){
           printf("Problem opening file stream.\n");
           return std::pair<int*, int*>(NULL, NULL);
       }
       // FIXME: use readline instead!!!! (maybe)
       int result = fread(elbuf, sizeof(char), num_el, felements);
       if (result < num_el){
           printf("Problem reading matrix A.\n");
           printf("expected number of items to read: %d\n", num_el);
           printf("File to read from: %s\n", in_file.c_str());
           printf("showing each thing I read: \n");
           for (int i = 0; i < num_el; i++){
               printf("%d\n", elbuf[i]);
           }
           printf("The return value: %d\n", result);
           return std::pair<int*, int*>(NULL, NULL);
       }
    // split buffer chars into matrices of integers
    int size = Matrix::n * Matrix::n;
    int* A = new int[size]; int* B = new int[size];

    int buf_counter = 0; // tracking postion in the buffer
    for (int r = 0; r < 2*Matrix::n; r++){
        for (int c = r*Matrix::n; c < (r+1)*Matrix::n; c++){
            if (buf_counter < total_size/2){ // check if we are at end of first matrix
                A[c] = strtol(&elbuf[buf_counter], NULL, 10);
            } else {
                B[c - size] = strtol(&elbuf[buf_counter], NULL, 10);
            }
            buf_counter = buf_counter + 2;
        }
    }

    Matrix::A = A; Matrix::B = B;
    return std::pair<int*, int*>(A, B);
   }

    /* 
        stand_mult
        Standard multiplication algorithm.
    */


    /*
        strass_mult
        Strassen's algorithm with crossover.
        We consciously edit the matrices in-place for runtime and space purposes.
        Takes a pointer to the factor matrices and a pointer to the product matrix.
        Returns pointer to the modified product matrix.
        Handle padding in here.
    */


};

/*
    Main function:
    handles command line arguments
        crossover, dimension, inputfile
    prints output in the specified format
*/
// int main(int argc, char* argv[]){
//     return 0;
// };