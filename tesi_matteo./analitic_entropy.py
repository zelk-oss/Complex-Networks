import numpy as np
import math
import matplotlib.pyplot as plt
import scipy.special as sc

filename = "Hopfield entropy"
N=400
max_patterns= 400

p = np.arange(0, max_patterns + 1, 1)

def entropy_sum(N_pattern):
    entropy_vector = np.zeros(max_patterns + 1)
    for x in N_pattern:
        sum = 0
        for a in range(-x,x):
            combin = sc.comb(x, (a+x)/2)
            power = pow(1/2.,x)
            logartim = np.log(combin*power)
            sum = sum + combin*power*logartim
        entropy_vector[x] = sum
    return entropy_vector  

entropy = - N*(N-1)/2 * entropy_sum(p)
plt.plot(p, entropy)
plt.title("Hopfield's entropy S")
plt.xlabel("p", fontsize=12)
plt.ylabel("S", fontsize=12)
plt.show()