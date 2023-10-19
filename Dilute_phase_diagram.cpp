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

    double Corruzione = 0.2;
    double Temp = 0.2;
    int N_pattern = 0;
    double distruzione = 0;

    std::fstream file_FA;
    for (int l = 0; l < 5; l++)
    {
        char filename[100];
        sprintf(filename, "Diluita_Magnetizzazione/Phase_Diagram_N%d_%d.txt", N_quad, l);

        file_FA.open(filename, std::fstream::out);        // pulisce il file
        file_FA << "Dimensione rete: " << N_quad << '\n'; // stampa su file
        file_FA << "Probabilità distruzione;"
                << "numero di ricordi;"
                << "prodotto scalare finale\n";
        file_FA.close();

        for (int k = 0; k < 26; k++)
        {
            distruzione = k / 25.;
            int Pattern_Max = N_quad * 0.15;
            for (int j = 1; j < Pattern_Max; j++)
            {
                N_pattern = j;
                Hopfield_Network hopfield(N_quad, Temp);

                for (int i = 0; i < N_pattern; i++) // inizializzo
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

                hopfield.Strong_Weight_Destroyer(distruzione); // diluisco il network

                int cicli = 40 * N_quad;
                for (int i = 0; i < cicli; i++) // il network lavora per 30 secondi
                {
                    corrotto = hopfield.Glauber(corrotto);
                }
                double prod_scal = hopfield.Scalar_Product(scelta, corrotto);

                file_FA.open(filename, std::fstream::app);
                file_FA << distruzione << ';' << N_pattern << ';' << prod_scal << '\n';
                file_FA.close();
            }
        }
    }
}