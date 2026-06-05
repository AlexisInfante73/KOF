#include "combatemusical.h"
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <cmath>

Combatemusical::Combatemusical() {
    suelo.setSize(sf::Vector2f(800.f, 150.f));
    suelo.setFillColor(sf::Color(35, 35, 40)); 
    suelo.setPosition(0.f, 450.f);             
    xCentroInicio = 300.f;
}

void Combatemusical::actualizar(int jugadorActivo) {
    if (relojNotas.getElapsedTime().asSeconds() > 0.40f) {
        Nota nuevaNota;
        nuevaNota.rect.setSize(sf::Vector2f(45.f, 20.f)); 
        nuevaNota.riel = rand() % 4;
        nuevaNota.propietario = jugadorActivo; 

        if (nuevaNota.propietario == 1) nuevaNota.rect.setFillColor(sf::Color::Cyan);
        else nuevaNota.rect.setFillColor(sf::Color(255, 0, 255));

        nuevaNota.rect.setPosition(xCentroInicio + (nuevaNota.riel * 50), 0.f);
        nuevaNota.velocidad = 5.5f;
        notas.push_back(nuevaNota);
        relojNotas.restart();
    }

    for (size_t i = 0; i < notas.size(); i++) {
        notas[i].rect.move(0.f, notas[i].velocidad);
    }

    if (!notas.empty() && notas[0].rect.getPosition().y > 520.f) {
        notas.erase(notas.begin());
    }
}

int Combatemusical::procesarGolpe(int riel, int jugador, bool& aumentoCombo) {
    for (size_t i = 0; i < notas.size(); i++) {
        if (notas[i].riel == riel && notas[i].propietario == jugador) {
            float yNota = notas[i].rect.getPosition().y;
            float distancia = std::abs(yNota - 500.f);

            if (distancia <= 45.f) {
                notas.erase(notas.begin() + i);
                aumentoCombo = true;

                if (distancia <= 8.f) return 30; 
                else if (distancia <= 20.f) return 20; 
                else return 10; 
            }
        }
    }
    aumentoCombo = false;
    return 0; 
}

void Combatemusical::dibujar(sf::RenderWindow& window) {
    window.draw(suelo);
    for (const auto& nota : notas) {
        window.draw(nota.rect);
    }
}
