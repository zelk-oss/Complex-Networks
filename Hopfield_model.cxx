#include "Hopfield_model.hpp"

#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>
#include <algorithm>

Hopfield_Network::Hopfield_Network() = default;

Hopfield_Network::Hopfield_Network(int n, double T) : N_quad(n), Temp(T){};

Hopfield_Network::~Hopfield_Network() = default;

std::vector<int> const Hopfield_Network::Get_Pattern(int i) // get i-pattern
{
    std::vector<int> neuron_;
    neuron_ = Patterns[i];
    return neuron_;
}

int const Hopfield_Network::Get_N_Patterns() // Get the size of Patterns
{
    int get_n_patterns = Patterns.size();
    return get_n_patterns;
}

double Hopfield_Network::Weight(int i, int j) // set the Weight
{
    double weight_ = 0;
    if (i == j)
    {
        weight_ = 0;
    }
    else
    {
        double sum = 0;
        for (int k = 0; k < Patterns.size(); k++)
        {
            double sum_part = Patterns[k][i] * Patterns[k][j];
            sum = sum + sum_part;
        }
        weight_ = sum / N_quad;
    }
    return weight_;
}

void Hopfield_Network::Update_Weights() // update the Weights
{
    Weights.clear();
    Adjacency.clear();
    for (int i = 0; i < N_quad; i++)
    {
        for (int j = 0; j < N_quad; j++)
        {
            Weights.push_back(Weight(i, j));
            if (i == j)
            {
                Adjacency.push_back(0);
            }
            else
            {
                Adjacency.push_back(1);
            }
        }
    }
}

void const Hopfield_Network::Print_Weights() // print ht e Weight matrix
{
    for (int i = 0; i < N_quad; i++)
    {
        for (int j = 0; j < N_quad; j++)
        {
            std::cout << Weights[i * N_quad + j] << ", ";
        }
        std::cout << '\n';
    }
}

int const Hopfield_Network::Get_Weights_Size() // Get the size of Weifhts
{
    int Weights_Size = Weights.size();
    return Weights_Size;
}

void Hopfield_Network::AddPattern(std::vector<int> &pattern) // Add a pattern in Patterns
{
    Patterns.push_back(pattern);
    Update_Weights();
}

double const Hopfield_Network::Scalar_Product(int i_pattern, std::vector<int> vector_) // scalar product between a general vector and the i-pattern
{
    double add_ = 0;
    for (int j = 0; j < N_quad; j++)
    {
        int add_par = Patterns[i_pattern][j] * vector_[j];
        add_ += add_par;
    }
    add_ /= N_quad;
    return add_;
}

double const Hopfield_Network::Hamiltonian(std::vector<int> &vector_) // Hamiltonian
{
    double sum_1 = 0;
    for (int i = 0; i < vector_.size(); i++)
    {
        double sum_2 = 0;
        for (int j = 0; j < vector_.size(); j++)
        {
            double add = Weights[i * N_quad + j] * vector_[j];
            sum_2 = sum_2 + add;
        }
        double add_ = vector_[i] * sum_2;
        sum_1 = sum_1 + add_;
    }

    double hamiltonian = -sum_1 / 2;
    return hamiltonian;
}

std::vector<int> Hopfield_Network::Basic_dinamics(std::vector<int> &neuron) // deterministic dinamics
{
    auto final_ = neuron;
    double sign = 0;
    int caso = rand() % N_quad;

    for (int j = 0; j < neuron.size(); j++)
    {
        double weight_ij = Weights[caso * N_quad + j];
        double sum = weight_ij * neuron[j];
        sign += sum;
    }

    if (sign < 0)
    {
        final_[caso] = -1;
    }
    else
    {
        final_[caso] = 1;
    }
    return final_;
}

std::vector<int> Hopfield_Network::Metropolis(std::vector<int> &neuron) // Metropolis algorithm
{
    std::vector<int> test = neuron;
    int caso = rand() % N_quad;
    test[caso] = -test[caso];

    double delta_H = Hamiltonian(test) - Hamiltonian(neuron);

    if (delta_H < 0)
    {
        return test;
    }
    else
    {
        double caso = (double)rand() / RAND_MAX;
        double expo = std::exp(-delta_H / Temp);

        if (caso < expo)
        {
            return test;
        }
        else
        {
            return neuron;
        }
    }
}

std::vector<int> Hopfield_Network::Glauber(std::vector<int> &neuron) // Glauber algorithm
{
    std::vector<int> test = neuron;
    int caso = rand() % N_quad;
    test[caso] = -test[caso];

    double sum_2 = 0;
    for (int j = 0; j < test.size(); j++)
    {
        double add = Weights[caso * N_quad + j] * test[j];
        sum_2 += add;
    }

    double delta_H = 2 * neuron[caso] * sum_2;

    double caso_ = (double)rand() / RAND_MAX;
    double expo = std::exp(delta_H / Temp) + 1;
    double prob_flip = 1 / expo;

    if (caso_ < prob_flip)
    {
        return test;
    }
    else
    {
        return neuron;
    }
}

void Hopfield_Network::Strong_Weight_Destroyer(double &prob)
{
    for (int i = 0; i < N_quad; i++)
    {
        for (int j = i; j < N_quad; j++)
        {
            double caso = (double)rand() / RAND_MAX;

            if (caso < prob)
            {
                Weights[i * N_quad + j] = 0;
                Weights[j * N_quad + i] = 0;
                Adjacency[i * N_quad + j] = 0;
                Adjacency[j * N_quad + i] = 0;
            }
            else
            {
                Weights[i * N_quad + j] /= (1 - prob);
                Weights[j * N_quad + i] /= (1 - prob);
                Adjacency[i * N_quad + j] = 1;
                Adjacency[j * N_quad + i] = 1;
            }
        }
    }
}

void Hopfield_Network::Weak_Weight_Destroyer(int n)
{
    for (int k = 0; k < n; k++)
    {
        int i = rand() % N_quad;
        int j = rand() % N_quad;
        Weights[i * N_quad + j] = 0;
        Weights[j * N_quad + i] = 0;
    }
}

std::vector<int> Hopfield_Network::Laplacian()
{
    std::vector<int> laplacian;
    for (int i = 0; i < Adjacency.size(); i++)
    {
        laplacian.push_back(-Adjacency[i]);
    }
    for (int j = 0; j < N_quad; j++)
    {
        int degree = 0;
        for (int k = 0; k < N_quad; k++)
        {
            degree += Adjacency[j * N_quad + k];
        }
        laplacian[j * N_quad + j] += degree;
    }
    return laplacian;
}
double Hopfield_Network::Local_Clustering()
{
    double local = 0.;
    int num = 0;
    int den = 0;
    for (int i = 0; i < N_quad; i++)
    {
        for (int j = 0; j < N_quad; j++)
        {
            for (int k = 0; k < N_quad; k++)
            {
                num += Adjacency[i * N_quad + j] * Adjacency[j * N_quad + k] * Adjacency[k * N_quad + i];
            }
        }

        int degree = 0;
        for (int j = 0; j < N_quad; j++)
        {
            degree += Adjacency[i * N_quad + j];
        }
        den += degree * (degree - 1);

        local += (double)num / (double)den;
    }

    return local / N_quad;
}

double Hopfield_Network::Global_Clustering()
{
    int num = 0;
    int den = 0;
    for (int i = 0; i < N_quad; i++)
    {
        for (int j = i + 1; j < N_quad; j++)
        {
            for (int k = j + 1; k < N_quad; k++)
            {
                num += Adjacency[i * N_quad + j] * Adjacency[j * N_quad + k] * Adjacency[k * N_quad + i];
            }
        }
    }
    for (int i = 0; i < N_quad; i++)
    {
        int degree = 0;
        for (int j = 0; j < N_quad; j++)
        {
            degree += Adjacency[i * N_quad + j];
        }
        den += degree * (degree - 1);
    }
    double cluster = (double)num / (double)den;
    return cluster;
}

int Hopfield_Network::Node_Degree(int i)
{
    int Degree = 0;
    for (int k = 0; k < N_quad; k++)
    {
        Degree += Adjacency[i * N_quad + k];
    }
    return Degree;
}

int Hopfield_Network::Stop(std::vector<int> neuron)
{
    int fermo_ = 0;
    for (int i = 0; i < Patterns.size(); i++)
    {
        if (neuron == Patterns[i])
        {
            fermo_ = 1;
        }
    }
    return fermo_;
};