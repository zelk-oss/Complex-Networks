#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "Hopfield_model.hpp"

int main()
{

    /********************
     * inizialization of the parameter
     *******************/

    srand(time(NULL));

    int const N_quad = 400; // number of neurons
    int const N_link = N_quad * N_quad;
    double Temp = 0.2;        // temperature
    int const N_pattern = 20; // number of patter
    double link_probability;
    double link_entropy;
    std::vector<std::vector<int>> histograms(N_link); // vector of histograms for the different weight
    std::vector<std::vector<int>> entropies(N_link);  // vector of histograms for the different weight
    std::fstream file_histo;
    std::fstream file_entropy;

    /********************
     * creating histograms
     ********************/

    for (int i = 0; i < 2 * N_pattern + 1; i++)
    { // inizialization of the single histogram with fixed bin
        for (int j = 0; j < histograms.size(); j++)
        {
            histograms[j].push_back(0);
        }
    }
    std::string filename_histo = "histogram.csv"; // create the output file
    file_histo.open(filename_histo, std::fstream::out);
    file_histo << "bin" << '\t' << "occurrences" << '\n'; // first raw of the file.
    file_histo.close();

    /********************
     *creation entropy repository
     ********************/

    for (int i = 0; i < 2 * N_pattern + 1; i++)
    { // inizialization of the single histogram with fixed bin
        for (int j = 0; j < entropies.size(); j++)
        {
            entropies[j].push_back(0);
        }
    }
    std::string filename_entropy = "entropy.csv";
    file_histo.open(filename_entropy, std::fstream::out);
    file_histo << "bin" << '\t' << "entropy" << '\n'; // first raw of the file.
    file_histo.close();

    /********************
     * I create a hopfield network, teach it N_pattern random genareted memories and count the occurence of the weight
     * I repeat it 100 times
     *******************/

    for (int a = 1; a <= 1000; a++) // set number of generations
    {
        Hopfield_Network hopfield(N_quad, Temp); // network creation

        // I add a memories to the network and check the weight distribution. I reiterat it many times

        for (int k = 0; k < N_pattern; k++) // the network will learn N_pattern memories
        {
            std::vector<int> pattern(N_quad, +1);
            for (int j = 0; j < pattern.size(); j++) // creation of a random pattern
            {
                double caso = (double)rand() / RAND_MAX;
                if (caso < 0.5) // p(+1) = 1/2
                {
                    pattern[j] = -1;
                }
                else
                {
                    pattern[j] = +1;
                }
            }
            hopfield.AddPattern(pattern);
        } // generated N_pattern memories
        // I count all the occurencies for a singles weight
        for (int i = 0; i < histograms.size(); i++)
        {
            int bin = static_cast<int>(hopfield.Get_Weight(i) * N_quad) + N_pattern;
            ++histograms[i][bin];
        }
        std::cout << "finish " << a << " iteration\n";

    } // repeated generation 1000 times

    /********************
     *  I export the data
     *******************/

    file_histo.open(filename_histo, std::fstream::app);
    for (int i = 0; i < histograms.size(); i++) // cicle over all the weight
    {
        for (int j = 0; j < histograms[i].size(); j++) // cicle over all the possible outcome
        {
            file_histo << j << '\t' << histograms[i][j] << std::endl; // print the value
        }
        file_histo << '\n';
    }
    file_histo.close();

    /********************
     * find and export the entropy
     *******************/

    file_entropy.open(filename_entropy, std::fstream::app);
    for (int i = 0; i < N_link; i++) // cicle overe all the raw
    {
        for (int j = i + 1; j < N_link;) // cicle over the upper triangular matrix (we cancel the trace and the symmetry)
        {
            for (int k = 0; k < histograms[i].size(); k++) // cicle over the possible outcome
            {
                link_probability = histograms[i * N_link + j][k] / 100.;      // find the probability distribution
                link_entropy = link_probability * std::log(link_probability); // find the entropy per outcome per link
                file_entropy << j << '\t' << link_entropy << '\n';            // print the value
            }
        }
    }
    file_entropy.close();
}