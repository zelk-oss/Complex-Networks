import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

# Funzione gaussiana per il fit
def gaussian(x, A, mu, sigma):
    return A * np.exp(-(x - mu)**2 / (2 * sigma**2))

# Inizializza le liste per gli istogrammi
histograms = []
current_histogram = []

# Leggi il file CSV con tabulazioni come delimitatori
with open('dati.csv', 'r') as file:
    for line in file:
        if line.strip():  # Ignora le linee vuote
            bin_value, entry = map(int, line.strip().split('\t'))

            if bin_value == 0:
                # Inizia un nuovo istogramma
                if current_histogram:
                    histograms.append(current_histogram)
                current_histogram = []

            current_histogram.extend([bin_value] * entry)

# Aggiungi l'ultimo istogramma
histograms.append(current_histogram)

# Fai il fit gaussiano e disegna gli istogrammi
for i, histogram in enumerate(histograms):
    x = np.arange(len(histogram))
    y = np.array(histogram)
    initial_guess = [max(y), np.argmax(y), np.std(y)]

    popt, _ = curve_fit(gaussian, x, y, p0=initial_guess)

    plt.figure()
    plt.bar(x, y, alpha=0.5, label='Istogramma')
    plt.plot(x, gaussian(x, *popt), 'r', label='Fit gaussiano')
    plt.title(f'Istogramma {i + 1}')
    plt.xlabel('Bin')
    plt.ylabel('Entries')
    plt.legend()
    plt.show()
