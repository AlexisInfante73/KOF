#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <SFML/Graphics.hpp>
#include <string>

class Personaje {
private:
    sf::CircleShape cuerpoShape; 
    sf::Vector2f posicion;
    sf::Vector2f velocidad;
    bool enElSuelo;
    bool estaAgachado;
    float radioOriginal;
    sf::Color colorBase;

    // Sistema de Vida estilo KOF
    float vida;
    
    // Identidad del luchador
    std::string nombre;
    std::string rutaAvatar;

    // Sistema de Estados de Ataque
    bool estaAtacando;
    bool esSuperAtaque; // Para el estado especial con la tecla E
    int tipoAtaque; 
    sf::Clock relojAtaque;
    const float DURACION_ATAQUE = 0.2f; 

    // Constantes físicas internas
    const float GRAVEDAD = 0.6f;
    const float FUERZA_SALTO = -14.f;
    const float VEL_CAMINAR = 5.f;
    const float ALTURA_SUELO = 600.f; 

public:
    Personaje();
    
    void inicializar(std::string nombrePeleador, std::string rutaImg, sf::Color color, float xInicial);
    void caminar(float direccion); 
    void saltar();
    void setAgachado(bool agachado);
    void lanzarAtaque(int tipo);

    void actualizar();
    void dibujar(sf::RenderWindow& window);

    float getPosicionX() const;
    void corregirPosicionX(float deltaX);
    bool getEstaAgachado() const;
    bool getEstaAtacando() const;
    bool getEsSuperAtaque() const;
    int getTipoAtaque() const;
    float getVida() const;
    std::string getNombre() const; 
    std::string getRutaAvatar() const;
    void recibirDanio(float cantidad);
    void curarVida(float cantidad);
};

#endif