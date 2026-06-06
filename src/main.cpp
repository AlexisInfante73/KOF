#include <SFML/Graphics.hpp>
#include "combatemusical.h"
#include <iostream>
#include <vector>
#include <string>

int main() {
    // 1. Crear la ventana principal con el nombre del juego
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Combate Musical KOF");
    window.setFramerateLimit(60);

    // 2. Instanciar la clase del combate
    CombateMusical combate;
    
    // --- CONTROL DE FLUJO Y ESTADOS ---
    int estadoActual = 0; 
    int medallasElegidas = 1;
    int opcionMenuSeleccionada = 0; 

    // --- CARGA DE TEXTURA DE FONDO (assets/images/Juego1.png) ---
    sf::Texture texturaFondo;
    sf::Sprite spriteFondo;
    bool tieneFondo = false;

    if (texturaFondo.loadFromFile("assets/images/Juego1.png")) {
        tieneFondo = true;
    }
    else if (texturaFondo.loadFromFile("../assets/images/Juego1.png")) {
        tieneFondo = true;
    }
    else if (texturaFondo.loadFromFile("Juego1.png")) {
        tieneFondo = true;
    }

    if (tieneFondo) {
        spriteFondo.setTexture(texturaFondo);
        float escalaX = 1280.f / texturaFondo.getSize().x;
        float escalaY = 720.f / texturaFondo.getSize().y;
        spriteFondo.setScale(escalaX, escalaY);
        // Dejamos el fondo brillante e impecable
        spriteFondo.setColor(sf::Color(255, 255, 255, 255)); 
    } else {
        std::cout << "ALERTA: No se pudo cargar 'assets/images/Juego1.png'" << std::endl;
    }

    // --- FUENTE DEL JUEGO ---
    sf::Font fuenteJuego;
    bool tieneFuente = false;

    if (fuenteJuego.loadFromFile("assets/images/COMIC.TTF")) { 
        tieneFuente = true;
    }
    else if (fuenteJuego.loadFromFile("../assets/images/COMIC.TTF")) {
        tieneFuente = true;
    }
    else if (fuenteJuego.loadFromFile("assets/fonts/COMIC.TTF")) {
        tieneFuente = true;
    }
    else if (fuenteJuego.loadFromFile("../assets/fonts/COMIC.TTF")) {
        tieneFuente = true;
    }
    else if (fuenteJuego.loadFromFile("COMIC.TTF")) {
        tieneFuente = true;
    }

    // --- DECLARACIÓN DE TODOS LOS TEXTOS ---
    sf::Text txtOpcionAventura("Iniciar Aventura", fuenteJuego, 32);
    sf::Text txtOpcionComoJugar("Como Jugar", fuenteJuego, 32);
    sf::Text txtOpcionConfig("Configuracion", fuenteJuego, 32);
    
    sf::Text txtMenuInstrucciones("Usa Flechas [Arriba / Abajo] y presiona [ENTER] para seleccionar", fuenteJuego, 18);
    txtMenuInstrucciones.setFillColor(sf::Color(220, 220, 220));

    sf::Text txtComoJugarTitulo("COMO JUGAR", fuenteJuego, 40);
    txtComoJugarTitulo.setFillColor(sf::Color::Cyan);
    sf::Text txtComoJugarContenido(
        "Presiona las teclas correspondientes al ritmo de las notas:\n\n"
        " Carril 1: Tecla [ A ]          Carril 2: Tecla [ S ]\n"
        " Carril 3: Tecla [ J ]          Carril 4: Tecla [ K ]\n\n"
        "¡Consigue el mayor combo posible para cargar tu ataque especial!\n\n"
        "Presiona [ ESCAPE ] para regresar al Menu Principal.", fuenteJuego, 24);
    txtComoJugarContenido.setFillColor(sf::Color::White);

    sf::Text txtConfigTitulo("CONFIGURACION", fuenteJuego, 40);
    txtConfigTitulo.setFillColor(sf::Color::Magenta);
    sf::Text txtConfigContenido(
        "Ajustes de Sistema:\n\n"
        " - Resolucion: 1280x720 (Por Defecto)\n"
        " - Sonido / Audio: Habilitado\n"
        " - Tasa de refresco: 60 FPS fijos\n\n"
        "Presiona [ ESCAPE ] para regresar al Menu Principal.", fuenteJuego, 24);
    txtConfigContenido.setFillColor(sf::Color::White);

    sf::Text txtNombresTitulo("REGISTRO DE JUGADORES", fuenteJuego, 40);
    txtNombresTitulo.setFillColor(sf::Color::Cyan);
    sf::Text txtNombresSub("Nombres registrados automaticamente por defecto.\nPresiona ENTER para elegir personajes.", fuenteJuego, 26);
    txtNombresSub.setFillColor(sf::Color::White);

    sf::Text txtPersonajesTitulo("SELECCION DE PERSONAJES (12 DISPONIBLES)", fuenteJuego, 40);
    txtPersonajesTitulo.setFillColor(sf::Color::Magenta);
    sf::Text txtPersonajesSub("Equipos equilibrados cargados con exito.\nPresiona ENTER para seleccionar la dificultad.", fuenteJuego, 26);
    txtPersonajesSub.setFillColor(sf::Color::White);

    sf::Text txtMedallasTitulo("SELECCIONA LA DIFICULTAD", fuenteJuego, 40);
    txtMedallasTitulo.setFillColor(sf::Color::Yellow);
    sf::Text txtContadorMedallas("", fuenteJuego, 34);
    txtContadorMedallas.setFillColor(sf::Color::White);
    sf::Text txtMedallasInstrucciones("Usa Flechas [Arriba / Abajo] para cambiar.\nPresiona [ENTER] para iniciar el combate.", fuenteJuego, 20);
    txtMedallasInstrucciones.setFillColor(sf::Color(180, 180, 180));

    bool flechaArribaPresionada = false;
    bool flechaAbajoPresionada = false;

    // --- BUCLE PRINCIPAL DEL JUEGO ---
    while (window.isOpen()) {
        sf::Event evento;
        
        while (window.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed) {
                window.close();
            }
            
            if (evento.type == sf::Event::KeyReleased) {
                if (evento.key.code == sf::Keyboard::Return) {
                    if (estadoActual == 0) {
                        if (opcionMenuSeleccionada == 0) estadoActual = 1;      
                        else if (opcionMenuSeleccionada == 1) estadoActual = 5; 
                        else if (opcionMenuSeleccionada == 2) estadoActual = 6; 
                    }
                    else if (estadoActual == 1) {
                        combate.configurarNombres("JUGADOR 1", "JUGADOR 2");
                        estadoActual = 2; 
                    }
                    else if (estadoActual == 2) {
                        std::vector<std::string> equipo1 = {"Kyo", "Benimaru", "Goro"};
                        std::vector<std::string> equipo2 = {"Iori", "Mature", "Vice"};
                        combate.configurarEquipos(equipo1, equipo2);
                        estadoActual = 3; 
                    }
                    else if (estadoActual == 3) {
                        combate.reiniciarRelojes(medallasElegidas);
                        estadoActual = 4; 
                    }
                }
            }
        }

        bool teclaArriba = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
        bool teclaAbajo = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            if (estadoActual == 5 || estadoActual == 6) {
                estadoActual = 0; 
            }
        }

        window.clear(sf::Color::Black);

        // --- MAQUINA DE RENDERIZADO POR ESTADOS ---
        if (estadoActual == 0) {
            if (tieneFondo) {
                window.draw(spriteFondo);
            }

            if (teclaArriba) {
                if (!flechaArribaPresionada) {
                    opcionMenuSeleccionada--;
                    if (opcionMenuSeleccionada < 0) opcionMenuSeleccionada = 2;
                    flechaArribaPresionada = true;
                }
            } else { flechaArribaPresionada = false; }

            if (teclaAbajo) {
                if (!flechaAbajoPresionada) {
                    opcionMenuSeleccionada++;
                    if (opcionMenuSeleccionada > 2) opcionMenuSeleccionada = 0;
                    flechaAbajoPresionada = true;
                }
            } else { flechaAbajoPresionada = false; }

            txtOpcionAventura.setFillColor(opcionMenuSeleccionada == 0 ? sf::Color::Cyan : sf::Color::White);
            txtOpcionComoJugar.setFillColor(opcionMenuSeleccionada == 1 ? sf::Color::Cyan : sf::Color::White);
            txtOpcionConfig.setFillColor(opcionMenuSeleccionada == 2 ? sf::Color::Cyan : sf::Color::White);

            txtOpcionAventura.setString(opcionMenuSeleccionada == 0 ? "> Iniciar Aventura <" : "Iniciar Aventura");
            txtOpcionComoJugar.setString(opcionMenuSeleccionada == 1 ? "> Como Jugar <" : "Como Jugar");
            txtOpcionConfig.setString(opcionMenuSeleccionada == 2 ? "> Configuracion <" : "Configuracion");

            // --- REPOSICIONAMIENTO HACIA ABAJO (Evita que se encime con el título) ---
            txtOpcionAventura.setPosition(1280.f / 2.f - txtOpcionAventura.getLocalBounds().width / 2.f, 490.f);
            txtOpcionComoJugar.setPosition(1280.f / 2.f - txtOpcionComoJugar.getLocalBounds().width / 2.f, 545.f);
            txtOpcionConfig.setPosition(1280.f / 2.f - txtOpcionConfig.getLocalBounds().width / 2.f, 600.f);
            txtMenuInstrucciones.setPosition(1280.f / 2.f - txtMenuInstrucciones.getLocalBounds().width / 2.f, 675.f);

            window.draw(txtOpcionAventura);
            window.draw(txtOpcionComoJugar);
            window.draw(txtOpcionConfig);
            window.draw(txtMenuInstrucciones);
        }
        else if (estadoActual == 1) {
            if (tieneFondo) window.draw(spriteFondo);
            txtNombresTitulo.setPosition(1280.f / 2.f - txtNombresTitulo.getLocalBounds().width / 2.f, 200.f);
            txtNombresSub.setPosition(1280.f / 2.f - txtNombresSub.getLocalBounds().width / 2.f, 350.f);
            window.draw(txtNombresTitulo);
            window.draw(txtNombresSub);
        }
        else if (estadoActual == 2) {
            if (tieneFondo) window.draw(spriteFondo);
            txtPersonajesTitulo.setPosition(1280.f / 2.f - txtPersonajesTitulo.getLocalBounds().width / 2.f, 200.f);
            txtPersonajesSub.setPosition(1280.f / 2.f - txtPersonajesSub.getLocalBounds().width / 2.f, 350.f);
            window.draw(txtPersonajesTitulo);
            window.draw(txtPersonajesSub);
        }
        else if (estadoActual == 3) {
            if (tieneFondo) window.draw(spriteFondo);
            if (teclaArriba) {
                if (!flechaArribaPresionada) {
                    if (medallasElegidas < 10) medallasElegidas++;
                    flechaArribaPresionada = true;
                }
            } else { flechaArribaPresionada = false; }

            if (teclaAbajo) {
                if (!flechaAbajoPresionada) {
                    if (medallasElegidas > 1) medallasElegidas--;
                    flechaAbajoPresionada = true;
                }
            } else { flechaAbajoPresionada = false; }

            int vVisual = 1 + ((medallasElegidas - 1) * 2);
            std::string tipoMedalla = (medallasElegidas == 1) ? " MEDALLA" : " MEDALLAS";
            txtContadorMedallas.setString("<  " + std::to_string(medallasElegidas) + tipoMedalla + "  (Velocidad: V." + std::to_string(vVisual) + ")  >");

            txtMedallasTitulo.setPosition(1280.f / 2.f - txtMedallasTitulo.getLocalBounds().width / 2.f, 180.f);
            txtContadorMedallas.setPosition(1280.f / 2.f - txtContadorMedallas.getLocalBounds().width / 2.f, 320.f);
            txtMedallasInstrucciones.setPosition(1280.f / 2.f - txtMedallasInstrucciones.getLocalBounds().width / 2.f, 480.f);

            window.draw(txtMedallasTitulo);
            window.draw(txtContadorMedallas);
            window.draw(txtMedallasInstrucciones);
        }
        else if (estadoActual == 4) {
            combate.procesarEntrada();
            combate.actualizar();
            combate.dibujar(window);
        }
        else if (estadoActual == 5) {
            txtComoJugarTitulo.setPosition(1280.f / 2.f - txtComoJugarTitulo.getLocalBounds().width / 2.f, 120.f);
            txtComoJugarContenido.setPosition(1280.f / 2.f - txtComoJugarContenido.getLocalBounds().width / 2.f, 260.f);
            window.draw(txtComoJugarTitulo);
            window.draw(txtComoJugarContenido);
        }
        else if (estadoActual == 6) {
            txtConfigTitulo.setPosition(1280.f / 2.f - txtConfigTitulo.getLocalBounds().width / 2.f, 120.f);
            txtConfigContenido.setPosition(1280.f / 2.f - txtConfigContenido.getLocalBounds().width / 2.f, 260.f);
            window.draw(txtConfigTitulo);
            window.draw(txtConfigContenido);
        }

        window.display();
    }

    return 0;
}