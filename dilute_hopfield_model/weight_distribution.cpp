#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "Hopfield_model.hpp"

int main()
{
    /*******************
     * inizialization of the parameter
     *******************/

    srand(time(NULL));

    int const N_quad = 200; // number of neurons
    int const N_link = N_quad * N_quad;
    int const N_generations = 100;
    double Temp = 0.2;        // temperature
    int const N_pattern = 20; // number of patterns
    double link_probability;
    double link_entropy;
    std::vector<int> histograms(N_link * (2 * N_pattern + 1)); // vector (histograms) for the different weights.
    std::fstream file_histo;
}