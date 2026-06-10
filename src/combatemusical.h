#ifndef COMBATEMUSICAL_H
#define COMBATEMUSICAL_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "personaje.h"

class CombateMusical {
private:
    // Escenario y Fondos
    sf::Texture texturaEscenario;
    sf::Sprite spriteEscenario;

    // Fuentes y Textos principales
    sf::Font fuenteUI;
    sf::Text txtCronometro;
    sf::Text txtAnuncioKO;
    sf::Text txtNombreJ1;
    sf::Text txtNombreJ2;

    // --- NUEVOS ELEMENTOS DE RESERVA ESTILO KOF 2002 ---
    sf::Text txtReservaJ1[3];
    sf::Text txtReservaJ2[3];

    // Avatars de la Interfaz
    sf::Texture texturaAvatarJ1;
    sf::Texture texturaAvatarJ2;
    sf::Sprite spriteAvatarJ1;
    sf::Sprite spriteAvatarJ2;
    sf::RectangleShape marcoAvatarJ1;
    sf::RectangleShape marcoAvatarJ2;

    // Barras de Vida
    sf::RectangleShape fondoBarraJ1;
    sf::RectangleShape fondoBarraJ2;
    sf::RectangleShape barraVidaJ1;
    sf::RectangleShape barraVidaJ2;

    // Barras de Energía / Especial
    sf::RectangleShape fondoEspecialJ1;
    sf::RectangleShape fondoEspecialJ2;
    sf::RectangleShape barraEspecialJ1;
    sf::RectangleShape barraEspecialJ2;
    sf::Text txtNivelJ1;
    sf::Text txtNivelJ2;

    // Control del Estado de Combate
    int tiempoRestante;
    sf::Clock relojPelea;
    sf::Clock relojEsperaRonda;
    sf::Clock relojDecisionBot;

    int indiceActivoJ1;
    int indiceActivoJ2;
    float energiaJ1;
    float energiaJ2;
    int nivelesJ1;
    int nivelesJ2;

    bool golpeImpactadoEsteTurno;
    bool botGolpeImpactadoEsteTurno;
    bool rondaTerminada;

    // Variables de Control de la IA
    float floatProximaDecision;
    bool botQuiereAgacharse;
    bool botQuiereDefenderse; // <-- CORREGIDO: Declaración de la variable faltante

    // Contenedores de los Equipos
    std::vector<Personaje> equipoJ1;
    std::vector<Personaje> equipoJ2;

    // Métodos Auxiliares Internos
    void cargarAvatarsUI();
    void acumularEnergiaJ1(float cantidad);
    void acumularEnergiaJ2(float cantidad);
    void actualizarIABot();
    void avanzarSiguienteRonda(int ganadorDeRonda);

public:
    CombateMusical();
    void reiniciarRelojes(int medallas);
    void procesarEntrada(sf::Event& evento);
    void actualizar();
    void dibujar(sf::RenderWindow& window);
};

#endif // COMBATEMUSICAL_H