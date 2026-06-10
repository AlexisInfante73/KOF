#ifndef COMBATEMUSICAL_H
#define COMBATEMUSICAL_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "personaje.h" // Asegúrate de que este include sea correcto según tu estructura

class CombateMusical {
private:
    sf::Texture texturaEscenario;
    sf::Sprite spriteEscenario;
    sf::Font fuenteUI;

    // HUD Superior
    sf::Text txtCronometro;
    sf::Text txtAnuncioKO;
    sf::Text txtContadorOrden;
    sf::Text txtNombreJ1;
    sf::Text txtNombreJ2;

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

    // HUD Inferior (Súper Energía)
    sf::Text txtNivelJ1;
    sf::Text txtNivelJ2;
    sf::RectangleShape fondoEspecialJ1;
    sf::RectangleShape fondoEspecialJ2;
    sf::RectangleShape barraEspecialJ1;
    sf::RectangleShape barraEspecialJ2;

    // Lógica del juego
    int tiempoRestante;
    sf::Clock relojPelea;
    sf::Clock relojDecisionBot;
    sf::Clock relojEsperaRonda;

    std::vector<Personaje> equipoJ1;
    std::vector<Personaje> equipoJ2;
    int indiceActivoJ1;
    int indiceActivoJ2;

    float energiaJ1;
    float energiaJ2;
    int nivelesJ1;
    int nivelesJ2;

    bool golpeImpactadoEsteTurno;
    bool botGolpeImpactadoEsteTurno;
    bool rondaTerminada;
    float floatProximaDecision;
    
    // Banderas de estado de la IA del Bot
    bool botQuiereAgacharse;
    bool botQuiereDefenderse; 

    void cargarAvatarsUI();
    void actualizarIABot();
    void avanzarSiguienteRonda(int ganadorDeRonda);

public:
    CombateMusical();
    void procesarEntrada(sf::Event& evento);
    void actualizar();
    void dibujar(sf::RenderWindow& window);
    void reiniciarRelojes(int medallas);
    void acumularEnergiaJ1(float cantidad);
    void acumularEnergiaJ2(float cantidad);
};

#endif // COMBATEMUSICAL_H