#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "Hopfield_model.hpp"

int main()
{
    srand(time(NULL));

    /*******************
     *Inizialization of the parameter
     *******************/
    int const N_quad = 200; // number of neurons
    int const N_link = N_quad * N_quad;
    int const N_generations = 100;
    double Temp = 0.2; // temperature
    int const N_pattern = 20;
    std::fstream file_entropy;

    /********************
     * Creating histograms repository
     ********************/
    std::string filename_entropy = "entropy_prob_distruction.csv"; // create the output file
    file_entropy.open(filename_entropy, std::fstream::out);
    file_entropy << "bin, occurrences" << '\n'; // first row of the file.
    file_entropy.close();

    for (int distruction = 0; distruction < 100; distruction++) // I cycle over the possible prob of cancel a link from 0 to 0.99
    {
        double prob_distruction = distruction / 100.; // prob of cancel a link
        double link_probability = 0;
        double link_entropy = 0;
        double entropy_Np = 0;
        std::vector<int> histograms(N_link * (2 * N_pattern + 1)); // vector (histograms) for the different weights.

        /********************
         * I create a hopfield network, teach it N_pattern random genareted memories
         * and count the occurence of the weights I repeat this procedure N_generations
         * times
         ********************/
        for (int a = 1; a <= N_generations; a++) // set number of generations
        {
            Hopfield_Network hopfield(N_quad, Temp); // network creation

            // I add memories to the network and check the weight distribution. I
            // reiterate this many times

            for (int k = 0; k < N_pattern; k++) // the network will learn N_pattern memories
            {
                std::vector<int> pattern(N_quad, +1);    // N_quad elements initialized at 1
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

            hopfield.Strong_Weight_Destroyer(prob_distruction); // I dilute the network

            // I count all the occurencies for a singles weight
            for (int i = 0; i < N_link; i++)
            {
                int bin = static_cast<int>(hopfield.Get_Weight(i) * N_quad) + N_pattern;
                ++histograms[bin + i * (2 * N_pattern + 1)];
            }

        } // repeated generation many times

        /* *******************
         *  Calculate and export the entropies
         ********************/
        for (int i = 0; i < N_quad; i++) // cicle over all the rows
        {
            for (int j = i + 1; j < N_quad; j++) // cicle over the columns
            {
                for (int k = 0; k < (2 * N_pattern + 1); k++) // cicle over all the possible outcomes for one weight
                {
                    if (histograms[(i * N_quad + j) * (2 * N_pattern + 1) + k] != 0)
                    {
                        link_probability = histograms[(i * N_quad + j) * (2 * N_pattern + 1) + k] / static_cast<double>(N_generations);
                        link_entropy = (-1) * link_probability * std::log(link_probability); // find the entropy per outcome per link
                        // file_entropy << k << ',' << link_entropy << '\n';
                        entropy_Np += link_entropy;
                    }
                }
            }
        }
        file_entropy.open(filename_entropy, std::fstream::app);
        file_entropy << prob_distruction << ',' << entropy_Np << '\n';
        file_entropy.close();
        std::cout << prob_distruction << "ciclo finito\n";
    }
}
