#include "personaje.h"
#include <iostream>

Personaje::Personaje() {
    vida = 100;
    estadoActual = Estado::IDLE;
    esJugador1 = true;
    posX = 0.f;
    posY = 0.f;
}

void Personaje::inicializar(std::string rutaTextura, bool esJ1, float xInicial, float yInicial) {
    esJugador1 = esJ1;
    posX = xInicial;
    posY = yInicial;

    // Intentar cargar la textura desde la raíz
    if (!textura.loadFromFile(rutaTextura)) {
        // Intento de respaldo por si el ejecutable corre dentro de /bin
        if (!textura.loadFromFile("../" + rutaTextura)) {
            std::cout << "[ERROR PERSONAJE]: No se pudo cargar la imagen: " << rutaTextura << std::endl;
            return;
        }
    }

    sprite.setTexture(textura);
    
    // Configurar el origen en el centro horizontal (width / 2) y en la base (height)
    // Esto hace que el personaje "pise" el suelo de forma correcta y rote bien al caer derrotado
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height); 

    // Escala del personaje (ajusta estos números si se ven muy grandes o pequeños)
    float escalaX = 2.5f;
    float escalaY = 2.5f;

    // Efecto espejo: El Jugador 2 debe mirar hacia la izquierda de forma nativa
    if (!esJugador1) {
        sprite.setScale(-escalaX, escalaY); 
    } else {
        sprite.setScale(escalaX, escalaY);
    }

    // Posicionar el sprite en las coordenadas iniciales
    sprite.setPosition(posX, posY);
}

void Personaje::actualizarPosicion(float nuevoX, float nuevoY) {
    posX = nuevoX;
    posY = nuevoY;
    sprite.setPosition(posX, posY); // Actualiza la posición física del sprite en el motor de SFML
}

void Personaje::cambiarEstado(Estado nuevoEstado) {
    estadoActual = nuevoEstado;
    relojEstado.restart(); // Reiniciar el cronómetro para los estados temporales

    // Modificaciones de color instantáneas según el estado
    if (estadoActual == Estado::ATACANDO) {
        sprite.setColor(sf::Color(200, 255, 200)); // Tinte verde/celeste claro al atacar
    }
}

void Personaje::recibirDanio(int cantidad) {
    vida -= cantidad;
    if (vida < 0) vida = 0;

    if (vida == 0) {
        cambiarEstado(Estado::DERROTADO);
    } else {
        cambiarEstado(Estado::HERIDO);
    }
}

int Personaje::getVida() const {
    return vida;
}

Estado Personaje::getEstado() const {
    return estadoActual;
}

void Personaje::actualizar() {
    // 1. Controlar la duración del estado ATACANDO (0.25 segundos)
    if (estadoActual == Estado::ATACANDO && relojEstado.getElapsedTime().asSeconds() > 0.25f) {
        estadoActual = Estado::IDLE;
        sprite.setColor(sf::Color::White); // Restaurar color original
    }
    
    // 2. Controlar la duración del estado HERIDO (0.20 segundos)
    if (estadoActual == Estado::HERIDO && relojEstado.getElapsedTime().asSeconds() > 0.20f) {
        estadoActual = Estado::IDLE;
        sprite.setColor(sf::Color::White); // Restaurar color original
    }

    // 3. Comportamiento en estado DERROTADO
    if (estadoActual == Estado::DERROTADO) {
        sprite.setColor(sf::Color(255, 100, 100, 150)); // Rojo semitransparente
        
        // Rotar el sprite 90 grados para simular que cae noqueado al piso
        // El J1 rota hacia la derecha y el J2 hacia la izquierda
        if (sprite.getRotation() == 0.f) {
            sprite.setRotation(90.f * (esJugador1 ? 1.f : -1.f));
        }
    }
    
    // 4. Forzar el color rojo brillante frame a frame mientras esté herido
    if (estadoActual == Estado::HERIDO) {
        sprite.setColor(sf::Color::Red);
    }
}

void Personaje::dibujar(sf::RenderWindow& ventana) {
    ventana.draw(sprite);
}