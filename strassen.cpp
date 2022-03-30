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
    int* C;
    Matrix(int dimension, int crossover){
        Matrix::n = dimension; Matrix::cop = crossover;
    }
    ~Matrix() = default;
    // do I need to delete the matrices??

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

    Matrix::A = A; Matrix::B = B; Matrix::C = new int[size];
    // initialize C matrix to zeros
    for (int i = 0; i < size; i++){
        Matrix:C[i] = 0;
    }
    return std::pair<int*, int*>(A, B);
   }

    /* 
        stand_mult
        Standard multiplication algorithm.
        Takes as argument starting pointers to the inside of A and B
        Returns pointer to the product matrix.

        idea: could pass integers only as arguments instead of pointers
        treat these as coordinates for upper left corner of sub-matrix of A
            and upper-left corner of sub-matrix of B
        works because these matrice are equal squares
    */
   int* standard_mult(int dim, bool pad, std::pair<int, int> acoord, 
                        std::pair<int, int> bcoord,
                        std::vector<std::pair<int, std::pair<int, int>>> ccoord_vec){
       // multiply each column in B by every row in A
       // trying to optimize because caches dredge up entire row
       //Matrix::C = new int[dim*dim]; // OPT Q: can I avoid making a ton of these new matrices??
       for (int j = 0; j < dim; j++){ // column in B
            for (int i = 0; i < dim; i++){ // row of A
               // reset C's column element sum to zero
               int partial_sum = 0;
               for (int k = 0; k < dim; k++){ // element of column in B, element of row of A
                  // handle padding here! "pretend" matrix has padding
                  if (pad && (k == dim-1 || i == dim-1 || k == dim-1)){
                      // do nothing to partial_sum, because we assume padding is zero
                  } else { // no padding case
                    partial_sum = partial_sum + A[(i+acoord.first)*dim + (k + acoord.second)]
                        * B[(k + bcoord.first)*dim + (j + bcoord.second)];
                  }
               }
               // fill in element of C's column
               // and all relevant blotches of C
               for (auto ccoord_wrap : ccoord_vec){
                   auto ccoord = ccoord_wrap.second;
                   if (ccoord_wrap.first == -1) {
                        Matrix::C[(i + ccoord.first)*dim + 
                        (j + ccoord.second)] -= partial_sum;
                   } else {
                        Matrix::C[(i + ccoord.first)*dim + 
                        (j + ccoord.second)] += partial_sum;
                   }

               }
           }
       }

       return Matrix::C;
   }


    /*
        strass_mult
        Strassen's algorithm with crossover.
        We consciously edit the matrices in-place for runtime and space purposes.
        Handles padding without explicitly storing it (generates it on-demand)
        Arguments:
            - dimension size
            - pad (boolean for whether or not this matrix should include padding)
            - coordinates of upper-left submatrix of A (if 0,0 means entire matrix A)
            - coordinates of upper-left submatrix of B (if 0,0 means entire matrix B)
            - coordinates of where output should be deposited in C (0,0 means blanket entire C)
    */
   int* strassens_w_crossover(int dim, bool pad, std::pair<int,int> acoord, 
                                std::pair<int,int> bcoord,
                                std::vector<std::pair<int, std::pair<int, int>>> ccoord_vec){
        // messy base case
        // if (dim == 2){ // just dealing with a real number in each quadrant here
        //     Matrix::C[acoord.first*dim + bcoord.second] = Matrix::A[acoord.]
        //     Matrix::C[acoord.first*dim + bcoord.second + 1] = 
        //     Matrix::C[(acoord.first + 1)*dim + bcoord.second] = 
        //     Matrix::C[(acoord.first + 1)*dim + bcoord.second + 1] = 
        // }
        if (dim == 2 || dim <= Matrix::cop){ // invoke normal multiplication, this is "base case"
            return Matrix::standard_mult(dim, pad, acoord, bcoord, ccoord_vec);
        } else {
            if (dim % 2 != 0){
                // need to add padding
                dim++;
                pad = true;
            }
            // do strassen multiplications

            // invoke recursive step
        }

        return Matrix::C;
   }
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