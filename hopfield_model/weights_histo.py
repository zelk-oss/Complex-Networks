import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

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

# Fai il fit gaussiano e disegna gli istogrammi nella stessa figura
n_hist = len(histograms)
trial_plots = [0, 699, 6999, 69999]  # Modifica questa lista con gli indici degli istogrammi che desideri plottare

# Imposta la griglia di subplot
rows = 2
cols = 2

colors = ['#000000'] * 70000
indices_to_replace = trial_plots
new_values = ['#414487FF', '#2A788EFF', '#7AD151FF', '#FDE725FF']

for index, new_value in zip(indices_to_replace, new_values):
    colors[index] = new_value

for i, (bin_values, entries) in enumerate(histograms):
    
    if i in trial_plots:  # Assicurati che l'indice sia valido e ci siano colori disponibili
        bin_values = np.array(bin_values)
        entries = np.array(entries)

        popt, pcov = curve_fit(gaussian, bin_values, entries, p0=[max(entries), np.mean(bin_values), np.std(bin_values)])

        # Calcola la posizione del subplot
        plt.subplot(rows, cols, figure_counter)
        
        bin_width = bin_values[1] - bin_values[0]  # Calcola la larghezza del bin

        # Utilizza bin_width come larghezza del bin nella funzione plt.bar
        plt.bar(bin_values, entries, width=bin_width, alpha=0.5, label=f'Istogramma {i + 1}', color=colors[i])

        x_fit = np.linspace(min(bin_values), max(bin_values), 100)  # Estendi x_fit sull'intervallo completo
        y_fit = gaussian(x_fit, *popt)

        plt.plot(x_fit, y_fit, label=f'Fit Gaussiano {i + 1}', color=colors[i])

        # Print the fit parameters in the legend
        A_fit, mu_fit, sigma_fit = popt
        legend_text = f'μ = {mu_fit:.2f}, σ = {sigma_fit:.2f}'
        plt.legend([legend_text], loc='upper right')

        plt.title(f'Weights histogram for link {i + 1}')
        plt.xlabel('Weight bin')
        plt.ylabel('Entries')

        plt.grid(True)
    
        # Incrementa il contatore delle figure
        figure_counter += 1

# Aggiusta la disposizione dei subplot
plt.tight_layout()
plt.show()
