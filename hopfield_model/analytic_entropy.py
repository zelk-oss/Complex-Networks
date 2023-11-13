import numpy as np
import math
import matplotlib.pyplot as plt
import scipy.special as sc

filename = "Hopfield entropy"
N=400
max_patterns= 400

p = np.arange(0, max_patterns + 1, 1)           #set the number of memories

def entropy_sum(N_pattern):                     #it does the sum
    entropy_vector = np.zeros(max_patterns + 1) 
    for x in N_pattern:
        sum = 0
        for a in range(-x,x+1,2):               #the +1 is needed to take also the x value
            combin = sc.comb(x, (a+x)/2)        #solve the combinatory coefficient
            power = pow(1/2.,x)                 #solve the power
            logartim = np.log(combin*power)     #solve the logarithm
            sum = sum + combin*power*logartim   #combine the previous part
        entropy_vector[x] = sum                 #do the sum 
    return entropy_vector  

entropy = - N*(N-1)/2 * entropy_sum(p)
plt.plot(p, entropy, linewidth=3, color='red')
plt.title("Hopfield's entropy analytic behaviour")
plt.xlabel("Stored patterns", fontsize=12)
plt.ylabel("Entropy", fontsize=12)
plt.grid()
plt.show()