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
    int* scratch_pad; // a matrix we only allocate once for storing
                    // intermediate results
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

        Matrix::A = A; Matrix::B = B;
        if (n % 2 != 0){
            Matrix::C = new int[(Matrix::n + 1) * (Matrix::n + 1)];
            // 10 quad sums per round of recursion
            // there will be 
            Matrix::scratch_pad = new int[((size+1)/2) * ((size+1)/2)];
        } else {
            Matrix::C = new int[size];
            Matrix::scratch_pad = new int[(size/2) * (size/2)];
        }
        // initialize C matrix to zeros
        for (int i = 0; i < size; i++){
            Matrix::C[i] = 0;
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
    int* standard_mult(int dim, bool pad, int* A, int* B, int* C, std::pair<int, int> acoord, 
                        std::pair<int, int> bcoord,
                        std::vector<std::pair<int, std::pair<int, int>>> ccoord_vec){
       // multiply each column in B by every row in A
       // trying to optimize because caches dredge up entire row   
       if (pad){
           dim = dim-1;
       }
       
       for (int i = 0; i < dim; i++){  // row of A
            for (int k = 0; k < dim; k++){ // element of column in B, element of row of A
               // reset C's column element sum to zero
               int partial_sum = 0;
               for (int j = 0; j < dim; j++){  // column in B
                partial_sum += A[(i+acoord.first)*dim + (k + acoord.second)]
                        * B[(k + bcoord.first)*dim + (j + bcoord.second)];
               }
               // fill in element of C's column
               // and all relevant blotches of C
               for (auto ccoord_wrap : ccoord_vec){
                    auto ccoord = ccoord_wrap.second;
                    if (pad){
                        if (ccoord_wrap.first == -1) {
                            C[(i + ccoord.first)*(dim+1) + 
                            (k + ccoord.second)] -= partial_sum;
                        } else {
                            C[(i + ccoord.first)*(dim + 1) + 
                            (k + ccoord.second)] += partial_sum;
                        }
                    } else {
                        if (ccoord_wrap.first == -1) {
                            C[(i + ccoord.first)*dim + 
                            (k + ccoord.second)] -= partial_sum;
                        } else {
                            C[(i + ccoord.first)*dim + 
                            (k + ccoord.second)] += partial_sum;
                        }
                    }
               }
           }
       }

       return C;
    }

   /*
        Subtract or add two submatrices of a larger matrix.
        Takes dimension of the larger matrix, assumes smaller
        matrix dimensions will always be half of that dimension.
        Assume that the dim passed is always an even number/
        Return pointer to resulting matrix.
   */
    int* subtract(int dim, bool pad, int sign, int* A, int* B, int* C,
        std::pair<int,int> acoord, std::pair<int,int> bcoord, 
        std::pair<int,int>ccoord){
        int subdim = dim/2; // dimension of the submatrix
        if (pad){
            dim = dim - 1;
        }
        int coeff = 1;
        if (sign < 0){
            coeff = -1;
        }
        //int* ret = new int[subdim*subdim];
        int subA;
        int subB;
        for (int r = 0; r < subdim; r++){
            for (int c = 0; c < subdim; c++){ // Q: confirm this loop!
                if (pad && acoord.second + c > dim - 1) {
                    subA = 0;
                } else {
                    subA = A[(r+acoord.first)*dim + (c+acoord.second)];
                }
                if (pad && bcoord.second + c > dim - 1){
                    subB = 0;
                } else {
                    subB = B[(r+bcoord.first)*dim + (c+bcoord.second)];
                }
                //Matrix::scratch_pad[r*subdim + c] = subA + coeff * subB;
                C[(r + ccoord.first)*subdim + c + ccoord.second] += subA + coeff * subB;
            }
        }
        //return Matrix::scratch_pad;
        return C;
    }

    void fillZero(int dim, int* matptr){
        for (int i = 0; i < dim*dim; i++){
            matptr[i] = 0;
        }
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
        Returns dimension of compute product matrix and pointer to its upper left corner
    */
   std::pair<int, int*> strassens_w_crossover(int dim, bool pad, int* Aptr, int* Bptr, std::pair<int,int> acoord, 
                                std::pair<int,int> bcoord,
                                std::vector<std::pair<int, std::pair<int, int>>> ccoord_vec){
        // FIXME: maybe base case should be dim == 1
        if (dim == 2 || dim <= Matrix::cop){ // invoke normal multiplication, this is "base case"
            int* Cptr = new int[dim*dim];
            fillZero(dim, Cptr);
            int* res_ptr = Matrix::standard_mult(dim, pad, Aptr, Bptr, Cptr, acoord, bcoord, 
                ccoord_vec);
            return std::pair<int, int*>(dim, res_ptr);
        } else {
            printf("not in base case\n");
            if (dim % 2 != 0){
                // need to add padding
                dim++;
                pad = true;
            }
            // do strassen multiplications
            // define coordinate for each sub-matrix

            //Aptr
            std::pair<int,int> A = std::pair<int,int>(acoord.first, acoord.second);
            std::pair<int,int> B = std::pair<int,int>(acoord.first, acoord.second + dim/2);
            std::pair<int,int> C = std::pair<int,int>(acoord.first + dim/2, acoord.second);
            std::pair<int,int> D = std::pair<int,int>(acoord.first + dim/2, acoord.second + dim/2);
            //Bptr
            std::pair<int,int> E = std::pair<int,int>(bcoord.first, bcoord.second);
            std::pair<int,int> F = std::pair<int,int>(bcoord.first, bcoord.second + dim/2);
            std::pair<int,int> G = std::pair<int,int>(bcoord.first + dim/2, bcoord.second);
            std::pair<int,int> H = std::pair<int,int>(bcoord.first + dim/2, bcoord.second + dim/2);
            
            // OPT idea: create matrix here, need really (overshoot eh) log(n) or log(n+1)
            std::pair<int,int> wholeMat = std::pair<int,int>(0,0);
            int quadarea = (dim/2)*(dim/2);
            printf("computed quad area\n");

            int* Cptr = new int[dim*dim];
            printf("allocated Cptr\n");
            fillZero(dim, Cptr);
            printf("filled Cptr with zeros\n");
            // delete all unnecessary storage matrices that are additive compounds of A/B
            // p1
            int* f_min_h = new int[quadarea];
            fillZero(dim, f_min_h);
            printf("allocated subtraction matrix\n");
            Matrix::subtract(dim, pad, -1, Bptr, Bptr, f_min_h, F, H, wholeMat);
            printf("subtracted f_min_h\n");
            std::vector<std::pair<int, std::pair<int,int>>> p1_coords;
            // for-loop to push back all 
            //p1_coords.push_back(std::pair<int, std::pair<int,int>>(1, B)); // confirm
            //p1_coords.push_back(std::pair<int, std::pair<int,int>>(1, D));
            printf("pushed everything back\n");
            auto p1 = strassens_w_crossover(dim/2, pad, Aptr, f_min_h, A, wholeMat, p1_coords);
            //delete[] f_min_h; // OPT: can we immediately re-purpose the matrix here?
            printf("finished recursive call\n");

            // p2
            int* a_plus_b = new int[quadarea];
            fillZero(dim, a_plus_b);
            Matrix::subtract(dim, pad, 1, Aptr, Aptr, a_plus_b, A, B, wholeMat);
            std::vector<std::pair<int, std::pair<int,int>>> p2_coords;
            p2_coords.push_back(std::pair<int, std::pair<int,int>>(-1, A)); // confirm
            p2_coords.push_back(std::pair<int, std::pair<int,int>>(1, B));
            auto p2 = strassens_w_crossover(dim/2, pad, a_plus_b, Bptr, wholeMat, H, p2_coords);
            //delete[] a_plus_b;
            // p3
            int* c_plus_d = new int[quadarea];
            fillZero(dim, c_plus_d);
            Matrix::subtract(dim, pad, 1, Aptr, Aptr, c_plus_d, C, D, wholeMat);
            std::vector<std::pair<int, std::pair<int,int>>> p3_coords;
            p3_coords.push_back(std::pair<int, std::pair<int,int>>(-1, C)); // confirm
            p3_coords.push_back(std::pair<int, std::pair<int,int>>(1, D));
            auto p3 = strassens_w_crossover(dim/2, pad, c_plus_d, Bptr, wholeMat, E, p3_coords);
            //delete[] c_plus_d; 
            // p4
            int* g_min_e = new int[quadarea];
            fillZero(dim, g_min_e);
            Matrix::subtract(dim, pad, -1, Bptr, Bptr, g_min_e, G, E, wholeMat);
            std::vector<std::pair<int, std::pair<int,int>>> p4_coords;
            p4_coords.push_back(std::pair<int, std::pair<int,int>>(-1, C)); // confirm
            p4_coords.push_back(std::pair<int, std::pair<int,int>>(1, D));
            auto p4 = strassens_w_crossover(dim/2, pad, g_min_e, Bptr, wholeMat, E, p4_coords);
            //delete[] g_min_e; 
            // p5
            int* a_plus_d = new int[quadarea];
            fillZero(dim, a_plus_d);
            int* e_plus_h = new int[quadarea];
            fillZero(dim, e_plus_h);
            Matrix::subtract(dim, pad, 1, Aptr, Aptr, a_plus_d, A, D, wholeMat);
            Matrix::subtract(dim, pad, 1, Bptr, Bptr, e_plus_h, E, H, wholeMat);
            std::vector<std::pair<int, std::pair<int,int>>> p5_coords;
            p5_coords.push_back(std::pair<int, std::pair<int,int>>(-1, C)); // confirm
            p5_coords.push_back(std::pair<int, std::pair<int,int>>(1, D));
            auto p5 = strassens_w_crossover(dim/2, pad, a_plus_d, e_plus_h, wholeMat, wholeMat, p5_coords);
            //delete[] a_plus_d;
            //delete[] e_plus_h; 
            // p6
            int* b_min_d = new int[quadarea];
            fillZero(dim, b_min_d);
            int* g_plus_h = new int[quadarea];
            fillZero(dim, g_plus_h);
            Matrix::subtract(dim, pad, -1, Aptr, Aptr, b_min_d, B, D, wholeMat);
            Matrix::subtract(dim, pad, 1, Bptr, Bptr, g_plus_h, G, H, wholeMat);
            std::vector<std::pair<int, std::pair<int,int>>> p6_coords;
            p6_coords.push_back(std::pair<int, std::pair<int,int>>(-1, C)); // confirm
            p6_coords.push_back(std::pair<int, std::pair<int,int>>(1, D));
            auto p6 = strassens_w_crossover(dim/2, pad, b_min_d, g_plus_h, wholeMat, wholeMat, p6_coords);
            //delete[] b_min_d;
            //delete[] e_plus_h; 
            // p7
            int* c_min_a = new int[quadarea];
            fillZero(dim, c_min_a);
            int* e_plus_f = new int[quadarea];
            fillZero(dim, e_plus_f);
            //printf("e_plus_f allocated\n");
            Matrix::subtract(dim, pad, -1, Aptr, Aptr, c_min_a, C, A, wholeMat);
            Matrix::subtract(dim, pad, 1, Bptr, Bptr, e_plus_f, E, F, wholeMat);
            std::vector<std::pair<int, std::pair<int,int>>> p7_coords;
            p7_coords.push_back(std::pair<int, std::pair<int,int>>(-1, C)); // confirm
            p7_coords.push_back(std::pair<int, std::pair<int,int>>(1, D));
            // put this into specific region of C matrix
            auto p7 = strassens_w_crossover(dim/2, pad, c_min_a, e_plus_f, wholeMat, wholeMat, p7_coords);
            //delete[] c_min_a;
            //delete[] e_plus_f; 
            // invoke recursive step

            //int* q1 = new int[quadarea];
            std::pair<int,int> q1 = std::pair<int,int> (0,0);
            //int* q2 = new int[quadarea];
            std::pair<int,int> q2 = std::pair<int,int> (0, p1.first);
            //int* q3 = new int[quadarea];
            std::pair<int,int> q3 = std::pair<int,int> (p1.first, 0);
            //int* q4 = new int[quadarea];
            std::pair<int,int> q4 = std::pair<int,int> (p1.first, p1.first);

            // first quadrant
            Matrix::subtract(p1.first, pad, -1, p4.second, p2.second, Cptr, wholeMat, wholeMat, q1);
            Matrix::subtract(p1.first, pad, 1, p5.second, p6.second, Cptr, wholeMat, wholeMat, q1);
            // second quadrant
            Matrix::subtract(p1.first, pad, 1, p1.second, p2.second, Cptr, wholeMat, wholeMat, q2);
            // third quadrant
            Matrix::subtract(p1.first, pad, 1, p3.second, p4.second, Cptr, wholeMat, wholeMat, q3);
            // fourth quadrant
            Matrix::subtract(p1.first, pad, -1, p1.second, p3.second, Cptr, wholeMat, wholeMat, q4);
            Matrix::subtract(p1.first, pad, 1, p5.second, p7.second, Cptr, wholeMat, wholeMat, q4);
            // NOTE: need to account for padding down here too!!
            return std::pair<int, int*> (dim, Cptr);
        }
   }
};

/*
    Main function:
    handles command line arguments
        crossover, dimension, inputfile
    prints output in the specified format
*/
// void main(int argc, char* argv[]){
//     char* cross_s = argv[1];
//     int crossover = strtol(cross_s, &cross_s, 10);
//     char* dim_s = argv[2];
//     int dimension = strtol(dim_s, &dim_s, 10);
//     char* inputfile = argv[2];

//     Matrix m(dimension, crossover);
//     m.read_input(inputfile);

//     auto begincoord = std::pair<int,int>(0,0);
//     std::vector<std::pair<int, std::pair<int, int>>> beginccoord;
//     beginccoord.push_back(std::pair<int, std::pair<int,int>>(1, std::pair<int,int>(0,0)));
//     auto finalret = m.strassens_w_crossover(dimension, false, m.A, m.B, 
//         begincoord, begincoord,
//         beginccoord);
//     for (int i = 0; i < dimension; i++){
//         for (int j = 0; j < dimension; j++){
//             if (i == j){
//                 printf("%d\n", finalret.second[i*dimension + j]);
//             }
//         }
//     }
// };


// PROBLEM HERE IS THAT I WAS CORRUPTING MEMEORY
// TRY FOR TESTING 7 DISTINCT CROSSOVER VALUES
// MOVING TO PYTHON NOW

