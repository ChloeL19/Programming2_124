#include <vector>
#include <string>
#include <stdio.h>
#include <cstdio>

/*
    A struct for representing a matrix object.
*/
struct Matrix {
    int n; // dimension of the matrix
    std::vector<std::vector<int>> elements; // elements stored as vector of vectors
                                            // each vector is a row
    /*
        Read the numbers from the input file into matrices A and B.
    */
   int read_input(std::string in_file){
       // load everything at once from file 
       // goal is to take advantage of built-in caching optimization in stdio
       int total_size = 2*Matrix::n;
       int elbuf [total_size];
       auto felements = fopen(in_file.c_str(), "r");
       if (felements == NULL){
           return -1;
       }
       int result = fread(elbuf, sizeof(int), total_size, felements);
       if (result < total_size){
           return -1;
       }

       // add padding if necessary
       int dimsize = Matrix::n;
       if (Matrix::n % 2 != 0){ // if odd, add padding
           dimsize++;
       }
       // fill the elements of the matrix in row-major order
       std::vector<int> rowvec;
       for (auto row = 0; row < dimsize; row = row + dimsize){
           for (auto col = row; col < row + dimsize; col++) {
               rowvec.push_back(elbuf[col]);
           }
        Matrix::elements.push_back(rowvec);
        rowvec.clear();
       }

       // delete the buffer memory
       delete[] elbuf; // maybe repetitive, but just to be safe

       return 0;
   }

   /*
        Strassen's algorithm.
        We consciously edit the matrices in-place for runtime and space purposes.
        Takes a pointer to the factor matrices and a pointer to the product matrix.
        Returns pointer to the modified product matrix.
   */

   /* 
        Clears all existing values from the matrix so it can then be re-loaded
        with the read_input function.
   */ 
   int empty_matrix(){
       Matrix::elements.clear();
       return 0;
   }
};

/*
    Main function:
    handles command line arguments
    prints output in the specified format
*/
void main(int argc, char* argv[]){

};