import sys
import numpy as np
import matplotlib.pyplot as pyplot
import time

def conventional(i, j):
    #check for two nxn
    if (i.shape[0] != i.shape[1]) or (j.shape[0] != j.shape[1]) or (i.shape[0] != j.shape[0]):
        print("wrong size")
        return 0
    
    n = i.shape[0]
    i = i.tolist()
    j = j.tolist()
    c = np.zeroes((n,n)).tolist()
    
    for row in range(n):
        for col in range(n):
            for val in range(n):
                c[row][col] += i[row][val] * j[val][col]
                
    return np.array(c)


#strassen for multiplying two nxn matrices
def strassen(i, j, n0):
    #check for two nxn
    if (i.shape[0] != i.shape[1]) or (j.shape[0] != j.shape[1]) or (i.shape[0] != j.shape[0]):
        print("wrong size")
        return 0
    
    if i.size == 1 and j.size == 1:
        return i[0][0] * j[0][0]
    
    n = i.shape[0]

    if n == n0:
        return conventional(i,j)




    # if n is odd, pad with column of zeroes.
    if (n%2 == 1):
        zeroes = np.zeros((n,1))
        i = np.concatenate((i,zeroes), axis = 1)
        j = np.concatenate((j,zeroes), axis = 1)
        new_n = n + 1
        
    half = new_n/2
    
    #split into submatrices
    #for matrix i
    a = i[:half, :half]
    b = i[:half, half:]
    c = i[half:, :half]
    d = i[half:, half:]
    
    #for matrix j
    e = j[:half, :half]
    f = j[:half, half:]
    g = j[half:, :half]
    h = j[half:, half:]
    
   """  #strassen
    p1 = strassen(a, np.subtract(f,h))
    p2 = strassen(np.add(a,b), h)
    p3 = strassen(np.add(c,d), e)
    p4 = strassen(d, np.subtract(g,e))
    p5 = strassen(np.add(a,d), np.add(e,h))
    p6 = strassen(np.subtract(b,d), np.add(g,h))
    p7 = strassen(np.subtract(c,a), np.add(e,f)) """


    #crossover point
    #strassen
    p1 = strassen(a, np.subtract(f,h), n0)
    p2 = strassen(np.add(a,b), h, n0)
    p3 = strassen(np.add(c,d), e, n0)
    p4 = strassen(d, np.subtract(g,e), n0)
    p5 = strassen(np.add(a,d), np.add(e,h), n0)
    p6 = strassen(np.subtract(b,d), np.add(g,h),n0)
    p7 = strassen(np.subtract(c,a), np.add(e,f),n0)
    
    #return nxn size matrix
    c = np.zeroes((new_n,new_n))
    c[:half, :half] = np.add(np.subtract(p4,p2),np.add(p5,p6))
    c[:half, half:] = np.add(p1,p2)
    c[half:, :half] = np.add(p3,p4)
    c[half:, half:] = np.add(np.subtract(p1,p3), np.add(p5,p7))
    
    unpadded = c[:n, :n]
    
    return unpadded 


if __name__ == "__main__":

    mat1 = sys.argv[1]
    mat2 = sys.argv[2]
    #take in matrix
    i = np.loadtxt(mat1)
    j = np.loadtxt(mat2)

    #test different crossover points
    n0s = []
    times = []
    for n0 in range(50):
        n0s.append(n0)

        start = time.time()
        strassen(i,j, n0)
        end = time.time()
        times.append(end-start)

    plt.plot(n0s, times, "o")
    plt.xlabel("Crossover point")
    plt.ylabel("Time Taken")
    plt.show()


