#include <SFML/Graphics.hpp>
#include "combatemusical.h"
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "KOF x Osu!mania");
    window.setFramerateLimit(60);

    CombateMusical combate;

    sf::Font fuente;
    bool fuenteCargada = true;

    if (!fuente.loadFromFile("assets/fonts/COMIC.TTF")) {
        std::cout << "No se pudo cargar desde: assets/fonts/COMIC.TTF" << std::endl;
        if (!fuente.loadFromFile("../assets/fonts/COMIC.TTF")) {
            fuenteCargada = false;
        }
    }

    sf::Text textoMenu;
    if (fuenteCargada) {
        textoMenu.setFont(fuente);
        textoMenu.setString("EMPEZAR AVENTURA");
        textoMenu.setFillColor(sf::Color::White);
    } else {
        textoMenu.setString("ERROR: No se encontro COMIC.TTF");
        textoMenu.setFillColor(sf::Color::Red);
    }
    
    textoMenu.setCharacterSize(42);
    textoMenu.setPosition(430.f, 550.f); 

    int estadoJuego = 0;

    sf::CircleShape bolitaAzul(35.f);
    bolitaAzul.setFillColor(sf::Color::Blue);
    bolitaAzul.setPosition(80.f, 600.f);

    sf::CircleShape bolitaRoja(35.f);
    bolitaRoja.setFillColor(sf::Color::Red);
    bolitaRoja.setPosition(1130.f, 600.f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter && estadoJuego == 0) {
                    estadoJuego = 1;
                    combate.reiniciarRelojes();
                }
                else if (event.key.code == sf::Keyboard::Escape && estadoJuego == 1) {
                    estadoJuego = 0;
                    combate.limpiarNotas();
                }
            }
        }

        // --- ENTRADA Y ACTUALIZACIÓN DEL JUEGO ---
        if (estadoJuego == 1) {
            combate.procesarEntrada(); // Captura las teclas A, S, J, K en tiempo real
            combate.actualizar();
        }

        window.clear(sf::Color(25, 25, 25));

        if (estadoJuego == 0) {
            window.draw(textoMenu);
        } 
        else if (estadoJuego == 1) {
            combate.dibujar(window);
            window.draw(bolitaAzul);
            window.draw(bolitaRoja);
        }

        window.display();
    }

    return 0;
}