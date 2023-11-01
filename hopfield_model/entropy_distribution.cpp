// this program produces the file entropy_n_patterns.csv
// aim: get entropy points as function of number of patterns

// CREDO CHE ORA FUNZIONI MA VOGLIO CHE FACCIA APPEND SOLO DENTRO AL CICLO NON
// ANCHE OGNI VOLTA CHE RIESEGUO

#include "tesi_matteo/Hopfield_model.hpp"
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

int main() {

  /********************
   * inizialization of the parameter
   *******************/

  srand(time(NULL));

  int const N_quad = 400; // number of neurons
  int const N_link = N_quad * N_quad;
  int const N_generations = 1000;
  double Temp = 0.2; // temperature
  // int const N_pattern = 20; // number of patter
  double link_probability;
  double link_entropy;
  std::vector<std::vector<int>> histograms(
      N_link); // vector of vectors (histograms) for the different weights. dim
               // = N_link
  // will have to removes symmetric elements and the diagonal later
  std::vector<std::vector<int>> entropies(
      N_link);       // vector of entropy values for the different weights
  double entropy_Np; // entropy as function of patterns number
  std::fstream file_entropy;

  std::string filename_entropy = "entropy_n_patterns.csv";
  // empty file if not clean already
  file_entropy.open(
      filename_entropy,
      std::ios::in |
          std::ios::out); // Apri il file in modalità lettura/scrittura

  if (file_entropy.is_open()) {
    // Verifica se il file non è vuoto
    file_entropy.seekg(0, std::ios::end);
    if (file_entropy.tellg() > 0) {
      // Riporta la posizione del puntatore all'inizio del file
      file_entropy.seekg(0);
      // Svuota il file sovrascrivendolo con un file vuoto
      file_entropy.close();
      file_entropy.open(filename_entropy, std::ios::out | std::ios::trunc);
      if (file_entropy.is_open()) {
        std::cout << "File has been emptied." << std::endl;
      } else {
        std::cerr << "Impossible to open file in writing mode." << std::endl;
      }
    } else {
      std::cout << "File has been emptied." << std::endl;
    }
    file_entropy.close();
  } else {
    std::cerr << "Impossible to open file." << std::endl;
  }

  /******************************************
   * BIG LOOP BEGINS
   * ***************************************/
  for (int N_pattern = 10; N_pattern < 60; N_pattern++) {
    // clean vectors from previous iteration
    if (!histograms.empty()) {
      std::vector<std::vector<int>> histograms(N_link);
    }
    if (!entropies.empty()) {
      std::vector<std::vector<int>> entropies(N_link);
    }

    for (int i = 0; i < 2 * N_pattern + 1;
         i++) // max weight = p patterns, minimum = -p. 2N_p + 1 = length
    { // inizialization of the single histogram with fixed bin: set number of
      // bins
      for (int j = 0; j < histograms.size(); j++) {
        histograms[j].push_back(0);
      }
    }
    // produces N_link lines of 41 0s. so we are ready to fill them up.

    /********************
     *creation of entropy repository
     ********************/

    for (int i = 0; i < 2 * N_pattern + 1;
         i++) { // inizialization of the single histogram with fixed bin
      for (int j = 0; j < entropies.size(); j++) {
        entropies[j].push_back(0);
      }
    }
    for (int i = 0; i < 2 * N_pattern + 1; i++) {
      for (int j = 0; j < histograms.size(); j++) {
        histograms[j].push_back(0);
      }
    }

    /********************
     *creation of entropy repository
     ********************/

    for (int i = 0; i < 2 * N_pattern + 1; i++) {
      for (int j = 0; j < entropies.size(); j++) {
        entropies[j].push_back(0);
      }
    }

    /********************
     * I create a hopfield network, teach it N_pattern random genareted memories
     *and count the occurence of the weights I repeat this procedure
     *N_generations times
     *******************/
    for (int a = 1; a <= N_generations; a++) // set number of generations
    {
      Hopfield_Network hopfield(N_quad, Temp); // network creation

      // I add memories to the network and check the weight distribution. I
      // reiterate this many times

      for (int k = 0; k < N_pattern;
           k++) // the network will learn N_pattern memories
      {
        std::vector<int> pattern(N_quad,
                                 +1); // N_quad elements initialized at 1
        for (int j = 0; j < pattern.size(); j++) // creation of a random pattern
        {
          double caso = (double)rand() / RAND_MAX;
          if (caso < 0.5) // p(+1) = 1/2
          {
            pattern[j] = -1;
          } else {
            pattern[j] = +1;
          }
        }
        hopfield.AddPattern(pattern);
      } // generated N_pattern memories
      // I count all the occurencies for a singles weight
      for (int i = 0; i < histograms.size(); i++) {
        int bin = static_cast<int>(hopfield.Get_Weight(i) * N_quad) + N_pattern;
        ++histograms[i][bin];
      }

    } // repeated generation many times

    /********************
     * find and export the entropy
     *******************/

    file_entropy.open(filename_entropy, std::fstream::app);
    for (int i = 0; i < N_quad; i++) // cicle over all the rows
    {
      for (int j = i + 1; j < N_quad;
           j++) // cicle over the upper triangular matrix (we cancel the trace
                // and the symmetry)
      {
        for (int k = 0; k < histograms[i].size();
             k++) // cicle over the possible weight outcomes
        {
          if (histograms[i * N_quad + j][k] == 0) {
          } // skip probability == 0 cases. we should get rid of nan-s.
          else {
            link_probability = histograms[i * N_quad + j][k] /
                               static_cast<double>(N_generations);
            link_entropy =
                (-1) * link_probability *
                std::log(
                    link_probability); // find the entropy per outcome per link

            entropy_Np += link_entropy;
          }
        } // before ending the total loop i need to push stuff inside the file
      }
    }
    file_entropy << N_pattern << "," << entropy_Np << '\n';
    file_entropy.close();
    // reset variables to zero before beginning new loop 
    link_entropy = 0;
    link_probability = 0;
    entropy_Np = 0;
    std::cout << "Done." <<'\n'; 
  }
  // re-set variables to zero for next cycle.
}
