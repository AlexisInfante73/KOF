#include <SFML/Graphics.hpp>
#include "combatemusical.h"
#include <iostream>
#include <vector>
#include <string>

enum class EstadoApp {
    MenuPrincipal,
    RegistroCombatientes,
    SeleccionPersonaje,
    Combate,
    ComoJugar,
    Configuracion
};

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Leyendas del Ritmo - Arena 2D Completa");
    window.setFramerateLimit(60);

    CombateMusical combate;
    EstadoApp estadoActual = EstadoApp::MenuPrincipal;
    int opcionMenuSeleccionada = 0; 

    // --- CARGAR FUENTE ---
    sf::Font fuenteJuego;
    if (!fuenteJuego.loadFromFile("assets/fonts/COMIC.TTF")) {
        std::cout << "[ADVERTENCIA]: No se cargó assets/fonts/COMIC.TTF en ruta directa.\n";
        if (!fuenteJuego.loadFromFile("../assets/fonts/COMIC.TTF")) {
            std::cerr << "[ERROR CRÍTICO]: No se encontró la tipografía.\n";
        }
    }

    // --- CARGAR FONDO DE MENÚS ---
    sf::Texture texturaJuego;
    sf::Sprite spriteFondo;
    bool tieneFondo = false;

    if (texturaJuego.loadFromFile("assets/images/Juego1.png") || 
        texturaJuego.loadFromFile("../assets/images/Juego1.png")) {
        tieneFondo = true;
        spriteFondo.setTexture(texturaJuego);
        if (texturaJuego.getSize().x > 0 && texturaJuego.getSize().y > 0) {
            spriteFondo.setScale(1280.f / texturaJuego.getSize().x, 720.f / texturaJuego.getSize().y);
        }
    } else {
        std::cout << "[ADVERTENCIA]: No se cargó el fondo del menú.\n";
    }

    // --- TEXTOS E INSTRUCCIONES ---
    sf::Text txtOpcionAventura("Iniciar Pelea", fuenteJuego, 32);
    sf::Text txtOpcionComoJugar("Como Jugar", fuenteJuego, 32);
    sf::Text txtOpcionConfig("Configuracion", fuenteJuego, 32);
    
    sf::Text txtMenuInstrucciones("Usa Flechas [Arriba / Abajo] y presiona [ENTER] para confirmar", fuenteJuego, 18);
    txtMenuInstrucciones.setFillColor(sf::Color(200, 200, 200));

    sf::Text txtComoJugarTitulo("CONTROLES DE LA ARENA", fuenteJuego, 40);
    txtComoJugarTitulo.setFillColor(sf::Color::Cyan);
    sf::Text txtComoJugarContenido(
        "Manejo Físico de Combate (Mano Izquierda):\n\n"
        " - Tecla [ D ] : Caminar hacia adelante\n"
        " - Tecla [ A ] : Caminar hacia atras\n"
        " - Tecla [ S ] : Agacharse (Evasion de golpes activa)\n"
        " - [ ESPACIO ] : Saltar (Fisicas con gravedad simulada)\n\n"
        " Acciones de Ataque (Mano Derecha):\n"
        " - Tecla [ J ] : Lanzar golpe directo\n"
        " - Tecla [ K ] : Conectar patada media\n\n"
        "Presiona [ ESCAPE ] para volver al Menu Principal.", fuenteJuego, 24);
    txtComoJugarContenido.setFillColor(sf::Color::White);

    sf::Text txtConfigTitulo("CONFIGURACION DEL MOTOR", fuenteJuego, 40);
    txtConfigTitulo.setFillColor(sf::Color::Magenta);
    sf::Text txtConfigContenido(
        "Especificaciones del Sistema:\n\n"
        " - Renderizado: 1280x720 (Pantalla Completa Activa)\n"
        " - Suelo Físico: Habilitado para luchadores\n"
        " - Escenarios Aleatorios: Activos (4 Fondos Disponibles)\n\n"
        "Presiona [ ESCAPE ] para volver al Menu Principal.", fuenteJuego, 24);
    txtConfigContenido.setFillColor(sf::Color::White);

    sf::Text txtNombresTitulo("REGISTRO DE COMBATIENTES", fuenteJuego, 40);
    txtNombresTitulo.setFillColor(sf::Color::Cyan);
    sf::Text txtNombresSub("Fisicas de suelo y escenarios aleatorios listos.\nPresiona ENTER para continuar.", fuenteJuego, 26);
    txtNombresSub.setFillColor(sf::Color::White);

    sf::Text txtPersonajesTitulo("SELECCION DE PERSONAJE", fuenteJuego, 40);
    txtPersonajesTitulo.setFillColor(sf::Color::Magenta);
    
    sf::Text txtPersonajesSub("Peleador listo.\nPresiona ENTER para ajustar la dificultad de la IA.", fuenteJuego, 26);
    txtPersonajesSub.setFillColor(sf::Color::White);

    // --- BUCLE PRINCIPAL ---
    while (window.isOpen()) {
        sf::Event evento;
        while (window.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed) {
                window.close();
            }
            
            if (estadoActual == EstadoApp::SeleccionPersonaje || estadoActual == EstadoApp::Combate) {
                combate.procesarEntrada(evento);
            }
            
            if (evento.type == sf::Event::KeyPressed) {
                if (evento.key.code == sf::Keyboard::Escape) {
                    if (estadoActual == EstadoApp::ComoJugar || 
                        estadoActual == EstadoApp::Configuracion ||
                        estadoActual == EstadoApp::SeleccionPersonaje ||
                        estadoActual == EstadoApp::Combate) {
                        estadoActual = EstadoApp::MenuPrincipal;
                    }
                }

                if (evento.key.code == sf::Keyboard::Up) {
                    if (estadoActual == EstadoApp::MenuPrincipal) {
                        opcionMenuSeleccionada = (opcionMenuSeleccionada - 1 + 3) % 3;
                    }
                } 
                else if (evento.key.code == sf::Keyboard::Down) {
                    if (estadoActual == EstadoApp::MenuPrincipal) {
                        opcionMenuSeleccionada = (opcionMenuSeleccionada + 1) % 3;
                    }
                }

                if (evento.key.code == sf::Keyboard::Enter) {
                    switch (estadoActual) {
                        case EstadoApp::MenuPrincipal:
                            if (opcionMenuSeleccionada == 0) estadoActual = EstadoApp::RegistroCombatientes;      
                            else if (opcionMenuSeleccionada == 1) estadoActual = EstadoApp::ComoJugar; 
                            else if (opcionMenuSeleccionada == 2) estadoActual = EstadoApp::Configuracion; 
                            break;
                        case EstadoApp::RegistroCombatientes:
                            estadoActual = EstadoApp::SeleccionPersonaje; 
                            break;
                        default:
                            break;
                    }
                }
            }
        }

        // --- RENDERIZADO ---
        window.clear(sf::Color::Black);

        if (estadoActual == EstadoApp::MenuPrincipal) {
            window.setView(window.getDefaultView()); 
            if (tieneFondo) { 
                spriteFondo.setColor(sf::Color(255, 255, 255, 255)); 
                window.draw(spriteFondo); 
            }

            txtOpcionAventura.setFillColor(opcionMenuSeleccionada == 0 ? sf::Color::Cyan : sf::Color::White);
            txtOpcionComoJugar.setFillColor(opcionMenuSeleccionada == 1 ? sf::Color::Cyan : sf::Color::White);
            txtOpcionConfig.setFillColor(opcionMenuSeleccionada == 2 ? sf::Color::Cyan : sf::Color::White);

            txtOpcionAventura.setString(opcionMenuSeleccionada == 0 ? "> Iniciar Pelea <" : "Iniciar Pelea");
            txtOpcionComoJugar.setString(opcionMenuSeleccionada == 1 ? "> Como Jugar <" : "Como Jugar");
            txtOpcionConfig.setString(opcionMenuSeleccionada == 2 ? "> Configuracion <" : "Configuracion");

            float centroX = 1280.f / 2.f;
            txtOpcionAventura.setPosition(centroX - txtOpcionAventura.getLocalBounds().width / 2.f, 490.f);
            txtOpcionComoJugar.setPosition(centroX - txtOpcionComoJugar.getLocalBounds().width / 2.f, 545.f);
            txtOpcionConfig.setPosition(centroX - txtOpcionConfig.getLocalBounds().width / 2.f, 600.f);
            txtMenuInstrucciones.setPosition(centroX - txtMenuInstrucciones.getLocalBounds().width / 2.f, 675.f);

            window.draw(txtOpcionAventura);
            window.draw(txtOpcionComoJugar);
            window.draw(txtOpcionConfig);
            window.draw(txtMenuInstrucciones);
        }
        else if (estadoActual == EstadoApp::RegistroCombatientes) {
            window.setView(window.getDefaultView());
            if (tieneFondo) { 
                spriteFondo.setColor(sf::Color(80, 80, 80, 255)); 
                window.draw(spriteFondo); 
            }
            txtNombresTitulo.setPosition(1280.f / 2.f - txtNombresTitulo.getLocalBounds().width / 2.f, 220.f);
            txtNombresSub.setPosition(1280.f / 2.f - txtNombresSub.getLocalBounds().width / 2.f, 380.f);
            window.draw(txtNombresTitulo);
            window.draw(txtNombresSub);
        }
        else if (estadoActual == EstadoApp::SeleccionPersonaje || estadoActual == EstadoApp::Combate) { 
            combate.actualizar();
            combate.dibujar(window); 
        }
        else if (estadoActual == EstadoApp::ComoJugar) {
            window.setView(window.getDefaultView());
            if (tieneFondo) { 
                spriteFondo.setColor(sf::Color(60, 60, 60, 255)); 
                window.draw(spriteFondo); 
            }
            txtComoJugarTitulo.setPosition(1280.f / 2.f - txtComoJugarTitulo.getLocalBounds().width / 2.f, 100.f);
            txtComoJugarContenido.setPosition(1280.f / 2.f - txtComoJugarContenido.getLocalBounds().width / 2.f, 200.f);
            window.draw(txtComoJugarTitulo);
            window.draw(txtComoJugarContenido);
        }
        else if (estadoActual == EstadoApp::Configuracion) {
            window.setView(window.getDefaultView());
            if (tieneFondo) { 
                spriteFondo.setColor(sf::Color(60, 60, 60, 255)); 
                window.draw(spriteFondo); 
            }
            txtConfigTitulo.setPosition(1280.f / 2.f - txtConfigTitulo.getLocalBounds().width / 2.f, 120.f);
            txtConfigContenido.setPosition(1280.f / 2.f - txtConfigContenido.getLocalBounds().width / 2.f, 260.f);
            window.draw(txtConfigTitulo);
            window.draw(txtConfigContenido);
        }

        window.display();
    }

    return 0;
}