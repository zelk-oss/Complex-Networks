#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <cmath>
#include "Hopfield_model.hpp"

int main()
{

    /******************
     * inizialization of the parameter
     ******************/

    srand(time(NULL));

    int const N_quad = 400; // number of neurons
    int const N_link = N_quad * N_quad;
    double Temp = 0.2;       // temperature
    int const N_pattern = 55; // number of patter
    std::fstream file_histo;

    /******************
     * creating histogram
     ******************/

    std::vector<int> histogram(2 * N_pattern + 1); // we know from theory at the weight can have just discrete value
    std::string filename = "histogram.csv";
    file_histo.open(filename, std::fstream::out);
    file_histo << "bin" << '\t' << "occurrences" << '\n'; // first raw of the file.
    file_histo.close();

    /******************
     * I create a hopfield network, teach it N_pattern random genareted memories and count the occurence of the weight
     * I repeat it 100 times
     ******************/

    for (int a = 1; a <= 100; a++) // set number of generations
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

        // I count all the weight
        for (int j = 0; j < hopfield.Get_Weights_Size(); ++j)
        {
            int bin = static_cast<int>((hopfield.Get_Weight(j) * N_quad) + N_pattern);
            ++histogram[bin];
        }

        std::cout << "finish " << a << " iteration\n";

    } // repeated generation 100 times

    /******************
     *  I export the data
     ******************/

    file_histo.open(filename, std::fstream::app);
    for (int i = 0; i < histogram.size(); i++)
    {
        file_histo << i - N_pattern << '\t' << histogram[i] << std::endl;
    }
    file_histo << "-------" << std::endl;
    file_histo.close(); // il file quindi avrà cinque colonne, una per ogni peso
}