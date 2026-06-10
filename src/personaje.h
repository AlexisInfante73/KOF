#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <SFML/Graphics.hpp>
#include <string>

// Enumeración para controlar las animaciones y estados del luchador
enum class Estado {
    IDLE,
    ATACANDO,
    HERIDO,
    DERROTADO
};

class Personaje {
private:
    sf::Texture textura;
    sf::Sprite sprite;
    
    // Variables de atributos lógicos
    int vida;
    std::string nombre;
    Estado estadoActual;
    bool esJugador1;

    // Coordenadas actuales en la pantalla
    float posX;
    float posY;

    // Control de tiempo para los estados temporales (como ataques)
    sf::Clock relojEstado;

public:
    Personaje();
    
    // Inicializa texturas, escala, orientación y posición inicial
    void inicializar(std::string rutaTextura, bool esJ1, float xInicial, float yInicial);
    
    // Permite que la clase CombateMusical altere la posición física del sprite de forma fluida
    void actualizarPosicion(float nuevoX, float nuevoY);
    
    // Cambia el estado (animación lógica) del peleador
    void cambiarEstado(Estado nuevoEstado);
    
    // Aplica daño restando puntos de vida
    void recibirDanio(int cantidad);
    
    // Getters esenciales
    int getVida() const;
    Estado getEstado() const;

    // Actualiza las lógicas internas de tiempo y colores por frame
    void actualizar();
    
    // Renderiza el sprite actual en la ventana
    void dibujar(sf::RenderWindow& ventana);
};

#endif // PERSONAJE_H