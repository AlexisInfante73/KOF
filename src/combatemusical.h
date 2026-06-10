#ifndef COMBATEMUSICAL_H
#define COMBATEMUSICAL_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "personaje.h"

class CombateMusical {
private:
    sf::Texture texturaEscenario;
    sf::Sprite spriteEscenario;
    sf::Font fuenteUI;

    // Elementos del HUD Superior (Vida y Nombres)
    sf::RectangleShape fondoBarraJ1;
    sf::RectangleShape fondoBarraJ2;
    sf::RectangleShape barraVidaJ1;
    sf::RectangleShape barraVidaJ2;
    
    sf::RectangleShape marcoAvatarJ1;
    sf::RectangleShape marcoAvatarJ2;
    sf::Texture texturaAvatarJ1;
    sf::Texture texturaAvatarJ2;
    sf::Sprite spriteAvatarJ1;
    sf::Sprite spriteAvatarJ2;

    sf::Text txtCronometro;
    sf::Text txtContadorOrden;
    sf::Text txtNombreJ1;
    sf::Text txtNombreJ2;
    sf::Text txtAnuncioKO;

    // Elementos del HUD Inferior (Barras de Carga Especial)
    sf::RectangleShape fondoEspecialJ1;
    sf::RectangleShape fondoEspecialJ2;
    sf::RectangleShape barraEspecialJ1;
    sf::RectangleShape barraEspecialJ2;
    sf::Text txtNivelJ1;
    sf::Text txtNivelJ2;

    // Lógica interna de la pelea
    std::vector<Personaje> equipoJ1;
    std::vector<Personaje> equipoJ2;
    int indiceActivoJ1;
    int indiceActivoJ2;
    int tiempoRestante;
    
    float energiaJ1;
    float energiaJ2;
    int nivelesJ1;
    int nivelesJ2;

    sf::Clock relojPelea;
    sf::Clock relojDecisionBot;
    sf::Clock relojEsperaRonda;
    bool rondaTerminada;
    bool golpeImpactadoEsteTurno;
    bool botGolpeImpactadoEsteTurno;
    float floatProximaDecision;
    bool botQuiereAgacharse;

    void acumularEnergiaJ1(float cantidad);
    void acumularEnergiaJ2(float cantidad);
    void cargarAvatarsUI();
    void actualizarIABot();
    void avanzarSiguienteRonda(int ganadorDeRonda);

public:
    CombateMusical();
    void procesarEntrada(sf::Event& evento);
    void actualizar();
    void dibujar(sf::RenderWindow& window);
    void reiniciarRelojes(int medallas);
};

#endif