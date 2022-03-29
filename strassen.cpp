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
   int read_input(std::string in_file){
       // load everything at once from file 
       // goal is to take advantage of built-in caching optimization in stdio
       int total_size = 2*Matrix::n;
       //int elbuf [total_size];

       // store immediately in the two factor matrices
       int A[total_size/2]; int B[total_size/2];
       auto felements = fopen(in_file.c_str(), "r");
       if (felements == NULL){
           return -1;
       }
       int resulta = fread(A, sizeof(int), total_size/2, felements);
       if (resulta < total_size){
           return -1;
       }
       int resultb = fread(B, sizeof(int), total_size/2, felements);
       if (resultb < total_size){
           return -1;
       }

       Matrix::A = A; Matrix::B = B;
       return 0;
   }

    // OLD UNNECESSARY STUFF
    //    // add padding if necessary
    //    int dimsize = Matrix::n;
    //    if (Matrix::n % 2 != 0){ // if odd, add padding
    //        dimsize++;
    //    }
    //    // fill the elements of the matrix in row-major order
    //    std::vector<int> rowvec;
    //    for (auto row = 0; row < dimsize; row = row + dimsize){
    //        for (auto col = row; col < row + dimsize; col++) {
    //            rowvec.push_back(elbuf[col]);
    //        }
    //     Matrix::elements.push_back(rowvec);
    //     rowvec.clear();

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
// void main(int argc, char* argv[]){

// };