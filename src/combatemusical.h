#ifndef COMBATEMUSICAL_H
#define COMBATEMUSICAL_H
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

class Combatemusical {
private:
    float xCentroInicio;
    sf::RectangleShape suelo;
    struct Nota { 
        sf::RectangleShape rect;
        int riel;
        float velocidad;
        int propietario;
    };
    std::vector<Nota> notas;
    sf::Clock relojNotas;
public:
    Combatemusical();
    void actualizar(int jugadorActivo);
    void dibujar(sf::RenderWindow& window);
    int procesarGolpe(int riel, int jugador, bool& aumentoCombo);
    float getXCentroInicio() const { return xCentroInicio; }
};
#endif