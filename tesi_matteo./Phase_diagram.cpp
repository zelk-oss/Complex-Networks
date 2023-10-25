#include <fstream>
#include <cassert>
#include "Hopfield_model.hpp"

// sono qui solo per un commento!!! 
int main()
{
    srand(time(NULL));

    int N_dim = 0;
    std::cout << "dimensione rete di Hopfield:" << '\n';
    std::cin >> N_dim;
    assert(N_dim > 0);
    int N_quadd = N_dim * N_dim;

    double Corruzione = 0;
    std::cout << "percentuale di corruzione:" << '\n';
    std::cin >> Corruzione;
    assert(Corruzione >= 0 && Corruzione <= 1);

    double Temp = 0.2;
    int N_pattern = 0;
    std::fstream file_FA;

    for (int k = 0; k < 10; k++)
    {
        char filename[100];
        sprintf(filename, "Magnetizzazione_critica/N_625/Phase_diagram_%d.txt", k);
        file_FA.open(filename, std::fstream::out);         // pulisce il file
        file_FA << "Dimensione rete: " << N_quadd << '\n'; // stampa su file
        file_FA << "N Pattern:\t overlap:\n";
        file_FA.close();

        for (int j = 1; j < 101; j++)
        {
            N_pattern = j;
            Hopfield_Network hopfield(N_quadd, Temp);

            for (int i = 0; i < N_pattern; i++) // inizializzo
            {
                std::vector<int> pattern(N_quadd, +1);
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

            int scelta = rand() % N_pattern;

            std::vector<int> corrotto = hopfield.Get_Pattern(scelta);

            for (int i = 0; i < corrotto.size(); ++i)
            {
                double caso = (double)rand() / RAND_MAX;
                if (caso < Corruzione)
                {
                    corrotto[i] = -corrotto[i];
                }
            }

            int cicli = 50 * N_quadd;
            for (int i = 0; i < cicli; i++)
            {
                corrotto = hopfield.Glauber(corrotto);
            }

            double prod_scal = hopfield.Scalar_Product(scelta, corrotto);

            file_FA.open(filename, std::fstream::app);
            file_FA << N_pattern << ';' << prod_scal << '\n';
            file_FA.close();
        }
    }
}