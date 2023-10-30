import matplotlib.pyplot as plt

# this script will open the file with entropy points as function of number of patterns 
# and plot the points. 
x_values = []
y_values = []

with open('dati.txt', 'r') as file:
    for line in file:
        if line.strip():
            x, y = map(float, line.strip().split('\t'))
            x_values.append(x)
            y_values.append(y)

plt.plot(x_values, y_values, marker='o', linestyle='-')
plt.title('Entropy(p)')
plt.xlabel('p')
plt.ylabel('Entropy')
plt.grid(True)
plt.show()