#include <iostream>
#include <fstream>
#include <filesystem>
#include <cassert>
#include <algorithm>
#include "Hopfield_model.hpp"

int main()
{
    srand(time(NULL));

    int N_quad = 625; // number of neurons

    double Temp = 0.2; // temperature
    int N_pattern = 20;
    std::fstream file_FA;

    std::filesystem::create_directory("Weight_Distribution_N_625");

    // for (int k = 1; k < 0.2 * N_quad; k++)
    for (int a = 1; a < 10; a++)
    {
        Hopfield_Network hopfield(N_quad, Temp); // network creation
        // I add a memories to the network and check the weight distribution. I reiterat it many times
        for (int k = 1; k < N_pattern; k++) // the network will learn N_pattern memories
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

        // i would like to put generated weights into columns to produce the histo
        // I save all the 100 outcome for just for five differnte weights

        char filename[100];
        sprintf(filename, "Weight_Distribution_N_625/Pattern_%d_%d.txt", N_pattern, a);
        file_FA.open(filename, std::fstream::out);
        file_FA << "indicator" << '\t' << "weight" << '\n'; // first raw of the file.
        file_FA.close();

        file_FA.open(filename, std::fstream::app);
        for (int i = 0; i < hopfield.Get_Weights_Size(); i++)
        {
            file_FA << i << ';' << hopfield.Get_Weight(i) << '\n';
        }
        file_FA.close(); // il file quindi avrà cinque colonne, una per ogni peso
    }                    // repeated generation 100 times
}