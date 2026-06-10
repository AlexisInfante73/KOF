#include "combatemusical.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

CombateMusical::CombateMusical() {
    // Inicializar generador aleatorio
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    // Elegir un escenario aleatorio del 1 al 4
    int escenarioAleatorio = (std::rand() % 4) + 1;
    std::string rutaFondo = "assets/images/fondo_batalla_" + std::to_string(escenarioAleatorio) + ".png";

    // Cargar la textura del escenario de batalla de fondo
    if (!texturaEscenario.loadFromFile(rutaFondo)) {
        std::cout << "[ERROR]: No se pudo cargar el fondo en: " << rutaFondo << std::endl;
        // Intento alternativo por si ejecutas desde otra carpeta
        if (!texturaEscenario.loadFromFile("../" + rutaFondo)) {
            std::cout << "[ERROR CRÍTICO]: No se encontró el fondo de batalla." << std::endl;
        }
    }

    // Configurar el Sprite para que cubra la pantalla fija de 1280x720
    spriteEscenario.setTexture(texturaEscenario);
    spriteEscenario.setScale(
        1280.f / texturaEscenario.getSize().x, 
        720.f / texturaEscenario.getSize().y
    );

    // Configurar las barras de vida arriba en la interfaz estática
    barraVidaJ1.setSize(sf::Vector2f(400.f, 30.f));
    barraVidaJ1.setFillColor(sf::Color::Green);
    barraVidaJ1.setPosition(50.f, 40.f);

    barraVidaJ2.setSize(sf::Vector2f(400.f, 30.f));
    barraVidaJ2.setFillColor(sf::Color::Green);
    barraVidaJ2.setPosition(1280.f - 450.f, 40.f);
}

void CombateMusical::procesarEntrada(sf::Event& evento) {
    // Aquí puedes capturar teclas específicas del juego si lo necesitas más adelante
}

void CombateMusical::reiniciarRelojes(int medallas) {
    std::cout << "[INFO]: Pelea iniciada con agresividad nivel: " << medallas << std::endl;
}

void CombateMusical::actualizar() {
    // Lógica interna del juego (movimiento simple o tiempos) sin tocar funciones rotas de Personaje
}

void CombateMusical::dibujar(sf::RenderWindow& window) {
    // 1. Dibujar el fondo del escenario elegido fijamente en la pantalla completa
    window.draw(spriteEscenario);

    // 2. Dibujar la interfaz superior (las barras de vida estables)
    window.draw(barraVidaJ1);
    window.draw(barraVidaJ2);
}