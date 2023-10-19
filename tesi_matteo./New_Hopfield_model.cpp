#include <iostream>
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

    int N_dim = 0;
    std::cout << "dimensione rete di Hopfield:" << '\n';
    std::cin >> N_dim;
    assert(N_dim > 0);
    int N_quad = N_dim * N_dim;

    double Temp = 0;
    std::cout << "temperatura della rete:" << '\n';
    std::cin >> Temp;
    assert(Temp > 0);

    Hopfield_Network hopfield(N_quad, Temp);

    int funzione = -1;
    int stop_ = 0;
    std::vector<int> stringa_aggiunta(N_quad, 1);
    std::vector<int> primo_pattern(N_quad, 1);
    // hopfield.AddPattern(primo_pattern);
    std::vector<int> secondo_pattern(N_quad, -1);
    // hopfield.AddPattern(secondo_pattern);
    std::vector<int> random(N_quad);

    std::vector<int> su_giu(N_quad, 1);
    for (int i = 1; i < N_quad; i = i + 2)
    {
        su_giu[i] = -1;
    }
    // hopfield.AddPattern(su_giu);

    std::vector<int> colonne(N_quad, 1);
    for (int i = 1; i < N_dim; i = i + 2)
    {
        for (int j = 0; j < N_dim; j++)
        {
            colonne[i * N_dim + j] = -1;
        }
    }
    // hopfield.AddPattern(colonne);

    std::vector<int> righe(N_quad, 1);
    for (int i = 0; i < N_dim; i++)
    {
        for (int j = 1; j < N_dim; j = j + 2)
        {
            righe[i * N_dim + j] = -1;
        }
    }
    hopfield.AddPattern(righe);

    std::cout << "selezionare algoritmo da utilizzare:" << '\n';
    std::cout << "1- utilizza l'algoritmo di Metropolis" << '\n';
    std::cout << "2- utilizza l'algoritmo di Glauber" << '\n';
    std::cin >> funzione;

    sf::RenderWindow window(sf::VideoMode(N_dim * 20.0f, N_dim * 20.0f), "grafico");

    random = righe;

    for (int i = 0; i < random.size(); i++)
    {
        int caso = rand() % 101;
        if (caso < 50)
        {
            random[i] = -1;
        }
        else
        {
            random[i] = +1;
        }
    }
    stop_ = hopfield.Stop(random);

    int N_Patterns = hopfield.Get_N_Patterns();

    while (window.isOpen())
    {
        sf::Event event;

        // int cicli = 50 * N_quad;
        // for (int i = 0; i < cicli; i++)
        //{

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        window.clear(sf::Color::White);

        switch (funzione)
        {
        case 1:
            random = hopfield.Metropolis(random);
            break;

        case 2:
            random = hopfield.Glauber(random);
            break;

        default:
            std::cout << "selezionare algoritmo da utilizzare" << '\n';
            std::cout << "1- utilizza l'algoritmo di Metropolis" << '\n';
            std::cout << "2- utilizza l'algoritmo di Glauber" << '\n';
            std::cin >> funzione;

            break;
        }

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
        //}

        //std::cout << "ciclo finito \n";
    }
}