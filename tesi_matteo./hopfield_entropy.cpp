#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm> // per std::min_element e std::max_element

int main() {
    int N_files = 10; // Numero di file
    int N_quad = 3;   // Intervallo per selezionare gli elementi

    std::vector<std::vector<double>> selectedData(N_quad);

    // Lettura dei file e raccolta dei dati
    for (int fileNum = 1; fileNum <= N_files; ++fileNum) {
        // Costruzione del nome del file
        std::string filename = "Weight_Distribution_N_625/Pattern_20_" + std::to_string(fileNum) + ".txt";
        std::ifstream file(filename);

        if (!file.is_open()) {
            std::cerr << "Impossibile aprire il file: " << filename << std::endl;
            continue;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string firstColumn, secondColumn;
            std::getline(ss, firstColumn, ';');  // Ignora la prima colonna
            if (std::getline(ss, secondColumn, ';')) {
                double value = std::stod(secondColumn);
                selectedData[fileNum % N_quad].push_back(value);
            }
        }
        file.close();
    }

    // Apri un file CSV per l'output
    std::ofstream outputFile("histograms.csv");

    // Creazione e analisi degli istogrammi
    for (const auto& dataVector : selectedData) {
        if (dataVector.empty()) {
            std::cout << "Il vettore è vuoto. Salto l'istogramma." << std::endl;
            continue;
        }

        // Calcola il minimo e il massimo dei dati
        double min = *std::min_element(dataVector.begin(), dataVector.end());
        double max = *std::max_element(dataVector.begin(), dataVector.end());

        int numBins = 10;
        double binSize = (max - min) / numBins;

        std::map<int, int> histogram;

        // Costruzione dell'istogramma
        for (double value : dataVector) {
            int bin = static_cast<int>((value - min) / binSize);
            if (bin >= 0 && bin < numBins) {
                histogram[bin]++;
            }
        }

        // Esporta le occorrenze degli istogrammi
        for (int i = 0; i < numBins; ++i) {
            int occurrences = histogram[i];
            outputFile << i << "\t" << occurrences << std::endl;
        }
        outputFile << "-------" << std::endl;
    }

    outputFile.close();

    return 0;
}
