import matplotlib.pyplot as plt
import numpy as np
from scipy.optimize import curve_fit

# Leggi il file con due colonne separate da tabulazioni
with open("histograms.csv", "r") as file:
    data = [line.strip().split("\t") for line in file]

# Estrai le colonne in due liste separate
bin_indices = [int(row[0]) for row in data]
bin_entries = [int(row[1]) for row in data]

# Crea un istogramma
plt.bar(bin_indices, bin_entries, width=1.0, color="blue")

# Aggiungi etichette
plt.xlabel("Bin Index")
plt.ylabel("Entries")
plt.title("Histogram")

# Definisci la funzione gaussiana per il fit
def gaussian(x, A, mu, sigma):
    return A * np.exp(-((x - mu) / (2 * sigma))**2)

# Fai il fit gaussiano
bin_centers = np.array(bin_indices)
bin_counts = np.array(bin_entries)
popt, _ = curve_fit(gaussian, bin_centers, bin_counts, p0=[1, np.mean(bin_centers), np.std(bin_centers)])

# Estrai i parametri del fit
A_fit, mu_fit, sigma_fit = popt

# Calcola la curva di fit gaussiana
x_fit = np.linspace(min(bin_indices), max(bin_indices), 1000)
y_fit = gaussian(x_fit, A_fit, mu_fit, sigma_fit)

# Disegna la curva di fit gaussiana
plt.plot(x_fit, y_fit, 'r-', label="Fit (Gaussian)")

# Aggiungi la legenda
plt.legend()

# Mostra il grafico
plt.show()

# Stampa i parametri del fit
print("A_fit:", A_fit)
print("mu_fit:", mu_fit)
print("sigma_fit:", sigma_fit)
