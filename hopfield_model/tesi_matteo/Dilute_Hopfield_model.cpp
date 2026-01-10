#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cmath>
#include <chrono>
#include <thread>
#include <cassert>
#include <SFML/Graphics.hpp>
#include "Hopfield_model.hpp"

int main()
{
    srand(time(NULL));

    int N_dim = 0; // inizializzo la macchina
    std::cout << "dimensione rete di Hopfield:" << '\n';
    std::cin >> N_dim;
    assert(N_dim > 0);
    int N_quad = N_dim * N_dim;

    double Temp = 0;
    std::cout << "temperatura:" << '\n';
    std::cin >> Temp;
    assert(Temp >= 0);

    double corruzione = 0;
    std::cout << "percentuale di corruzione:" << '\n';
    std::cin >> corruzione;
    assert(corruzione >= 0 && corruzione <= 1);

    double distruzione = 0;
    std::cout << "probabilita che un legame venga distrutto:" << '\n';
    std::cin >> distruzione;
    assert(distruzione >= 0 && distruzione <= 1);

    Hopfield_Network hopfield(N_quad, Temp);

    std::vector<int> colonne(N_quad, 1);
    for (int i = 1; i < N_dim; i += 2)
    {
        for (int j = 0; j < N_dim; ++j)
        {
            colonne[i * N_dim + j] = -1;
        }
    }
    hopfield.AddPattern(colonne);

    std::vector<int> random = colonne;
    for (int i = 0; i < N_quad; i++)
    {
        double caso = (double)rand() / RAND_MAX;
        ;
        if (caso < corruzione)
        {
            random[i] = -random[i];
        }
    }

    sf::RenderWindow window(sf::VideoMode(N_dim * 20.0f, N_dim * 20.0f), "grafico"); // inizializzo la componente grafica

    std::clock_t start_ = clock(); // impostazioni per il tempo
    std::clock_t end_ = 0;
    std::clock_t delta_time = 0;
    int contatore = 0;

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::Closed)
            {
                window.close();
                std::cout << contatore << '\n';
            }
        }

        window.clear(sf::Color::White);

        end_ = clock();
        delta_time = end_ - start_;
        delta_time = delta_time / CLOCKS_PER_SEC;

        if (delta_time > 10) // ogni 20 secondi mi distrugge alcuni pesi e riparte la macchina da capo
        {
            start_ = clock();
            for (int i = 0; i < 10; i++)
            {

                hopfield.Weak_Weight_Destroyer(1);
            }
            random = colonne;
            for (int i = 0; i < N_quad; ++i)
            {
                double caso = (double)rand() / RAND_MAX;
                if (caso < corruzione)
                {
                    random[i] = -random[i];
                }
            }
            contatore++;
        }

        random = hopfield.Glauber(random);

        for (int i = 0; i < N_dim; i++)
        {
            for (int j = 0; j < N_dim; j++)
            {
                sf::RectangleShape square(sf::Vector2f(20.0f, 20.0f));
                if (random[i * N_dim + j] == +1)
                {
                    square.setFillColor(sf::Color::Green);
                }
                else
                {
                    square.setFillColor(sf::Color::Red);
                }
                square.setPosition(i * 20.0f, j * 20.0f);
                window.draw(square);
            }
        }
        window.display();
    }
}