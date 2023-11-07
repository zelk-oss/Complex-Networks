import matplotlib.pyplot as plt
import numpy as np 
import scipy.special as sc
import scipy

# this script will open the file with entropy points as function of number of patterns 
# and plot the points. 
x_values = []
y_values = []
z_values = []

with open('all_parameter_entropy.csv', 'r') as file:
    next(file)
    for line in file:
        if line.strip():
            x, y, z = line.strip().split(",")
            x_values.append(float(x))
            y_values.append(1. - float(y)) 
            z_values.append(float(z))

plt.plot(x_values, z_values, marker='o', linestyle='-')
plt.title('Entropy(p)')
plt.xlabel('probabilità di distuzione')
plt.ylabel('Entropy')
plt.grid(True)
plt.show()