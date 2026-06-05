#include <SFML/Graphics.hpp>
#include "combatemusical.h"
#include <iostream>
#include <vector>
#include <fstream>

// Función auxiliar para logging
void Log(const std::string& mensaje) {
    std::ofstream log_file("game.log", std::ios::app);
    log_file << mensaje << std::endl;
    log_file.close();
    std::cout << mensaje << std::endl;
}

enum EstadoJuego { MENU, JUGANDO, COMO_JUGAR, CONFIGURACION, FIN_PARTIDA };
enum EstadoPersonaje { IDLE, ATACANDO };

// Cabecera modificada para corregir el enlazado en Windows de forma automatica
int main(int argc, char* argv[])
{
    Log("[INICIO] Iniciando KOF x OsuMania...");
    
    sf::RenderWindow window(sf::VideoMode(800, 600), "KOF x OsuMania - Leyenda del Ritmo");
    window.setFramerateLimit(60);
    window.setPosition(sf::Vector2i(100, 100)); // Posición explícita para asegurar que sea visible
    
    Log("[OK] Ventana creada: 800x600");

    // ======================================================================
    // 🐉 CARGA DIRECTA DE SPRITES (NARUTO Y SASUKE)
    // ======================================================================
    std::string charP1 = "naruto";
    std::string charP2 = "sasuke";

    sf::Texture texP1, texP2;
    bool usarSpriteP1 = false, usarSpriteP2 = false;

    // Intentar cargar la imagen del Jugador 1
    if (texP1.loadFromFile("assets/images/" + charP1 + ".png")) {
        usarSpriteP1 = true;
        std::cout << "[OK] Imagen de P1 cargada con exito." << std::endl;
    } else {
        std::cout << "[AVISO] No se encontro assets/images/" + charP1 + ".png. Usando cuadro de respaldo." << std::endl;
    }

    // Intentar cargar la imagen del Jugador 2
    if (texP2.loadFromFile("assets/images/" + charP2 + ".png")) {
        usarSpriteP2 = true;
        std::cout << "[OK] Imagen de P2 cargada con exito." << std::endl;
    } else {
        std::cout << "[AVISO] No se encontro assets/images/" + charP2 + ".png. Usando cuadro de respaldo." << std::endl;
    }

    sf::Sprite p1Sprite, p2Sprite;

    if (usarSpriteP1) {
        p1Sprite.setTexture(texP1);
        p1Sprite.setPosition(40.f, 320.f);
        p1Sprite.setScale(1.5f, 1.5f);
    }

    if (usarSpriteP2) {
        p2Sprite.setTexture(texP2);
        p2Sprite.setPosition(760.f, 320.f);
        p2Sprite.setScale(-1.5f, 1.5f); // Volteo en espejo para que mire al P1
    }

    // Cuadrados de respaldo si no existen los PNGs en esta carpeta
    sf::RectangleShape p1Box(sf::Vector2f(70.f, 120.f)); 
    p1Box.setFillColor(sf::Color::Cyan); 
    p1Box.setPosition(60.f, 340.f); 

    sf::RectangleShape p2Box(sf::Vector2f(70.f, 120.f)); 
    p2Box.setFillColor(sf::Color::Magenta); 
    p2Box.setPosition(670.f, 340.f); 

    // Variables de estado de control de personajes
    EstadoPersonaje p1Estado = IDLE; sf::Clock p1Reloj; 
    EstadoPersonaje p2Estado = IDLE; sf::Clock p2Reloj; 

    int comboP1 = 0; int scoreP1 = 0;
    int comboP2 = 0; int scoreP2 = 0;

    // Instancia de Combatemusical
    Combatemusical combate;

    // Configurar las 4 pistas verticales estilo Osu!Mania
    sf::RectangleShape pistesOsu[4];
    float xCentroInicio = combate.getXCentroInicio(); 
    for (int i = 0; i < 4; i++) {
        pistesOsu[i].setSize(sf::Vector2f(45.f, 480.f));
        pistesOsu[i].setFillColor(sf::Color(35, 35, 45));
        pistesOsu[i].setOutlineColor(sf::Color(60, 60, 75));
        pistesOsu[i].setOutlineThickness(1.f);
        pistesOsu[i].setPosition(xCentroInicio + (i * 50), 20.f);
    }

    // Linea verde de juicio de golpes
    sf::RectangleShape lineaJuicio(sf::Vector2f(195.f, 6.f));
    lineaJuicio.setFillColor(sf::Color::Green);
    lineaJuicio.setPosition(xCentroInicio, 500.f);

    float tiempoLimitePartida = 120.f; 
    float tiempoLimiteRonda = 15.f;   
    sf::Clock relojPartida;
    sf::Clock relojRonda;
    int jugadorTurnoActual = 1; 

    // Carga de la fuente del sistema para textos
    sf::Font font;
    bool fontCargada = font.loadFromFile("assets/fonts/COMIC.TTF");
    if (!fontCargada) {
        Log("Advertencia: No se pudo cargar assets/fonts/COMIC.TTF. Los textos pueden no aparecer correctamente.");
    }

    // Textos del menu principal (solo se crean si la fuente se cargó)
    sf::Text txtTitulo;
    sf::Text opcEmpezar;
    sf::Text opcComoJugar;
    sf::Text opcConfiguracion;
    
    if (fontCargada) {
        txtTitulo.setFont(font);
        txtTitulo.setString("Leyenda del Ritmo");
        txtTitulo.setCharacterSize(55);
        txtTitulo.setFillColor(sf::Color::Yellow);
        txtTitulo.setPosition(160.f, 80.f);

        opcEmpezar.setFont(font);
        opcEmpezar.setString("Empezar a jugar");
        opcEmpezar.setCharacterSize(32);
        opcEmpezar.setPosition(280.f, 260.f);

        opcComoJugar.setFont(font);
        opcComoJugar.setString("Como jugar");
        opcComoJugar.setCharacterSize(32);
        opcComoJugar.setPosition(280.f, 340.f);

        opcConfiguracion.setFont(font);
        opcConfiguracion.setString("Configuracion");
        opcConfiguracion.setCharacterSize(32);
        opcConfiguracion.setPosition(280.f, 420.f);
    }

    int opcionSeleccionada = 0; 

    // Interfaz de juego
    sf::RectangleShape panelInferior(sf::Vector2f(800.f, 75.f));
    panelInferior.setFillColor(sf::Color(15, 15, 22));
    panelInferior.setPosition(0.f, 525.f);

    sf::Text textoCronometro;
    sf::Text textoTurnoAviso;
    sf::Text textoMarcadorP1;
    sf::Text textoMarcadorP2;
    
    if (fontCargada) {
        textoCronometro.setFont(font);
        textoCronometro.setString("");
        textoCronometro.setCharacterSize(26);
        textoCronometro.setPosition(330.f, 545.f); 
        textoCronometro.setFillColor(sf::Color::White);

        textoTurnoAviso.setFont(font);
        textoTurnoAviso.setString("");
        textoTurnoAviso.setCharacterSize(26);
        textoTurnoAviso.setPosition(285.f, 25.f);  

        textoMarcadorP1.setFont(font);
        textoMarcadorP1.setString("");
        textoMarcadorP1.setCharacterSize(22);
        textoMarcadorP1.setPosition(30.f, 40.f);
        textoMarcadorP1.setFillColor(sf::Color::Cyan);

        textoMarcadorP2.setFont(font);
        textoMarcadorP2.setString("");
        textoMarcadorP2.setCharacterSize(22);
        textoMarcadorP2.setPosition(610.f, 40.f); 
        textoMarcadorP2.setFillColor(sf::Color(255, 0, 255));
    }

    EstadoJuego estadoActual = MENU;

    Log("[OK] Todas las variables inicializadas");
    Log("[INFO] Entrando al loop principal...");

    // Loop Principal del Juego
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // Logica de controles en el Menu
            if (estadoActual == MENU && event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Up) opcionSeleccionada = (opcionSeleccionada - 1 + 3) % 3;
                if (event.key.code == sf::Keyboard::Down) opcionSeleccionada = (opcionSeleccionada + 1) % 3;
                if (event.key.code == sf::Keyboard::Enter) {
                    if (opcionSeleccionada == 0) { 
                        estadoActual = JUGANDO; 
                        relojPartida.restart(); 
                        relojRonda.restart();
                        jugadorTurnoActual = 1;
                        scoreP1 = 0; scoreP2 = 0; comboP1 = 0; comboP2 = 0;
                    }
                    else if (opcionSeleccionada == 1) estadoActual = COMO_JUGAR;
                    else if (opcionSeleccionada == 2) estadoActual = CONFIGURACION;
                }
            }
            // Regresar al menu con ESC desde sub-menus
            else if ((estadoActual == COMO_JUGAR || estadoActual == CONFIGURACION) && event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) estadoActual = MENU;
            }
            // Controles de ritmo durante la partida
            else if (estadoActual == JUGANDO && event.type == sf::Event::KeyPressed)
            {
                bool aumentoCombo = false;

                if (jugadorTurnoActual == 1) {
                    int rielJ1 = -1;
                    if (event.key.code == sf::Keyboard::A) rielJ1 = 0;
                    if (event.key.code == sf::Keyboard::S) rielJ1 = 1;
                    if (event.key.code == sf::Keyboard::K) rielJ1 = 2;
                    if (event.key.code == sf::Keyboard::L) rielJ1 = 3;

                    if (rielJ1 != -1) {
                        p1Estado = ATACANDO; p1Reloj.restart();
                        int puntos = combate.procesarGolpe(rielJ1, 1, aumentoCombo);
                        if (aumentoCombo) { comboP1++; scoreP1 += puntos; } else { comboP1 = 0; }
                    }
                }
                else if (jugadorTurnoActual == 2) {
                    int rielJ2 = -1;
                    if (event.key.code == sf::Keyboard::Left)  rielJ2 = 0;
                    if (event.key.code == sf::Keyboard::Down)  rielJ2 = 1;
                    if (event.key.code == sf::Keyboard::Up)    rielJ2 = 2;
                    if (event.key.code == sf::Keyboard::Right) rielJ2 = 3;

                    if (rielJ2 != -1) {
                        p2Estado = ATACANDO; p2Reloj.restart();
                        int puntos = combate.procesarGolpe(rielJ2, 2, aumentoCombo);
                        if (aumentoCombo) { comboP2++; scoreP2 += puntos; } else { comboP2 = 0; }
                    }
                }
            }
        }

        // Actualizaciones logicas constantes
        if (estadoActual == MENU && fontCargada) {
            opcEmpezar.setFillColor(opcionSeleccionada == 0 ? sf::Color::Red : sf::Color::White);
            opcComoJugar.setFillColor(opcionSeleccionada == 1 ? sf::Color::Red : sf::Color::White);
            opcConfiguracion.setFillColor(opcionSeleccionada == 2 ? sf::Color::Red : sf::Color::White);
        }
        else if (estadoActual == JUGANDO)
        {
            if (relojPartida.getElapsedTime().asSeconds() > tiempoLimitePartida) estadoActual = FIN_PARTIDA;

            // Cambio automatico de turnos cada 15 segundos
            float tiempoRondaRestante = tiempoLimiteRonda - relojRonda.getElapsedTime().asSeconds();
            if (tiempoRondaRestante <= 0.f) {
                jugadorTurnoActual = (jugadorTurnoActual == 1) ? 2 : 1;
                relojRonda.restart();
                tiempoRondaRestante = tiempoLimiteRonda;
            }

            combate.actualizar(jugadorTurnoActual);

            if (p1Estado == ATACANDO && p1Reloj.getElapsedTime().asSeconds() > 0.15f) p1Estado = IDLE;
            if (p2Estado == ATACANDO && p2Reloj.getElapsedTime().asSeconds() > 0.15f) p2Estado = IDLE;

            if (fontCargada) {
                textoCronometro.setString("TIEMPO: " + std::to_string(static_cast<int>(tiempoRondaRestante)) + "s");
                textoTurnoAviso.setString("ROUND JUGADOR " + std::to_string(jugadorTurnoActual));
                textoTurnoAviso.setFillColor(jugadorTurnoActual == 1 ? sf::Color::Cyan : sf::Color(255, 0, 255));
                textoMarcadorP1.setString("P1 SCORE: " + std::to_string(scoreP1) + "\nCOMBO: " + std::to_string(comboP1));
                textoMarcadorP2.setString("P2 SCORE: " + std::to_string(scoreP2) + "\nCOMBO: " + std::to_string(comboP2));
            }
        }

        // Renderizado en pantalla
        window.clear(sf::Color(21, 21, 30)); 

        if (estadoActual == MENU && fontCargada) {
            window.draw(txtTitulo); window.draw(opcEmpezar); window.draw(opcComoJugar); window.draw(opcConfiguracion);
        }
        else if (estadoActual == COMO_JUGAR && fontCargada) {
            sf::Text txtInfo;
            txtInfo.setFont(font);
            txtInfo.setString("MODO COMPETITIVO POR TURNOS\n\nCada jugador tiene 15s en su turno para sumar puntos.\n\nCONTROLES:\nP1: Teclas A, S, K, L\nP2: Flechas Direccionales\n\nPresiona ESC para regresar.");
            txtInfo.setCharacterSize(22);
            txtInfo.setPosition(100.f, 180.f); 
            window.draw(txtInfo);
        }
        else if (estadoActual == CONFIGURACION && fontCargada) {
            sf::Text txtConfig;
            txtConfig.setFont(font);
            txtConfig.setString("CONFIGURACION GENERAL\n\nResolucion: 800x600 Fija\nLibreria: SFML 2.6\n\nPresiona ESC para regresar al menu.");
            txtConfig.setCharacterSize(24);
            txtConfig.setPosition(150.f, 220.f); 
            window.draw(txtConfig);
        }
        else if (estadoActual == JUGANDO) {
            for (int i = 0; i < 4; i++) window.draw(pistesOsu[i]);
            window.draw(lineaJuicio); 

            combate.dibujar(window); 
            
            // Dibujar imagen o caja de seguridad según disponibilidad
            if (usarSpriteP1) window.draw(p1Sprite); else window.draw(p1Box);
            if (usarSpriteP2) window.draw(p2Sprite); else window.draw(p2Box);
            
            window.draw(panelInferior);
            if (fontCargada) {
                window.draw(textoMarcadorP1); window.draw(textoMarcadorP2); 
                window.draw(textoCronometro); window.draw(textoTurnoAviso);
            }
        }
        else if (estadoActual == FIN_PARTIDA && fontCargada) {
            std::string ganador = "¡EMPATE TECNICO!";
            if (scoreP1 > scoreP2) ganador = "¡EL GANADOR ES EL JUGADOR 1!";
            else if (scoreP2 > scoreP1) ganador = "¡EL GANADOR ES EL JUGADOR 2!";

            sf::Text txtFin;
            txtFin.setFont(font);
            txtFin.setString("PARTIDA COMPLETADA\n\n" + ganador + "\n\nPresiona ENTER para ir al menu principal.");
            txtFin.setCharacterSize(32);
            txtFin.setFillColor(sf::Color::Yellow); 
            txtFin.setPosition(120.f, 200.f);
            window.draw(txtFin);
            
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) estadoActual = MENU;
        }

        window.display(); 
    }
    return 0;
}