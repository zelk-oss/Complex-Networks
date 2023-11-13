import matplotlib.pyplot as plt
import numpy as np

from matplotlib import colors
from matplotlib.ticker import PercentFormatter

N_quad = 625
N_generations = 100
hist_data = []

# extract weights from file 
for i in range(1, N_generations + 1):  # from 1 to N_gen (included)
    file_name = f"Weight_Distribution_N_625/Pattern_20_{i}.txt"  
    with open(file_name, 'r') as file:
        for line in file:
            columns = line.split(';')  # Divide la riga in colonne (presumendo uno spazio come delimitatore)
            if len(columns) >= 2:
                data_point = float(columns[1])
                hist_data.append(data_point)  # hist_data will contain second column 
                                              # from all files in a comfy array 
n_links = len(hist_data) # N_generations # 625^2 

# select only the indices of the upper triangular matrix 
def upper_triangular_indices(N_quad):
    indices = []
    for i in range(N_quad):
        for j in range(i+1, N_quad):
            indices.append(i * N_quad + j)
    return indices
#index_matrix = create_index_matrix(N_quad)
weights_indices = upper_triangular_indices(N_quad)


print("Wanna see some nice histograms?")
user_choice = input()
#for link in range(N_quad*N_quad): 
for link in range(5): 
    link_weights = hist_data[link::n_links]   # slicing: split the whole array into single neuron arrays of weights 
    #print(len(neuron_weights))  # should be = N_generations 
    
    # *** graphic rendering 
    # remember to reduce the number of generations 
    if user_choice == "yes": 
        (link_hist, bins, _) = plt.hist(link_weights, bins = 10)  # adjust number of bins
        plt.xlabel('weight')
        plt.ylabel('occurrences')
        plt.title(f'Distribution of weights for neuron {link}')
        plt.show()
    else: 
        pass 


# probability 
# a volte fa una cosa strana e annulla la probabilità nel bin attorno a zero. 
(link_hist, bins) = np.histogram(link_weights, bins = 10)  # adjust number of bins
prob_array = link_hist / N_generations


net_entropy = 0
#print(prob_array)
if (sum(prob_array) > 1.001) or (sum(prob_array) < 0.999): # total prob should be approx 1 
    print("Total prob is not 1 for link ", link)
    print("Intstead = ", sum(prob_array))
for b in range(len(prob_array + 1)):
    if (prob_array[b] != 0): 
        net_entropy += prob_array[b] * np.log(prob_array[b])
print(net_entropy)
# now sum over all links (i < j)

# mi si è sfanculato di nuovo voglio morire. 
#poi capiamo dov'è il problema 
