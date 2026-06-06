#ifndef COMBATEMUSICAL_H
#define COMBATEMUSICAL_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <algorithm>

struct Nota {
    int carril;
    float velocidad;
    sf::CircleShape cuerpo;

    Nota(int _carril, float _velocidad) {
        carril = _carril;
        velocidad = _velocidad;
        
        cuerpo.setRadius(38.f);
        cuerpo.setOrigin(38.f, 38.f);
        
        float posX = 480.f + (carril * 80.f) + 40.f;
        cuerpo.setPosition(posX, -40.f);
    }
};

class CombateMusical {
public:
    CombateMusical();
    void configurarNombres(const std::string& nombre1, const std::string& nombre2);
    int calcularSinergiaEquipo(const std::vector<std::string>& equipo);
    void configurarEquipos(const std::vector<std::string>& e1, const std::vector<std::string>& e2);
    void reiniciarRelojes(int medallas); // Se mantiene por compatibilidad
    void limpiarNotas();
    void procesarEntrada();
    void actualizar();
    void dibujar(sf::RenderWindow& ventana);

private:
    float tiempoSpawn;
    float tiempoRonda;
    float tiempoGlobalJ1;
    float tiempoEsperaTurno;
    float tiempoVisualEspecial;
    
    bool turnoActivo;
    bool esTurnoJ1;
    bool enPausaTransicion;
    bool especialActivadoVisual;

    // --- Control de Medallas Integrado ---
    bool faseMedallas;           // TRUE = Eligiendo medallas, FALSE = Peleando
    int medallasActuales;        
    float velocidadFijaPartida;  

    int vidaJ1;
    int vidaJ2;
    int scoreJ1;
    int scoreJ2;
    int comboJ1;
    int comboJ2;
    int puntosParaSiguienteAtaque;

    int sinergiaTotalE1;
    int sinergiaTotalE2;
    float cargaEspecialE1;
    float cargaEspecialE2;

    std::string strNombreJ1;
    std::string strNombreJ2;
    std::vector<std::string> integrantesE1;
    std::vector<std::string> integrantesE2;

    bool carrilPresionado[4];
    bool teclaBloqueada[4];

    std::vector<sf::RectangleShape> lineasCarriles;
    std::vector<sf::CircleShape> circulosImpacto;
    std::vector<Nota> notas;

    sf::Clock relojDelta;
    sf::Clock relojSpawn;
    sf::Clock relojJuicio;

    sf::Font fuenteHUD;
    sf::Text textoVidaJ1;
    sf::Text textoScoreJ1;
    sf::Text textoRelojGlobal;
    sf::Text textoVidaJ2;
    sf::Text textoScoreJ2;
    sf::Text textoRelojRonda;
    sf::Text textoIndicadorTurno;
    sf::Text textoJuicio;
    sf::Text textoCombo;
    sf::Text textoFinRound;
    sf::Text textoEquiposIntegrantes;
    sf::Text textoMedidorEspecial;
    sf::Text textoAlertaEspecial;

    // --- Textos de la interfaz de medallas ---
    sf::Text textoTituloMedallas;
    sf::Text textoContadorMedallas;
    sf::Text textoInstruccionesMedallas;
    bool teclaArribaBloqueada;
    bool teclaAbajoBloqueada;
    bool teclaEnterBloqueada;
};

#endif // COMBATEMUSICAL_H