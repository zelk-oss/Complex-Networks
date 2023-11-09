import matplotlib.pyplot as plt
import numpy as np 
import scipy.special as sc
import math
from scipy.optimize import curve_fit

# this script will open the file with entropy points as function of number of patterns 
# and plot the points. 
""" create some arrays """
x_values = np.empty(60)
y_values = np.empty(60)
y_error = np.empty(60)

""" fill the array with the mean value of 5 simulation """
for i in range(1,5):
    fileinput = "entropy_n_pattern_" + i + ".csv"
    file = open(fileinput, 'r')
    for line in file:
        x, y = map(float, line.strip().split(","))  
        x_values = x_values + x
        y_values = y_values + y
    file.close()

x_values /= 4
y_values /= 4

"""compute the error """
for i in range(1,5):
    fileinput = "entropy_n_pattern_" + i + ".csv"
    file = open(fileinput, 'r')
    for line in file:
        x, y = map(float, line.strip().split(","))  
        y_error = x_values + pow(y - y_values, 2)
    file.close()

y_error = math.sqrt(y_error / 4)

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

def f(x, N):
    return - N*(N-1)/2 * entropy_sum(x)

entropy = - N*(N-1)/2 * entropy_sum(p)

""" Run a first least-square fit (disregard dx). """
popt, pcov = curve_fit(f, x_values, y_values, (400), y_error)
# Iteratively update the errors and refit.
"""
# l’elemento 0 dell’array è proprio la derivata di cui abbiamo bisogno per combinare gli errori
"""
popt, pcov = curve_fit(f, x, y, popt, y_error)
chisq = (((y - f(x, *popt))/y_error )**2.).sum()
# Print the fit output.
print(popt)
print(np.sqrt(pcov.diagonal()))
print(chisq)

plt.plot(x_values, y_values, marker='o', linestyle='-')
plt.plot(p, entropy)
plt.title('Entropy(p)')
plt.xlabel('p')
plt.ylabel('Entropy')
plt.grid(True)
plt.show()