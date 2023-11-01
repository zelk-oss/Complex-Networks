// this program produces the file entropy_n_patterns.csv
// aim: get entropy points as function of number of patterns

// CREDO CHE ORA FUNZIONI MA VOGLIO CHE FACCIA APPEND SOLO DENTRO AL CICLO NON ANCHE OGNI VOLTA CHE RIESEGUO 

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

  int const N_quad = 100; // number of neurons
  int const N_link = N_quad * N_quad;
  int const N_generations = 100;
  double Temp = 0.2; // temperature
  // int const N_pattern = 20; // number of patter
  double link_probability;
  double link_entropy;
  std::vector<std::vector<int>> histograms(
      N_link); // vector of vectors (histograms) for the different weights. dim
               // = N_link
  // will have to removes symmetric elements and the diagonal later
  std::vector<std::vector<int>> entropies(
      N_link); // vector of entropy values for the different weights
  std::fstream file_entropy;
  double entropy_Np; // entropy as function of patterns number
    
  std::string filename_entropy = "entropy_n_patterns.csv";

  /********************
   * creating histograms
   ********************/
  // iterate over variable N_pattern
  for (int N_pattern = 30; N_pattern < 33; N_pattern++) {
    // clean vectors from previous iteration
    if (!histograms.empty()) {
      histograms.clear();
    }
    if (!entropies.empty()) {
      entropies.clear();
    }

    for (int i = 0; i < 2 * N_pattern + 1; i++) {
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
    for (int i = 0; i < N_quad; i++) // cicle overe all the rows
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
  }
  // re-set variables to zero for next cycle.
  link_entropy = 0;
  link_probability = 0;
  entropy_Np = 0;
}
