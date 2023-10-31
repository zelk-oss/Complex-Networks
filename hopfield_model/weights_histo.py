import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

def gaussian(x, A, mu, sigma):
    return A * np.exp(-(x - mu)**2 / (2 * sigma**2))

bin_values = []
entries = []

histograms = []

figure_counter = 1

with open('hist_for_gauss.csv', 'r') as file:
    for line in file:
        if line.strip():  # Ignora le linee vuote
            bin_value, entry = map(int, line.strip().split(','))

            if bin_value == 0:
                # Inizia un nuovo istogramma
                if bin_values:
                    histograms.append((bin_values, entries))
                    bin_values = []
                    entries = []

            bin_values.append(bin_value)
            entries.append(entry)

histograms.append((bin_values, entries))

# Fai il fit gaussiano e disegna gli istogrammi in figure separate
hist_len = len(histograms)
trial_plots = [1, 1000, 10000, 100000, 1000000]
for i, (bin_values, entries) in enumerate(histograms):
    
    if i in trial_plots: 
        bin_values = np.array(bin_values)
        entries = np.array(entries)

        #popt, _ = curve_fit(gaussian, bin_values, entries, p0=[max(entries), np.mean(bin_values), np.std(bin_values)])

        plt.figure(figure_counter)
        plt.bar(bin_values, entries, alpha=0.5, label=f'Istogramma {i + 1}')

        #x_fit = np.linspace(min(bin_values), max(bin_values), 100)
        #y_fit = gaussian(x_fit, *popt)
        #plt.plot(x_fit, y_fit, label=f'Fit Gaussiano {i + 1}')

        plt.title(f'Istogramma {i + 1} con Fit Gaussiano')
        plt.xlabel('Bin')
        plt.ylabel('Entries')
        plt.legend()
    
        plt.show()
    figure_counter += 1