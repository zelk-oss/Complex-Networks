#ifndef HOPFIELD_MODEL_HPP
#define HOPFIELD_MODEL_HPP

#include <iostream>
#include <vector>
#include <cmath>

class Hopfield_Network
{
    double Temp = 0;
    int N_quad = 0;
    std::vector<std::vector<int>> Patterns; // Array of patterns
    std::vector<double> Weights;            // Weight matrix
    std::vector<int> Adjacency;             // Adjacency matrix

public:
    Hopfield_Network(); // Constructor

    Hopfield_Network(int n, double T); // Constructor

    ~Hopfield_Network(); // Distructor

    std::vector<int> const Get_Pattern(int i); // Get i pattern

    int const Get_N_Patterns(); // Get the number of patterns

    double Weight(int i, int j); // Compute the weight

    void Update_Weights(); // Update the weight matrix

    void const Print_Weights(); // Print the weight matrix

    int const Get_Weights_Size(); // Print the dimension of the Weights matrix

    double const Get_Weight(int i); // Get i weight

    void AddPattern(std::vector<int> &pattern); // Teach the network a new pattern

    double const Scalar_Product(int i_pattern, std::vector<int> neuron_); // Scalar product between i pattern and a vector

    double const Hamiltonian(std::vector<int> &neuron_); // Hamiltonian

    std::vector<int> Basic_dinamics(std::vector<int> &neuron); // Basic dynamics

    std::vector<int> Metropolis(std::vector<int> &neuron); // Metropolis algorithm

    std::vector<int> Glauber(std::vector<int> &neuron); // Glauber algorithm

    void Strong_Weight_Destroyer(double &p); // Destroy randomly the p part of the total link

    void Weak_Weight_Destroyer(int n); // Destroy n link chosen randomly

    std::vector<int> Laplacian(); // Compute the laplacian matrix

    double Local_Clustering(); // Compute the local clustering coefficient

    double Global_Clustering(); // Compute the global clustering coefficient

    int Node_Degree(int i); // Compute the node degree of i node
};

#endif