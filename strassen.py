import numpy as np
import math
import argparse
import time

A = None
B = None

def print_diagonal(M):
    for i in range(M.shape[0]):
        for j in range(M.shape[0]):
            if (i == j):
                print(int(M[i][j]))
    # print a trailing newline
    print('')

def subtract(matA, matB, sign):
    dim = int(matA.shape[0])
    # initialize return matrix to all zeros
    retMat = np.zeros((dim, dim))

    # loop optimization!
    for i in range(dim):
        for j in range(dim):
            retMat[i][j] = matA[i][j] + sign * matB[i][j]
    return retMat

def read_input(inputfile, dim):
    alldata = np.loadtxt(inputfile)
    global A
    global B
    A = alldata[:int(alldata.shape[0]/2)]
    B = alldata[int(alldata.shape[0]/2):]
    A = np.reshape(A, (dim, dim))
    B = np.reshape(B, (dim, dim))

def standard_mult(matA, matB):
    '''
    Perform standard matrix multiplication. Spawn a new matrix, don't 
    edit matrices in-place.
    Use loop order to fully optimize.
    '''
    dim = int(matA.shape[0])
    # initialize return matrix to all zeros
    retMat = np.zeros((dim, dim))

    # loop optimization!
    for i in range(dim):
        for k in range(dim):
            for j in range(dim):
                retMat[i][j] += matA[i][k] * matB[k][j]
    return retMat

def strassens(matA, matB, crossover):
    '''
    crossover is an integer, defines when we flip to regular multiplication
    '''
    dim = int(matA.shape[0])
    pad = False
    if dim == 1 or dim < crossover:
        return standard_mult(matA, matB)
    else:
        # compute the fancy strassen stuff
        # handle padding if necessary
        if dim % 2 != 0:
            pad = True
            dim += 1
            #import pdb; pdb.set_trace();
            matA = np.concatenate((matA, np.zeros((matA.shape[0], 1))), axis=1)
            #import pdb; pdb.set_trace();
            matA = np.concatenate((matA, np.zeros((1, matA.shape[1]))), axis=0)
            matB = np.concatenate((matB, np.zeros((matB.shape[0], 1))), axis=1)
            matB = np.concatenate((matB, np.zeros((1, matB.shape[1]))), axis=0)

        quaddim = int(dim/2)
        # find the quadrants
        F = matB[:quaddim, quaddim:]
        H = matB[quaddim:, quaddim:]
        E = matB[:quaddim, :quaddim]
        G = matB[quaddim:, :quaddim]
        A = matA[:quaddim, :quaddim]
        B = matA[:quaddim, quaddim:]
        C = matA[quaddim:, :quaddim]
        D = matA[quaddim:, quaddim:]
        #import pdb; pdb.set_trace();
        # difference matrices --> speedup: try replacing these subtractions
        f_min_h = subtract(F, H, -1)
        a_plus_b = subtract(A, B, 1)
        c_plus_d = subtract(C, D, 1)
        g_min_e = subtract(G, E, -1)
        a_plus_d = subtract(A, D, 1)
        e_plus_h = subtract(E, H, 1)
        b_min_d = subtract(B,D,-1)
        g_plus_h = subtract(G, H, 1)
        c_min_a = subtract(C, A, -1)
        e_plus_f = subtract(E, F, 1)
        # p's
        p1 = strassens(A, f_min_h, crossover)
        p2 = strassens(a_plus_b, H, crossover)
        p3 = strassens(c_plus_d, E, crossover)
        p4 = strassens(D, g_min_e, crossover)
        p5 = strassens(a_plus_d, e_plus_h, crossover)
        p6 = strassens(b_min_d, g_plus_h, crossover)
        p7 = strassens(c_min_a, e_plus_f, crossover)
        # construct final return matrix!
        q1 = subtract(p4, p2, -1)
        q1 = subtract(q1, p5, 1)
        q1 = subtract(q1, p6, 1)

        q2 = subtract(p1, p2, 1)

        q3 = subtract(p3, p4, 1)

        q4 = subtract(p1, p3, -1)
        q4 = subtract(q4, p5, 1)
        q4 = subtract(q4, p7, 1)

        top = np.concatenate((q1,q2), axis=1)
        bottom = np.concatenate((q3,q4), axis=1)
        retMatrix = np.concatenate((top, bottom))

        if pad:
            return retMatrix[:dim-1, :dim-1]
        return retMatrix

if __name__ == "__main__":
    # parse arguments
        # crossover, dimension, inputfile

    parser = argparse.ArgumentParser(description="yey")
    parser.add_argument('crossover', type=int)
    parser.add_argument('dimension', type=int)
    parser.add_argument('inputfile')
    args = parser.parse_args()

    crossover = args.crossover
    if crossover == 0:
        crossover = 35

    read_input(args.inputfile, args.dimension)
    #start = time.time()
    strasout = strassens(A, B, crossover)
    #end = time.time()
    print_diagonal(strasout)
    #print("elapsed time: {}".format(end - start))


# EXPERIMENTAION PLAN
# dimension doesn't have to go above 200
# 5, 10, 20, 40, 80, 160, 320
# try 5 sizes
    # 7 crossover points, 3 trials per crossover point 