#include <iostream>
#include <fstream>
#include <cassert>
#include <algorithm>
#include "Hopfield_model.hpp"
#include "autovalori.hpp"

int main()
{
    srand(time(NULL));

    int N_dim = 0;
    std::cout << "dimensione rete di Hopfield:" << '\n';
    std::cin >> N_dim;
    assert(N_dim > 0);
    int N_quad = N_dim * N_dim;

    int N_Pattern = 0;
    std::cout << "numero di pattern casuali:" << '\n';
    std::cin >> N_Pattern;
    assert(N_Pattern > 0);

    double const Temp = 0.2;
    double const corruzione = 0.2;

    double distruzione;

    std::fstream file_FA;

    for (int k= 4; k < 5; k++)
    {
        char filename[100];
        sprintf(filename, "Diluita_Magnetizzazione/9_Pattern/Andamento%d.txt", k);
        file_FA.open(filename, std::fstream::out);
        file_FA << "Dimensione rete: " << N_quad << "\tNumero di ricordi: " << N_Pattern << '\n'; // stampa su file
        file_FA << "Probabilità distruzione\tprodotto scalare finale\n";
        file_FA.close();

        for (int d = 1; d < 100; d++) // inizializzo i ricordi casuali
        {
            distruzione = (double)d / 100.;

            Hopfield_Network hopfield(N_quad, Temp); // creo il network
            for (int i = 0; i < N_Pattern; i++)
            {
                std::vector<int> pattern(N_quad, +1);
                for (int j = 0; j < pattern.size(); j++)
                {
                    int caso = rand() % 101;
                    if (caso < 50)
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

            int scelta = rand() % N_Pattern; // scelgo il ricordo da corrompere
            std::vector<int> corrotto = hopfield.Get_Pattern(scelta);
            for (int i = 0; i < corrotto.size(); i++) // corrompo il ricordo
            {
                double caso = (double)rand() / RAND_MAX;
                if (caso < corruzione)
                {
                    corrotto[i] = -corrotto[i];
                }
            }

            hopfield.Strong_Weight_Destroyer(distruzione); // diluisco il network

            int cicli = 50 * N_quad;
            for (int i = 0; i < cicli; i++) // ogni neurone viene visto circa 50 volte
            {
                corrotto = hopfield.Glauber(corrotto);
            }
            double prod_scal = hopfield.Scalar_Product(scelta, corrotto);

            std::vector<int> laplacian = hopfield.Laplacian();
            std::vector<double> diagonal = diagonal_matrix(laplacian, N_quad);
            std::sort(diagonal.begin(), diagonal.end());

            double cluster = hopfield.Global_Clustering();

            file_FA.open(filename, std::fstream::app);
            file_FA << distruzione << ";" << prod_scal << ";" << diagonal[1] << ";" << cluster << '\n';
            file_FA.close();
        }
    }
}
