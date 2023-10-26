import matplotlib.pyplot as plt
import numpy as np

from matplotlib import colors
from matplotlib.ticker import PercentFormatter

N_quad = 625
N_generations = 100
hist_data = []

for i in range(1, N_generations + 1):  # from 1 to N_gen (included)
    file_name = f"Weight_Distribution_N_625/Pattern_20_{i}.txt"  
    with open(file_name, 'r') as file:
        for line in file:
            columns = line.split(';')  # Divide la riga in colonne (presumendo uno spazio come delimitatore)
            if len(columns) >= 2:
                data_point = float(columns[1])
                hist_data.append(data_point)  # hist_data will contain second column 
                                              # from all files in a comfy array 
    
for gen in range(N_generations):
    neuron_weights = hist_data[gen::N_quad]   # slicing: split the whole array into single neuron arrays of weights 
    #print(len(neuron_weights))  # should be = N_generations 
    
    # graphic rendering 
    n_bins = 10
    neuron_hist = plt.hist(neuron_weights, n_bins)  # adjust number of bins
    plt.xlabel('weight')
    plt.ylabel('occurrences')
    plt.title(f'Distribution of weights for neuron {gen}')
    # plt.show()

    # probabilities computation 
    # prob = entries in bin / total occurrences ( = integral)
    # build array with the probabilities 
    prob_array = neuron_hist / N_generations
    # non mi fa fare questa operazioneeeeee

    print(prob_array)