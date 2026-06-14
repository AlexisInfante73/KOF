#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <SFML/Graphics.hpp>
#include <string>

class Personaje {
private:
    // --- COMPONENTES GRÁFICOS (MODO BOLITA) ---
    sf::CircleShape cuerpoShape;
    sf::CircleShape sombraShape;
    sf::Color colorBase;

    // --- ATRIBUTOS DE CONFIGURACIÓN Y ESTADO ---
    float       radioOriginal;
    bool        enElSuelo;
    bool        estaAgachado;
    bool        estaAtacando;
    bool        estaRodando;
    float       direccionRodada;
    bool        esSuperAtaque;
    int         tipoAtaque;
    float       vida;
    int         comboStep; 
    std::string nombre;
    std::string rutaAvatar; // <-- Variable para la imagen añadida
    
    // --- COMBATE AÉREO ---
    bool        estaEnElAire;
    bool        estaAtacandoAire;
    int         tipoAtaqueAire;
    
    // --- ATURDIMIENTO Y LANZAMIENTO ---
    bool        estaAturdido;
    float       tiempoAturdimiento;
    
    // --- FÍSICAS Y POSICIÓN ---
    sf::Vector2f posicion;
    sf::Vector2f velocidad;
    
    // --- VARIABLES PARA EL SPRINT (DOBLE TOQUE) ---
    bool        presionoIzquierdaAntes;
    bool        presionoDerechaAntes;
    float       tiempoLimiteDobleToque;
    bool        estaCorriendo;

    // --- RELOJES ---
    sf::Clock   relojDobleToque;
    sf::Clock   relojRodada;
    sf::Clock   relojAtaque;
    sf::Clock   relojAturdimiento; 

public:
    Personaje();
    
    // --- INICIALIZACIÓN Y LÓGICA PRINCIPAL ---
    // Función actualizada para recibir la ruta de la imagen
    void inicializar(std::string nombrePeleador, std::string rutaImg, sf::Color color, float xInicial);
    
    void verificarDobleToque(sf::Event& evento, float xRival);
    void caminar(float direccion, bool correr = false); 
    void saltar();
    void setAgachado(bool agachado);
    void lanzarAtaque(int tipo);
    void lanzarAtaqueAereo(int tipo);
    void actualizar();
    bool puedeHacerCombo() const;
    void dibujar(sf::RenderWindow& window);
    void corregirPosicionX(float deltaX);
    
    // --- SISTEMA DE DAÑO Y COMBOS ---
    void recibirDanio(float cantidad);
    void curarVida(float cantidad);
    void aplicarAturdimiento(float duracion); 
    void serLanzado(float fuerzaX, float fuerzaY); 

    // --- GETTERS ---
    float       getPosicionX() const;
    float       getPosicionY() const; 
    bool        getEstaAgachado() const;
    bool        getEstaAtacando() const;
    bool        getEsSuperAtaque() const;
    int         getTipoAtaque() const;
    bool        getEstaRodando() const;
    bool        getEstaCorriendo() const;
    bool        getEstaAturdido() const; 
    float       getVida() const;
    sf::Color   getColorBase() const;
    std::string getNombre() const;
    std::string getRutaAvatar() const; // <-- Getter añadido
    
    bool        getEstaEnElAire() const;
    bool        getEstaAtacandoAire() const;
    int         getTipoAtaqueAire() const;
};

#endif // PERSONAJE_H