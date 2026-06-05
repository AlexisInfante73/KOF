#ifndef COMBATEMUSICAL_H
#define COMBATEMUSICAL_H

#include <SFML/Graphics.hpp>
#include <vector>

struct Nota {
    sf::CircleShape cuerpo;
    float velocidad;
    int carril;

    Nota(int _carril, float _velocidad) {
        carril = _carril;
        velocidad = _velocidad;
        cuerpo.setRadius(38.f);
        cuerpo.setFillColor(sf::Color::Cyan);
        float posX = 480.f + (carril * 80.f) + 2.f;
        cuerpo.setPosition(posX, -80.f); 
    }
};

class CombateMusical {
private:
    std::vector<Nota> notas;
    std::vector<sf::RectangleShape> lineasCarriles;
    std::vector<sf::CircleShape> circulosImpacto;
    sf::Clock relojSpawn;
    sf::Clock relojDelta;
    float tiempoSpawn;

    bool carrilPresionado[4];
    bool teclaBloqueada[4];

    // --- VARIABLES DE TORNEO Y CONTROL DE BANDOS ---
    int vidaEquipoJugador;     
    int vidaEquipoOponente;    
    
    float tiempoRonda;         
    float tiempoGlobalJugador; 
    
    int puntuacionTotal;
    int puntosParaSiguienteAtaque;
    
    // Sistema de combos y curación
    int comboActual;           // Contador de notas acertadas seguidas

    // Control de flujo de turnos
    bool turnoActivo;          
    bool esTurnoJugador;       
    float tiempoEsperaTurno;   
    bool enPausaTransicion;    

    // Elementos visuales del HUD
    sf::Font fuenteHUD;
    sf::Text textoPuntaje;
    sf::Text textoVidaOponente;
    sf::Text textoVidaJugador;
    sf::Text textoRelojRonda;    
    sf::Text textoRelojGlobal;   
    sf::Text textoJuicio; 
    sf::Text textoCombo;          // Nuevo texto para ver los combos en pantalla
    sf::Text textoFinRound;
    sf::Text textoIndicadorTurno; 
    sf::Clock relojJuicio;

public:
    CombateMusical();
    void reiniciarRelojes();
    void limpiarNotas();
    void procesarEntrada();
    void actualizar();
    void dibujar(sf::RenderWindow& ventana);
};

#endif // COMBATEMUSICAL_H