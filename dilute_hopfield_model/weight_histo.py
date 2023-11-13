import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
import scipy.stats
from scipy.stats import norm

def gaussian(x, A, mu, sigma):
    return A * np.exp(-(x - mu)**2 / (2 * sigma**2))

bin_values = []
entries = []

histograms = []

figure_counter = 1
counter = 0
with open('hist_for_gauss.csv', 'r') as file:
    for line in file:
        if line.strip():  # Ignora le linee vuote
            bin_value, entry = map(int, line.strip().split(','))

            if bin_value == 0:
                # Inizia un nuovo istogramma
                counter += 1
                if bin_values:
                    histograms.append((bin_values, entries))
                    bin_values = []
                    entries = []

            bin_values.append(bin_value)
            entries.append(entry)

histograms.append((bin_values, entries))

# Fai il fit gaussiano e disegna gli istogrammi in figure separate
n_hist = len(histograms)
trial_plots = [n_hist - 1000, n_hist / 2, n_hist - 100]

for i, (bin_values, entries) in enumerate(histograms):
    
    if i in trial_plots: 
        bin_values = np.array(bin_values)
        entries = np.array(entries)

        popt, pcov = curve_fit(gaussian, bin_values, entries, p0=[max(entries), np.mean(bin_values), np.std(bin_values)])

        plt.figure(figure_counter)
        bin_width = bin_values[1] - bin_values[0]  # Calcola la larghezza del bin

        # Utilizza bin_width come larghezza del bin nella funzione plt.bar
        plt.bar(bin_values, entries, width=bin_width, alpha=0.5, label=f'Istogramma {i + 1}')

        x_fit = np.linspace(min(bin_values), max(bin_values), 100)  # Estendi x_fit sull'intervallo completo
        y_fit = gaussian(x_fit, *popt)

        plt.plot(x_fit, y_fit, label=f'Fit Gaussiano {i + 1}')

        # Calcola il chi-quadro
        #chi_squared = np.sum(((entries - y_fit) ** 2) / y_fit)
        #print("Chi-squared:", chi_squared)

        print("Prefix =", popt[0], "+-", np.sqrt(pcov[0, 0]))
        print("mean =", popt[1], "+-", np.sqrt(pcov[1, 1]))
        print("sigma =", popt[2], "+-", np.sqrt(pcov[2, 2]))
        
        # Print the fit parameters in the legend
        A_fit, mu_fit, sigma_fit = popt
        legend_text = f'Istogramma {i + 1}\nA = {A_fit:.2f}, μ = {mu_fit:.2f}, σ = {sigma_fit:.2f}'
        plt.legend([legend_text], loc='upper right')

        plt.title(f'Istogramma {i + 1} con Fit Gaussiano')
        plt.xlabel('Bin')
        plt.ylabel('Entries')

        plt.grid(True)
        plt.show()
    
    figure_counter += 1
