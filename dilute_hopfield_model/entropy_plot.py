import matplotlib.pyplot as plt
import numpy as np 
import scipy.special as sc
import scipy

# this script will open the file with entropy points as function of number of patterns 
# and plot the points. 
x_values = []
y_values = []

with open('simulazione_40pti.txt', 'r') as file:
    for line in file:
        if line.strip():
            x, y = map(float, line.strip().split(","))
            x_values.append(x)
            y_values.append(y)
# analytic 
filename = "Hopfield entropy"
N=400
max_patterns= 70

p = np.arange(10, max_patterns + 1, 1)           #set the number of memories
print(len(p))

def entropy_sum(N_pattern):                     #it does the sum
    entropy_vector = np.zeros(max_patterns + 1 -10) 
    for x in N_pattern:
        sum = 0
        for a in range(-x,x+1,2):               #the +1 is needed to take also the x value
            combin = sc.comb(x, (a+x)/2)        #solve the combinatory coefficient
            power = pow(1/2.,x)                 #solve the power
            logartim = np.log(combin*power)     #solve the logarithm
            sum = sum + combin*power*logartim   #combine the previous part
        entropy_vector[x-10] = sum                 #do the sum 
    return entropy_vector  

entropy = - N*(N-1)/2 * entropy_sum(p)
print(entropy)

plt.plot(x_values, y_values, marker='o', linestyle='-')
plt.plot(p, entropy)
plt.title('Entropy(p)')
plt.xlabel('p')
plt.ylabel('Entropy')
plt.grid(True)
plt.show()