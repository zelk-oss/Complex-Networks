import matplotlib.pyplot as plt
import numpy as np

from matplotlib import colors
from matplotlib.ticker import PercentFormatter

N_generations = 100
hist_data = []

for i in N_generations: # run over all weights matrix generations 
    file_path = "data%d.txt"
    try:
        weights = []
        with open(file_path, "r") as file:
            for line in file:
                columns = line.strip().split('\t')

                if len(columns) >= 2:
                    weights.append(columns[1])

        print("Weights in generation x:")
        print(weights)

    except FileNotFoundError:
        print(f"The file '{file_path}' was not found.")
    except Exception as e:
        print(f"An error occurred: {str(e)}")
    
    hist_data = np.add(hist_data, weights) # weights are summed up in here 

# visualize 
plt.bar(range(len(hist_data)), hist_data, tick_label=[str(i) for i in data])

plt.xlabel('unit')
plt.ylabel('weights sum')
plt.title('Weights distribution with binomial stuff')

plt.show()