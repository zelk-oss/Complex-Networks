import matplotlib.pyplot as plt
import numpy as np 
import scipy.special as sc
import scipy

# this script will open the file with entropy points as function of number of patterns 
# and plot the points. 
N_pattern = []
prob_creation = []
entropies = []
x = []
y = []

with open('all_parameter_entropy.csv', 'r') as file:
    next(file)
    for line in file:
        row = line.strip().split(",")
        N_pattern.append(float(row[0]))
        prob_creation.append(1. - float(row[1]))
        entropies.append(float(row[2]))

for element in range(len(prob_creation)):
    if round(prob_creation[element],2) == 0.01:
        x.append(N_pattern[element])
        y.append(entropies[element])

plt.plot(x, y, marker='o', linestyle='-')
plt.title('Entropy(p)')
plt.xlabel('N_pattern')
plt.ylabel('Entropy')   
plt.grid(True)
plt.show()