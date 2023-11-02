// this program produces the file entropy_n_patterns.csv
// aim: get entropy points as function of number of patterns

#include "tesi_matteo/Hopfield_model.hpp"
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

int main() {

  auto start = std::chrono::high_resolution_clock::now();
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
  // open entropy file and begin the loop
  file_entropy.open(filename_entropy, std::fstream::app);

  /******************************************
   * BIG LOOP BEGINS
   * ***************************************/
  for (int N_pattern = 10; N_pattern < 50; N_pattern++) {
    // clean vectors from previous iteration
    for (int i = 0; i < 2 * N_pattern + 1; i++) {
      for (int j = 0; j < histograms.size(); j++) {
        histograms[j].clear();           // Clear the vector
        histograms[j].resize(N_quad, 0); // Resize and initialize with zeros
      }
    }
    /********************
     *creation of entropy repository
     ********************/
    for (int i = 0; i < 2 * N_pattern + 1; i++) {
      for (int j = 0; j < entropies.size(); j++) {
        entropies[j].clear();           // Clear the vector
        entropies[j].resize(N_quad, 0); // Resize and initialize with zeros
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
      // I count all the occurences for a single weight
      for (int i = 0; i < histograms.size(); i++) {
        int bin = static_cast<int>(hopfield.Get_Weight(i) * N_quad) + N_pattern;
        ++histograms[i][bin];
      }

    } // repeated generation many times

    /********************
     * find and export the entropy
     *******************/
    for (int row = 0; row < N_quad; row++) // loop over all the rows
    {
      for (int matrix_element = row + 1; matrix_element < N_quad;
           matrix_element++) // loop over the upper triangular matrix (we
                             // cancel the trace and the symmetry)
      {
        for (int hist_bin = 0;
             hist_bin < histograms[row].size(); // k is a bin position
             hist_bin++) // loop over the possible weight outcomes
        {
          if (histograms[row * N_quad + matrix_element][hist_bin] == 0) {
          } // skip probability == 0 cases. we should get rid of nan-s.
          else {
            link_probability =
                histograms[row * N_quad + matrix_element][hist_bin] /
                static_cast<double>(N_generations);
            if (link_probability >= 1) {
              std::cout << "Probability >= 1!!!. Istead = " << link_probability
                        << "\n";
            }
            link_entropy = (-1) * link_probability * std::log(link_probability);
            entropy_Np += link_entropy;
          }
        } // before ending the total loop i need to push stuff inside the file
      }
    }
    file_entropy << N_pattern << "," << entropy_Np << '\n';
    // reset variables to zero before beginning new loop
    link_entropy = 0;
    link_probability = 0;
    entropy_Np = 0;
    std::cout << "Done." << '\n';
  }
  // re-set variables to zero for next cycle.
  file_entropy.close();
  // Record the end time
  auto end = std::chrono::high_resolution_clock::now();

  // Calculate the elapsed time
  std::chrono::duration<double> elapsed = end - start;

  // Print the elapsed time in seconds
  std::cout << "Elapsed time: " << elapsed.count() << " seconds" << std::endl;
}