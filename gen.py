import numpy as np

mat = np.random.randint(-3, 3, size=(65,65))
mat2 = np.random.randint(-3, 3, size=(65,65))
with open('angelikatest1.txt', 'w') as f:
    for i in range(mat.shape[0]):
        for j in range(mat.shape[1]):
            f.write('{}\n'.format(mat[i][j]))
            f.write('{}\n'.format(mat2[i][j]))