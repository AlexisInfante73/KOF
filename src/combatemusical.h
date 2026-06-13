#ifndef COMBATEMUSICAL_H
#define COMBATEMUSICAL_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "personaje.h" // Asegúrate de tener este archivo en tu proyecto

enum class EstadoJuego {
    SeleccionPersonajes,
    Combate
    // (Nota: si añadiste medallasElegidas u otras pantallas en el .cpp, 
    // asegúrate de que el estado corresponda a tu lógica).
};

class CombateMusical {
private:
    // --- Recursos Visuales ---
    sf::Texture texturaEscenario;
    sf::Sprite spriteEscenario;
    sf::Texture texturaSeleccion;
    sf::Sprite spriteSeleccion;
    sf::Font fuenteUI;

    // --- HUD Selección ---
    sf::RectangleShape selectorCuadrula;
    int filaSeleccionada;
    int colSeleccionada;
    float tiempoSeleccionRestante;
    sf::Text txtTiempoSeleccion;
    sf::Clock relojSeleccion;

    // --- HUD Combate ---
    sf::Text txtCronometro;
    int tiempoRestante;
    sf::Text txtAnuncioKO;

    // --- SISTEMA DE COMBOS (HUD) ---
    sf::Text txtComboJ1;
    sf::Text txtComboJ2;
    int hitsJ1;
    int hitsJ2;
    
    sf::Text txtReservaJ1[3];
    sf::Text txtReservaJ2[3];
    sf::Text txtNombreJ1;
    sf::Text txtNombreJ2;

    sf::Texture texturaAvatarJ1;
    sf::Texture texturaAvatarJ2;
    sf::Sprite spriteAvatarJ1;
    sf::Sprite spriteAvatarJ2;
    sf::RectangleShape marcoAvatarJ1;
    sf::RectangleShape marcoAvatarJ2;

    sf::RectangleShape fondoBarraJ1;
    sf::RectangleShape fondoBarraJ2;
    sf::RectangleShape barraVidaJ1;
    sf::RectangleShape barraVidaJ2;

    sf::Text txtNivelJ1;
    sf::Text txtNivelJ2;
    sf::RectangleShape fondoEspecialJ1;
    sf::RectangleShape fondoEspecialJ2;
    sf::RectangleShape barraEspecialJ1;
    sf::RectangleShape barraEspecialJ2;

    // --- Lógica y Variables de Estado ---
    EstadoJuego estadoActual;
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
    
    // --- Relojes y Tiempos ---
    sf::Clock relojPelea;
    sf::Clock relojEsperaRonda;
    sf::Clock relojDecisionBot;
    sf::Clock relojComboJ1;
    sf::Clock relojComboJ2;
    sf::Clock relojDeltaTime; // Nuevo reloj para control de FPS

    // --- IA Bot ---
    float floatProximaDecision;
    bool botQuiereAgacharse;
    bool botQuiereDefenderse;

    // --- Métodos Internos ---
    void inicializarPantallaSeleccion();
    void avanzarSiguienteRonda(int ganadorDeRonda);
    void actualizarIABot();
    void acumularEnergiaJ1(float cantidad);
    void acumularEnergiaJ2(float cantidad);
    void cargarAvatarsUI();

public:
    CombateMusical();
    
    // --- SOLUCIÓN DEL ERROR DE COMPILACIÓN ---
    // Esta función ahora es pública para que main() pueda acceder a ella.
    void reiniciarRelojes();
    
    void procesarEntrada(sf::Event& evento);
    void actualizar();
    void dibujar(sf::RenderWindow& window);
};

#endif // COMBATEMUSICAL_H