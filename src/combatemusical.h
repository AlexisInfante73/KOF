#ifndef COMBATEMUSICAL_H
#define COMBATEMUSICAL_H

#include <SFML/Graphics.hpp>
#include "personaje.h"
#include <vector>
#include <string>

class CombateMusical {
private:
    // Personajes
    Personaje jugador1;
    Personaje jugador2;

    // Posiciones lógicas independientes de los personajes
    float posX_J1, posY_J1;
    float posX_J2, posY_J2;

    // Constantes de físicas
    const float SUELO_Y = 530.f;  // Ajusta según la altura de tus sprites
    const float GRAVEDAD = 0.98f;
    const float FUERZA_SALTO = -20.f;

    // Variables de control de combate
    float velocidadCaminar;
    float distanciaAtaque;
    int danioGolpeBase;

    // Estado físicas jugador 1
    float velocidadVerticalJ1;
    bool enElSueloJ1;
    bool estaAgachadoJ1;

    // Control de Inteligencia Artificial (IA)
    sf::Clock relojIA;
    float tiempoDecisionIA;

    // Elementos gráficos del escenario
    sf::Texture texturaEscenario;
    sf::Sprite spriteEscenario;
    bool tieneEscenario;

    // --- CÁMARA VIRTUAL (ESTILO KOF) ---
    sf::View camara;

    // Elementos de la Interfaz Gráfica (HUD fijo)
    sf::RectangleShape fondoBarrasJ1;
    sf::RectangleShape barraVidaJ1;
    sf::RectangleShape fondoBarrasJ2;
    sf::RectangleShape barraVidaJ2;

    void cargarEscenarioAleatorio();
    void evaluarImpactoAtaque(bool esJugador1Atacando);

public:
    CombateMusical();
    void configurarNombres(std::string n1, std::string n2);
    void configurarEquipos(std::vector<std::string> eq1, std::vector<std::string> eq2);
    void reiniciarRelojes(int dificultad);
    void procesarEntrada(sf::Event& evento);
    void actualizar();
    void dibujar(sf::RenderWindow& ventana);
};

#endif // COMBATEMUSICAL_H