#include <SFML/Graphics.hpp>
#include "combatemusical.h"
#include <iostream>
#include <vector>
#include <string>

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Leyendas del Ritmo - Arena 2D Completa");
    window.setFramerateLimit(60);

    CombateMusical combate;
    int estadoActual = 0; 
    int medallasElegidas = 1;
    int opcionMenuSeleccionada = 0; 

    // --- CARGAR FUENTE ---
    sf::Font fuenteJuego;
    if (!fuenteJuego.loadFromFile("assets/fonts/COMIC.TTF")) {
        std::cout << "[ADVERTENCIA]: No se cargó assets/fonts/COMIC.TTF en ruta directa." << std::endl;
        if (!fuenteJuego.loadFromFile("../assets/fonts/COMIC.TTF")) {
            std::cout << "[ERROR CRÍTICO]: No se encontró la tipografía." << std::endl;
        }
    }

    // --- CARGAR FONDO DE MENÚS (Juego1.png) ---
    sf::Texture texturaJuego;
    sf::Sprite spriteFondo;
    bool tieneFondo = false;

    if (texturaJuego.loadFromFile("assets/images/Juego1.png")) {
        tieneFondo = true;
        spriteFondo.setTexture(texturaJuego);
        spriteFondo.setScale(1280.f / texturaJuego.getSize().x, 720.f / texturaJuego.getSize().y);
    } 
    else if (texturaJuego.loadFromFile("../assets/images/Juego1.png")) {
        tieneFondo = true;
        spriteFondo.setTexture(texturaJuego);
        spriteFondo.setScale(1280.f / texturaJuego.getSize().x, 720.f / texturaJuego.getSize().y);
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

    sf::Text txtMedallasTitulo("DIFICULTAD DEL RIVAL", fuenteJuego, 40);
    txtMedallasTitulo.setFillColor(sf::Color::Yellow);
    sf::Text txtContadorMedallas("", fuenteJuego, 34);
    txtContadorMedallas.setFillColor(sf::Color::White);
    sf::Text txtMedallasInstrucciones("Usa Flechas [Arriba / Abajo] para calibrar la agresividad del bot.\nPresiona [ENTER] para iniciar el combate en el suelo.", fuenteJuego, 20);
    txtMedallasInstrucciones.setFillColor(sf::Color(180, 180, 180));

    bool flechaArribaPresionada = false;
    bool flechaAbajoPresionada = false;

    // --- BUCLE PRINCIPAL ---
    while (window.isOpen()) {
        sf::Event evento;
        while (window.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed) window.close();
            
            // Redirigir eventos de salto y golpes a la arena de pelea
            if (estadoActual == 4) {
                combate.procesarEntrada(evento);
            }
            
            if (evento.type == sf::Event::KeyReleased && evento.key.code == sf::Keyboard::Return) {
                if (estadoActual == 0) {
                    if (opcionMenuSeleccionada == 0) estadoActual = 1;      
                    else if (opcionMenuSeleccionada == 1) estadoActual = 5; 
                    else if (opcionMenuSeleccionada == 2) estadoActual = 6; 
                }
                else if (estadoActual == 1) {
                    estadoActual = 2; 
                }
                else if (estadoActual == 2) {
                    estadoActual = 3; 
                }
                else if (estadoActual == 3) {
                    combate.reiniciarRelojes(medallasElegidas);
                    estadoActual = 4; 
                }
            }
        }

        bool teclaArriba = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
        bool teclaAbajo = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && (estadoActual == 5 || estadoActual == 6 || estadoActual == 4)) {
            estadoActual = 0; 
        }

        // Forzar la vista estática normal al inicio de cada frame
        window.setView(window.getDefaultView());
        window.clear(sf::Color::Black);

        if (estadoActual == 0) {
            if (tieneFondo) { spriteFondo.setColor(sf::Color(255, 255, 255, 255)); window.draw(spriteFondo); }

            if (teclaArriba) {
                if (!flechaArribaPresionada) { opcionMenuSeleccionada = (opcionMenuSeleccionada - 1 + 3) % 3; flechaArribaPresionada = true; }
            } else { flechaArribaPresionada = false; }

            if (teclaAbajo) {
                if (!flechaAbajoPresionada) { opcionMenuSeleccionada = (opcionMenuSeleccionada + 1) % 3; flechaAbajoPresionada = true; }
            } else { flechaAbajoPresionada = false; }

            txtOpcionAventura.setFillColor(opcionMenuSeleccionada == 0 ? sf::Color::Cyan : sf::Color::White);
            txtOpcionComoJugar.setFillColor(opcionMenuSeleccionada == 1 ? sf::Color::Cyan : sf::Color::White);
            txtOpcionConfig.setFillColor(opcionMenuSeleccionada == 2 ? sf::Color::Cyan : sf::Color::White);

            txtOpcionAventura.setString(opcionMenuSeleccionada == 0 ? "> Iniciar Pelea <" : "Iniciar Pelea");
            txtOpcionComoJugar.setString(opcionMenuSeleccionada == 1 ? "> Como Jugar <" : "Como Jugar");
            txtOpcionConfig.setString(opcionMenuSeleccionada == 2 ? "> Configuracion <" : "Configuracion");

            txtOpcionAventura.setPosition(1280.f / 2.f - txtOpcionAventura.getLocalBounds().width / 2.f, 490.f);
            txtOpcionComoJugar.setPosition(1280.f / 2.f - txtOpcionComoJugar.getLocalBounds().width / 2.f, 545.f);
            txtOpcionConfig.setPosition(1280.f / 2.f - txtOpcionConfig.getLocalBounds().width / 2.f, 600.f);
            txtMenuInstrucciones.setPosition(1280.f / 2.f - txtMenuInstrucciones.getLocalBounds().width / 2.f, 675.f);

            window.draw(txtOpcionAventura);
            window.draw(txtOpcionComoJugar);
            window.draw(txtOpcionConfig);
            window.draw(txtMenuInstrucciones);
        }
        else if (estadoActual == 1 || estadoActual == 2 || estadoActual == 3) {
            if (tieneFondo) { spriteFondo.setColor(sf::Color(80, 80, 80, 255)); window.draw(spriteFondo); }

            if (estadoActual == 1) { 
                txtNombresTitulo.setPosition(1280.f / 2.f - txtNombresTitulo.getLocalBounds().width / 2.f, 220.f);
                txtNombresSub.setPosition(1280.f / 2.f - txtNombresSub.getLocalBounds().width / 2.f, 380.f);
                window.draw(txtNombresTitulo);
                window.draw(txtNombresSub);
            }
            else if (estadoActual == 2) { 
                txtPersonajesTitulo.setPosition(1280.f / 2.f - txtPersonajesTitulo.getLocalBounds().width / 2.f, 220.f);
                txtPersonajesSub.setPosition(1280.f / 2.f - txtPersonajesSub.getLocalBounds().width / 2.f, 380.f);
                window.draw(txtPersonajesTitulo);
                window.draw(txtPersonajesSub);
            }
            else if (estadoActual == 3) { 
                if (teclaArriba) {
                    if (!flechaArribaPresionada) { if (medallasElegidas < 5) medallasElegidas++; flechaArribaPresionada = true; }
                } else { flechaArribaPresionada = false; }

                if (teclaAbajo) {
                    if (!flechaAbajoPresionada) { if (medallasElegidas > 1) medallasElegidas--; flechaAbajoPresionada = true; }
                } else { flechaAbajoPresionada = false; }

                std::string nivelIA = "FACIL";
                if (medallasElegidas == 2) nivelIA = "NORMAL";
                if (medallasElegidas == 3) nivelIA = "DIFICIL";
                if (medallasElegidas == 4) nivelIA = "EXPERTO";
                if (medallasElegidas == 5) nivelIA = "PESADILLA";
                txtContadorMedallas.setString("< AGRESIVIDAD DEL ENEMIGO: " + nivelIA + " >");

                txtMedallasTitulo.setPosition(1280.f / 2.f - txtMedallasTitulo.getLocalBounds().width / 2.f, 200.f);
                txtContadorMedallas.setPosition(1280.f / 2.f - txtContadorMedallas.getLocalBounds().width / 2.f, 340.f);
                txtMedallasInstrucciones.setPosition(1280.f / 2.f - txtMedallasInstrucciones.getLocalBounds().width / 2.f, 490.f);

                window.draw(txtMedallasTitulo);
                window.draw(txtContadorMedallas);
                window.draw(txtMedallasInstrucciones);
            }
        }
        else if (estadoActual == 4) { 
            combate.actualizar();
            
            // Forzamos a que el renderizado de la pelea ignore cualquier cámara móvil interna
            window.setView(window.getDefaultView());
            combate.dibujar(window); 
        }
        else if (estadoActual == 5) {
            if (tieneFondo) { spriteFondo.setColor(sf::Color(60, 60, 60, 255)); window.draw(spriteFondo); }
            txtComoJugarTitulo.setPosition(1280.f / 2.f - txtComoJugarTitulo.getLocalBounds().width / 2.f, 100.f);
            txtComoJugarContenido.setPosition(1280.f / 2.f - txtComoJugarContenido.getLocalBounds().width / 2.f, 200.f);
            window.draw(txtComoJugarTitulo);
            window.draw(txtComoJugarContenido);
        }
        else if (estadoActual == 6) {
            if (tieneFondo) { spriteFondo.setColor(sf::Color(60, 60, 60, 255)); window.draw(spriteFondo); }
            txtConfigTitulo.setPosition(1280.f / 2.f - txtConfigTitulo.getLocalBounds().width / 2.f, 120.f);
            txtConfigContenido.setPosition(1280.f / 2.f - txtConfigContenido.getLocalBounds().width / 2.f, 260.f);
            window.draw(txtConfigTitulo);
            window.draw(txtConfigContenido);
        }

        window.display();
    }

    return 0;
}