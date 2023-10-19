#ifndef HOPFIELD_MODEL_HPP
#define HOPFIELD_MODEL_HPP

#include <iostream>
#include <vector>
#include <cmath>

class Hopfield_Network
{
    double Temp = 0;
    int N_quad = 0;
    std::vector<std::vector<int>> Patterns; // elenco dei ricordi
    std::vector<double> Weights;            // matrice pesi
    std::vector<int> Adjacency;             // matrice adiacenze

public:
    Hopfield_Network();

    Hopfield_Network(int n, double T);

    ~Hopfield_Network();

    std::vector<int> const Get_Pattern(int i); // ottieni i-esimo ricordo

    int const Get_N_Patterns(); // ottieni numero ricordi

    double Weight(int i, int j); // calcola i pesi

    void Update_Weights(); // aggiorna Weights

    void const Print_Weights(); // stampa la matrice dei pesi

    int const Get_Weights_Size(); // stampa la dimenzione del vettore Weights

    void AddPattern(std::vector<int> &pattern); // memorizza un ricordo

    double const Scalar_Product(int i_pattern, std::vector<int> neuron_); // prodotto scalare tra i-esimo pattern e un vettore

    double const Hamiltonian(std::vector<int> &neuron_); // Hamiltoniana

    std::vector<int> Basic_dinamics(std::vector<int> &neuron); // dinamica base

    std::vector<int> Metropolis(std::vector<int> &neuron); // Metropolis algorithm

    std::vector<int> Glauber(std::vector<int> &neuron); // Glauber algorithm

    void Strong_Weight_Destroyer(double &p);

    void Weak_Weight_Destroyer(int n);

    std::vector<int> Laplacian();

    double Local_Clustering();

    double Global_Clustering();

    int Node_Degree(int i);

    int Stop(std::vector<int> neuron); // ferma il ciclo
};

#endif