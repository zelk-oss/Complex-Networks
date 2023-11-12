import matplotlib.pyplot as plt
import numpy as np
from matplotlib.cm import viridis

# Leggi i dati dal file CSV come hai fatto nel tuo codice
N_pattern = []
prob_creation = []
entropies = []

# Definisci gli array di probabilità che ti interessano
#valori_interessanti = [0.0, 0.01, 0.1, 0.3, 0.5, 0.7, 0.90]  # Sostituisci con i valori desiderati
valori_interessanti = [0.01, 0.2, 0.4, 0.6, 0.8, 0.99]

with open('all_parameter_entropy.csv', 'r') as file:
    next(file)
    for line in file:
        row = line.strip().split(",")
        N_pattern.append(float(row[0]))
        prob = round(1. - float(row[1]),2)
        if prob in valori_interessanti:
            prob_creation.append(prob)
            entropies.append(float(row[2]))
# Crea un dizionario per raggruppare le entropie in base a prob_creation
entropies_dict = {}
for p, entropy in zip(prob_creation, entropies):
    if p not in entropies_dict:
        entropies_dict[p] = []
    entropies_dict[p].append(entropy)

# Rimuovi il grafico precedente
plt.clf()

# Seleziona la mappa di colori
cmap = viridis

# Crea un ciclo per plottare i punti per ciascun valore di prob_creation
for i, (p, entropies) in enumerate(entropies_dict.items()):
    N = len(entropies)
    x = np.linspace(1, N, N)  # Crea un array da 1 a N per l'asse x
    color = cmap(i / len(entropies_dict))  # Calcola il colore dalla mappa di colori
    plt.plot(x + 20, entropies, marker='o', label=f'p = {p:.2f}', color=color)

plt.title('Entropy - fixed connection probabilities')
plt.xlabel('Number of patterns')
plt.ylabel('Entropy')
plt.grid(True)
legend =plt.legend(loc='upper right')
legend.get_frame().set_alpha(0.9)
plt.show()

#####################
# inizia nuovo grafico 

# Inizializza le liste per i dati
N_pattern = []
prob_creation = []
entropies = []

with open('all_parameter_entropy.csv', 'r') as file:
    next(file)  # Salta la prima riga (intestazione)
    for line in file:
        row = line.strip().split(",")
        N_pattern.append(float(row[0]))
        prob_creation.append(float(row[1]))
        entropies.append(float(row[2]))

# Crea un dizionario per raggruppare le entropie in base a prob_creation
entropies_dict = {}
for p, entropy in zip(prob_creation, entropies):
    if p not in entropies_dict:
        entropies_dict[p] = []
    entropies_dict[p].append(entropy)

# Rimuovi il grafico precedente
plt.clf()

# Seleziona i valori di probabilità corrispondenti al valore fisso di N_pattern
probs_for_fixed_N_pattern_20 = []
entropies_for_fixed_N_pattern_20 = []
probs_for_fixed_N_pattern_40 = []
entropies_for_fixed_N_pattern_40 = []
probs_for_fixed_N_pattern_60 = []
entropies_for_fixed_N_pattern_60 = []

for i, N in enumerate(N_pattern):
    if N == 21:
        if (i%2 == 0): 
            probs_for_fixed_N_pattern_20.append(1-prob_creation[i])
            entropies_for_fixed_N_pattern_20.append(entropies[i])

for i, N in enumerate(N_pattern):
    if N == 43:
        if (i%2 == 0): 
            probs_for_fixed_N_pattern_40.append(1-prob_creation[i])
            entropies_for_fixed_N_pattern_40.append(entropies[i])

for i, N in enumerate(N_pattern):
    if N == 65:
        if (i%2 == 0): 
            probs_for_fixed_N_pattern_60.append(1-prob_creation[i])
            entropies_for_fixed_N_pattern_60.append(entropies[i])
# Rimuovi il grafico precedente
plt.clf()

# Crea un plot dell'entropia in funzione di tutti i valori della probabilità per N_pattern fisso
plt.plot(probs_for_fixed_N_pattern_20, entropies_for_fixed_N_pattern_20, marker='o', linestyle='-', label=f'N_pattern = {20}', color = '#22A884FF')
plt.plot(probs_for_fixed_N_pattern_40, entropies_for_fixed_N_pattern_40, marker='o', linestyle='-', label=f'N_pattern = {41}', color = '#7AD151FF')
plt.plot(probs_for_fixed_N_pattern_60, entropies_for_fixed_N_pattern_60, marker='o', linestyle='-', label=f'N_pattern = {60}', color = '#FDE725FF')

plt.title(f'Entropy function - fixed number of patterns')
plt.xlabel('Connection probability')
plt.ylabel('Entropy')
plt.grid(True)
plt.legend()
plt.show()
