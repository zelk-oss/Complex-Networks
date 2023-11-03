// this program produces entropy.csv, hist_for_gauss.csv
// aim: see distribution of link probability as function of weights

#include "tesi_matteo/Hopfield_model.hpp"
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

int main()
{

  /********************
   * inizialization of the parameter
   *******************/

  srand(time(NULL));

  int const N_quad = 200; // number of neurons
  int const N_link = N_quad * N_quad;
  int const N_generations = 100;
  double Temp = 0.2;        // temperature
  int const N_pattern = 20; // number of patterns

  std::vector<std::vector<int>> histograms(N_link); // vector of vectors (histograms) for the different weights. dim = N_link
  // will have to removes symmetric elements and the diagonal later
  std::fstream file_histo;
  std::fstream file_gauss;
  std::fstream file_entropy;

  /********************
   * creating histograms
   ********************/

  for (int i = 0; i < 2 * N_pattern + 1; i++) // max weight = p patterns, minimum = -p. 2N_p + 1 = length
  {                                           // inizialization of the single histogram with fixed bin: set number of bins
    for (int j = 0; j < histograms.size(); j++)
    {
      histograms[j].push_back(0);
    }
  } // produces N_link lines of 41 0s. so we are ready to fill them up.

  std::string filename_histo = "histogram.csv"; // create the output file
  file_histo.open(filename_histo, std::fstream::out);
  file_histo << "bin"
             << ","
             << "occurrences" << '\n'; // first row of the file.
  file_histo.close();

  std::string filename_gauss = "hist_for_gauss.csv";
  file_gauss.open(filename_gauss, std::fstream::out);
  file_gauss << "bin, occurrences" << '\n';
  file_gauss.close();

  /********************
   * I create a hopfield network, teach it N_pattern random genareted memories
   * and count the occurence of the weights I repeat this procedure N_generations
   * times
   ********************/
  for (int a = 1; a <= N_generations; a++) // set number of generations
  {
    Hopfield_Network hopfield(N_quad, Temp); // network creation

    // I add memories to the network and check the weight distribution. I
    // reiterate this many times

    for (int k = 0; k < N_pattern; k++) // the network will learn N_pattern memories
    {
      std::vector<int> pattern(N_quad, +1);    // N_quad elements initialized at 1
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
    // I count all the occurencies for a singles weight
    for (int i = 0; i < histograms.size(); i++)
    {
      int bin = static_cast<int>(hopfield.Get_Weight(i) * N_quad) + N_pattern;
      ++histograms[i][bin];
    }

  } // repeated generation many times

  /********************
   *  I export the data
   ********************/
  file_histo.open(filename_histo, std::fstream::out);
  file_gauss.open(filename_gauss, std::fstream::out);

  for (int i = 0; i < N_quad; i++) // cicle over all the rows
  {
    for (int j = i + 1; j < N_quad; j++) // cicle over the columns, just upper triangular histograms
    {
      for (int k = 0; k < histograms[i].size(); k++) // cicle over all the possible outcomes for one weight
      {                                              // note: size of histograms[i] is 41.
        file_histo << k << "," << histograms[i * N_quad + j][k] << '\n';
        if ((k % 2) == 0) // remove empty bins
        {
          file_gauss << j / 2 << "," << histograms[i][j] << '\n';
        }
      }
    }
  }
  file_histo.close();
  file_gauss.close();
}