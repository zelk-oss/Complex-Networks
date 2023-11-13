import matplotlib.pyplot as plt
import numpy as np 
import scipy.special as sc
from scipy.optimize import curve_fit

# this script will open the file with entropy points as function of number of patterns 
# and plot the points. 
""" create some arrays """
x_average = np.empty(60)
y_average = np.empty(60)
y_error = np.ones(60)


""" fill the array with the mean value of 5 simulation """
for i in range(1,6):
    fileinput = "entropy_n_patterns_" + str(i) + ".csv"
    file = open(fileinput, 'r')
    line = file.readlines()
    for row in range(0,60):
        x_read = float(line[row].strip().split(",")[0])
        y_read = float(line[row].strip().split(",")[1])
        x_average[row] += x_read
        y_average[row] += y_read
    file.close()

x_average /= 5.
y_average /= 5.

""" compute the errors """
for i in range(1,6):
    fileinput = "entropy_n_patterns_" + str(i) + ".csv"
    file = open(fileinput, 'r')
    line = file.readlines()
    for row in range(0,60):
        y_read = float(line[row].strip().split(",")[1])
        y_error[row] += pow(y_read - y_average[row],2)
    file.close()

y_error = np.sqrt(y_error / 5.)

# analytic 
filename = "Hopfield entropy"
N=400
max_patterns= 69

p = np.arange(10, max_patterns + 1, 1)           #set the number of memories

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

chisq = (((y_average - entropy)/y_error)**2.).sum()
# Print the fit output.
print(chisq/5)

plt.errorbar(x_average, y_average,y_error, marker='o',label="data")
plt.plot(p, entropy,label="theoretical function")
plt.title('Entropy function')
plt.xlabel('Number of pattern')
plt.ylabel('Entropy')
plt.grid(True)
plt.legend()
plt.show()