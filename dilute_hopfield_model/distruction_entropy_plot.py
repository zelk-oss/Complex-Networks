import matplotlib.pyplot as plt
import numpy as np 
import scipy.special as sc
import scipy

# this script will open the file with entropy points as function of number of patterns 
# and plot the points. 
x_values = []
y_values = []

with open('entropy_prob_distruction.csv', 'r') as file:
    for line in file:
        if line.strip():
            x, y = map(float, line.strip().split(","))
            x_values.append(1-x)
            y_values.append(y)
# analytic 

plt.plot(x_values, y_values, marker='o', linestyle='-')
plt.title('Entropy(p)')
plt.xlabel('p')
plt.ylabel('Entropy')
plt.grid(True)
plt.show()