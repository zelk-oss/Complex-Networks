#include <iostream>
#include <fstream>
#include <cmath>
#include <cassert>
#include "Hopfield_model.hpp"

int main()
{
    srand(time(NULL));

    int N_dim = 0;
    std::cout << "dimensione rete di Hopfield:" << '\n';
    std::cin >> N_dim;
    assert(N_dim > 0);
    int N_quad = N_dim * N_dim;

    double Temp = 0.1;
    double Corruzione = 0.5;
    int N_pattern = 9;
    double distruzione = 0;
    std::fstream file_FA;

    for (int k = 0; k < 100; k++)
    {
        distruzione = (double)k / 1000 + 0.900;

        char filename[100];
        sprintf(filename, "Diluita_Magnetizzazione/Attractor_basin_N625_T_0.1_clustered/Attractor_basin_rho%f_T_0.1.txt", distruzione);
        file_FA.open(filename, std::fstream::out);
        file_FA << "Dimensione rete: " << N_quad << '\n'; // stampa su file
        file_FA << "Distruzione:\tCorruzione:\tprod_Scal:\n";
        file_FA.close();

        Hopfield_Network hopfield(N_quad, Temp);
        for (int i = 0; i < N_pattern - 3; i++) // memorizza i ricordi casuali
        {
            std::vector<int> pattern(N_quad, +1);
            for (int j = 0; j < pattern.size(); j++)
            {
                double caso = (double)rand() / RAND_MAX;
                if (caso < 0.5)
                {
                    pattern[j] = -1;
                }
                else
                {
                    pattern[j] = +1;
                }
            }
            hopfield.AddPattern(pattern);
        }

        int vicini_vicini = rand() % (N_pattern - 3); // scelgo il ricordo da corrompere
        std::vector<int> vicini_vicini_v = hopfield.Get_Pattern(vicini_vicini);
        for (int i = 0; i < 3; i++)
        {
            std::vector<int> pattern = vicini_vicini_v;
            for (int j = 0; j < pattern.size(); j++)
            {
                double caso = (double)rand() / RAND_MAX;
                if (caso < 0.3)
                {
                    pattern[j] = -pattern[j];
                }
            }
            hopfield.AddPattern(pattern);
        }

        hopfield.Strong_Weight_Destroyer(distruzione); // diluisco il network

        for (int i = 0; i < 50; i++)
        {
            Corruzione = 0.50 - (double)(i * 0.01);
            int scelta = rand() % N_pattern; // scelgo il ricordo da corrompere
            std::vector<int> corrotto = hopfield.Get_Pattern(scelta);
            for (int i = 0; i < corrotto.size(); i++) // corrompo il ricordo
            {
                double caso = (double)rand() / RAND_MAX;
                if (caso < Corruzione)
                {
                    corrotto[i] = -corrotto[i];
                }
            }

            int cicli = 50 * N_quad;
            for (int i = 0; i < cicli; i++) // ogni neurone viene visto circa 50 volte
            {
                corrotto = hopfield.Glauber(corrotto);
            }
            double prod_scal = hopfield.Scalar_Product(scelta, corrotto);

            file_FA.open(filename, std::fstream::app);
            file_FA << distruzione << ";" << Corruzione << ";" << prod_scal << '\n';
            file_FA.close();
        }
    }
}