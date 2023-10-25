#include <iostream>
#include <fstream>
#include <cassert>
#include <algorithm>
#include "Hopfield_model.hpp"

int main()
{
    srand(time(NULL));

    int N_quad = 625; // number of neurons

    double Corruzione = 0;
    std::cout << "percentuale di corruzione:" << '\n';
    std::cin >> Corruzione;
    assert(Corruzione >= 0 && Corruzione <= 1);

    double Temp = 0.2; // temperature
    int N_pattern = 20;
    std::fstream file_FA;

    Hopfield_Network hopfield(N_quad, Temp); // network creation

    // I add a memories to the network and check the weight distribution. I reiterat it many times

    //for (int k = 1; k < 0.2 * N_quad; k++)
    for (int j = 1; 100; j++) {
    for (int k = 1; N_pattern; k++) 
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
    } // repeated generation 100 times 

        char filename[100];
        sprintf(filename, "Weight_Distribution/N_625/Pattern_%d.txt");
        file_FA.open(filename, std::fstream::out);
        file_FA << "peso" << '\t' << "valore" << '\n';
        for (int i = 0; i < hopfield.Get_Weights_Size(); i++)
        {
            file_FA << i << ';' << hopfield.Get_Weight(i) << '\n';  
        }
        file_FA.close();
}