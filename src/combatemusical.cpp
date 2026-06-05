#include "combatemusical.h"
#include <cstdlib>
#include <cmath>
#include <iostream>

CombateMusical::CombateMusical() {
    tiempoSpawn = 0.5f;
    
    // Configuración competitiva de HP y tiempos
    vidaEquipoJugador = 15000;
    vidaEquipoOponente = 15000;
    tiempoRonda = 30.f;              
    tiempoGlobalJugador = 300.f;     
    
    puntuacionTotal = 0;
    puntosParaSiguienteAtaque = 0;
    comboActual = 0; 
    
    turnoActivo = true;
    esTurnoJugador = true; 
    enPausaTransicion = false;
    tiempoEsperaTurno = 0.f;

    for (int i = 0; i < 4; i++) {
        carrilPresionado[i] = false;
        teclaBloqueada[i] = false;
    }

    if (!fuenteHUD.loadFromFile("assets/fonts/COMIC.TTF")) {
        fuenteHUD.loadFromFile("../assets/fonts/COMIC.TTF");
    }

    // --- DISEÑO GENERAL DEL HUD PERIFÉRICO ---
    textoVidaJugador.setFont(fuenteHUD);
    textoVidaJugador.setCharacterSize(26);
    textoVidaJugador.setFillColor(sf::Color::Blue);
    textoVidaJugador.setPosition(40.f, 30.f);

    textoPuntaje.setFont(fuenteHUD);
    textoPuntaje.setCharacterSize(22);
    textoPuntaje.setFillColor(sf::Color::White);
    textoPuntaje.setPosition(40.f, 70.f);

    textoRelojGlobal.setFont(fuenteHUD);
    textoRelojGlobal.setCharacterSize(22);
    textoRelojGlobal.setFillColor(sf::Color(135, 206, 235)); 
    textoRelojGlobal.setPosition(40.f, 100.f);

    textoVidaOponente.setFont(fuenteHUD);
    textoVidaOponente.setCharacterSize(26);
    textoVidaOponente.setFillColor(sf::Color::Red);
    textoVidaOponente.setPosition(960.f, 30.f);

    textoRelojRonda.setFont(fuenteHUD);
    textoRelojRonda.setCharacterSize(44);
    textoRelojRonda.setFillColor(sf::Color::Yellow);

    // Indicador de estado (ATACAS TU / RIVAL ATACANDO)
    textoIndicadorTurno.setFont(fuenteHUD);
    textoIndicadorTurno.setCharacterSize(24);

    // --- NUEVO FORMATO DE TEXTOS EN LA ZONA CENTRAL SUPERIOR ---
    textoJuicio.setFont(fuenteHUD);
    textoJuicio.setCharacterSize(24);

    // Contador de combo justo debajo del juicio con color naranja manual (RGB)
    textoCombo.setFont(fuenteHUD);
    textoCombo.setCharacterSize(22);
    textoCombo.setFillColor(sf::Color(255, 165, 0));

    // Letrero grande para anunciar los cambios de bando
    textoFinRound.setFont(fuenteHUD);
    textoFinRound.setCharacterSize(48);
    textoFinRound.setFillColor(sf::Color::Magenta);

    // Generar las líneas divisorias de la pista
    for (int i = 0; i <= 4; i++) {
        sf::RectangleShape linea;
        linea.setSize(sf::Vector2f(2.f, 720.f));
        linea.setFillColor(sf::Color(80, 80, 80)); 
        float posX = 480.f + (i * 80.f);
        linea.setPosition(posX, 0.f);
        lineasCarriles.push_back(linea);
    }

    // Generar los círculos base de impacto abajo
    for (int i = 0; i < 4; i++) {
        sf::CircleShape circuloBase;
        circuloBase.setRadius(38.f);
        circuloBase.setFillColor(sf::Color(45, 45, 45, 180));
        circuloBase.setOutlineThickness(2.f);
        circuloBase.setOutlineColor(sf::Color(180, 180, 180));
        float posX = 480.f + (i * 80.f) + 2.f;
        circuloBase.setPosition(posX, 600.f);
        circulosImpacto.push_back(circuloBase);
    }
}

void CombateMusical::reiniciarRelojes() {
    relojDelta.restart();
    relojSpawn.restart();
    vidaEquipoJugador = 15000;
    vidaEquipoOponente = 15000;
    tiempoRonda = 30.f;
    tiempoGlobalJugador = 300.f; 
    puntuacionTotal = 0;
    puntosParaSiguienteAtaque = 0;
    comboActual = 0;
    turnoActivo = true;
    esTurnoJugador = true;
    enPausaTransicion = false;
    textoJuicio.setString("");
    textoCombo.setString("");
    textoFinRound.setString("");
}

void CombateMusical::limpiarNotas() {
    notas.clear();
}

void CombateMusical::procesarEntrada() {
    if (!turnoActivo || enPausaTransicion || !esTurnoJugador) {
        for(int i = 0; i < 4; i++) {
            circulosImpacto[i].setFillColor(sf::Color(45, 45, 45, 180));
            circulosImpacto[i].setOutlineColor(sf::Color(180, 180, 180));
        }
        return; 
    }

    bool teclaActual[4];
    teclaActual[0] = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    teclaActual[1] = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
    teclaActual[2] = sf::Keyboard::isKeyPressed(sf::Keyboard::J);
    teclaActual[3] = sf::Keyboard::isKeyPressed(sf::Keyboard::K);

    for (int i = 0; i < 4; i++) {
        if (teclaActual[i]) {
            circulosImpacto[i].setFillColor(sf::Color(120, 120, 120, 220));
            circulosImpacto[i].setOutlineColor(sf::Color::White);
        } else {
            circulosImpacto[i].setFillColor(sf::Color(45, 45, 45, 180));
            circulosImpacto[i].setOutlineColor(sf::Color(180, 180, 180));
        }

        if (teclaActual[i] && !teclaBloqueada[i]) {
            carrilPresionado[i] = true;
            teclaBloqueada[i] = true;
        } else {
            carrilPresionado[i] = false;
        }

        if (!teclaActual[i]) {
            teclaBloqueada[i] = false;
        }
    }
}

void CombateMusical::actualizar() {
    float dt = relojDelta.restart().asSeconds();

    // --- CONTROL SOBERANO DE FIN DE PARTIDA ---
    if (vidaEquipoJugador <= 0 || tiempoGlobalJugador <= 0.f) {
        vidaEquipoJugador = 0;
        turnoActivo = false;
        textoFinRound.setString("GAME OVER");
        sf::FloatRect bounds = textoFinRound.getLocalBounds();
        textoFinRound.setPosition(640.f - (bounds.width / 2.f), 260.f);
        return;
    }
    if (vidaEquipoOponente <= 0) {
        vidaEquipoOponente = 0;
        turnoActivo = false;
        textoFinRound.setString("VICTORIA");
        sf::FloatRect bounds = textoFinRound.getLocalBounds();
        textoFinRound.setPosition(640.f - (bounds.width / 2.f), 260.f);
        return;
    }

    // --- PAUSA DE TRANSICIÓN ENTRE TURNOS ---
    if (enPausaTransicion) {
        tiempoEsperaTurno -= dt;
        if (tiempoEsperaTurno <= 0.f) {
            enPausaTransicion = false;
            turnoActivo = true;
            tiempoRonda = 30.f; 
            limpiarNotas();     
            textoFinRound.setString("");
        }
        return; 
    }

    // --- CONTROL DE TIEMPOS ACTIVOS ---
    tiempoRonda -= dt;
    if (esTurnoJugador) {
        tiempoGlobalJugador -= dt;
    }

    if (tiempoRonda <= 0.f) {
        tiempoRonda = 0.f;
        turnoActivo = false;
        enPausaTransicion = true;
        tiempoEsperaTurno = 2.0f; 
        comboActual = 0;          
        
        esTurnoJugador = !esTurnoJugador; 
        
        if (esTurnoJugador) {
            textoFinRound.setString("TU TURNO");
            textoFinRound.setFillColor(sf::Color::Cyan);
        } else {
            textoFinRound.setString("TURNO DEL RIVAL");
            textoFinRound.setFillColor(sf::Color::Red);
        }
        
        sf::FloatRect bounds = textoFinRound.getLocalBounds();
        textoFinRound.setPosition(640.f - (bounds.width / 2.f), 260.f);
        return;
    }

    // Generador automático de notas musicales
    if (relojSpawn.getElapsedTime().asSeconds() >= tiempoSpawn) {
        int carrilAleatorio = rand() % 4; 
        notas.push_back(Nota(carrilAleatorio, 320.f));
        relojSpawn.restart();
    }

    // Configuración del letrero dinámico superior de bando
    if (esTurnoJugador) {
        textoIndicadorTurno.setString("ATACAS TU");
        textoIndicadorTurno.setFillColor(sf::Color::Cyan);
    } else {
        textoIndicadorTurno.setString("RIVAL ATACANDO");
        textoIndicadorTurno.setFillColor(sf::Color::Red);
    }
    sf::FloatRect boundsTurno = textoIndicadorTurno.getLocalBounds();
    textoIndicadorTurno.setPosition(640.f - (boundsTurno.width / 2.f), 80.f);

    // Cronómetro central de ronda
    textoRelojRonda.setString(std::to_string(static_cast<int>(tiempoRonda)) + "s");
    sf::FloatRect boundsReloj = textoRelojRonda.getLocalBounds();
    textoRelojRonda.setPosition(640.f - (boundsReloj.width / 2.f), 20.f);

    // Formatear reloj global de 5 minutos
    int minutosM = static_cast<int>(tiempoGlobalJugador) / 60;
    int segundosM = static_cast<int>(tiempoGlobalJugador) % 60;
    std::string stringTiempoGlobal = "TIME: " + std::to_string(minutosM) + ":" + (segundosM < 10 ? "0" : "") + std::to_string(segundosM);

    textoVidaJugador.setString("PLAYER TEAM HP: " + std::to_string(vidaEquipoJugador));
    textoPuntaje.setString("SCORE: " + std::to_string(puntuacionTotal));
    textoRelojGlobal.setString(stringTiempoGlobal);
    textoVidaOponente.setString("RIVAL TEAM HP: " + std::to_string(vidaEquipoOponente));

    // Mostrar dinámicamente el contador de combo centrado
    if (comboActual > 0 && esTurnoJugador) {
        textoCombo.setString("COMBO: " + std::to_string(comboActual));
    } else {
        textoCombo.setString("");
    }
    sf::FloatRect boundsCombo = textoCombo.getLocalBounds();
    textoCombo.setPosition(640.f - (boundsCombo.width / 2.f), 150.f);

    if (relojJuicio.getElapsedTime().asSeconds() > 0.4f) {
        textoJuicio.setString("");
    }

    // --- ¡CORREGIDO AQUÍ! BUCLE DE ACTUALIZACIÓN DE NOTAS COMPLETAMENTE LIMPIO ---
    for (size_t i = 0; i < notas.size(); ) {
        notas[i].cuerpo.move(0.f, notas[i].velocidad * dt);
        float posYNota = notas[i].cuerpo.getPosition().y;
        int carrilNota = notas[i].carril;

        // Validar impacto solo si es el turno del jugador
        if (esTurnoJugador && carrilPresionado[carrilNota]) {
            float distancia = std::abs(posYNota - 600.f);

            if (distancia <= 70.f) {
                int puntosGanados = 0;
                bool aciertoValido = false;

                if (distancia <= 15.f) {
                    puntosGanados = 300;
                    textoJuicio.setFillColor(sf::Color::Cyan);
                    textoJuicio.setString("300 PERFECT!");
                    aciertoValido = true;
                } else if (distancia <= 30.f) {
                    puntosGanados = 200;
                    textoJuicio.setFillColor(sf::Color::Green);
                    textoJuicio.setString("200 GREAT");
                    aciertoValido = true;
                } else if (distancia <= 50.f) {
                    puntosGanados = 100;
                    textoJuicio.setFillColor(sf::Color::Yellow);
                    textoJuicio.setString("100 GOOD");
                    aciertoValido = true;
                } else {
                    puntosGanados = 50;
                    textoJuicio.setFillColor(sf::Color(255, 128, 0));
                    textoJuicio.setString("50 BAD");
                    comboActual = 0; 
                }

                puntuacionTotal += puntosGanados;
                puntosParaSiguienteAtaque += puntosGanados;
                relojJuicio.restart();

                if (aciertoValido) {
                    comboActual++;
                    if (comboActual % 10 == 0) {
                        vidaEquipoJugador += 100;
                        if (vidaEquipoJugador > 15000) vidaEquipoJugador = 15000; 
                    }
                }

                sf::FloatRect boundsJuicio = textoJuicio.getLocalBounds();
                textoJuicio.setPosition(640.f - (boundsJuicio.width / 2.f), 120.f);

                while (puntosParaSiguienteAtaque >= 500) {
                    vidaEquipoOponente -= 100;
                    puntosParaSiguienteAtaque -= 500;
                }

                carrilPresionado[carrilNota] = false;
                notas.erase(notas.begin() + i);
                continue;
            }
        }

        if (posYNota > 720.f) {
            if (esTurnoJugador) {
                textoJuicio.setFillColor(sf::Color::Red);
                textoJuicio.setString("MISS");
                vidaEquipoJugador -= 50; 
                comboActual = 0; 
                relojJuicio.restart();
                
                sf::FloatRect boundsJuicio = textoJuicio.getLocalBounds();
                textoJuicio.setPosition(640.f - (boundsJuicio.width / 2.f), 120.f);
            } else {
                textoJuicio.setFillColor(sf::Color(255, 100, 100));
                textoJuicio.setString("RIVAL ATK"); 
                relojJuicio.restart();

                sf::FloatRect boundsJuicio = textoJuicio.getLocalBounds();
                textoJuicio.setPosition(640.f - (boundsJuicio.width / 2.f), 120.f);
            }
            notas.erase(notas.begin() + i);
        } else {
            i++;
        }
    }
}

void CombateMusical::dibujar(sf::RenderWindow& ventana) {
    for (auto& linea : lineasCarriles) {
        ventana.draw(linea);
    }
    for (auto& circulo : circulosImpacto) {
        ventana.draw(circulo);
    }
    for (auto& nota : notas) {
        ventana.draw(nota.cuerpo);
    }

    ventana.draw(textoVidaJugador);
    ventana.draw(textoPuntaje);
    ventana.draw(textoRelojGlobal);
    ventana.draw(textoVidaOponente);
    ventana.draw(textoRelojRonda);
    ventana.draw(textoIndicadorTurno);
    ventana.draw(textoJuicio);
    ventana.draw(textoCombo); 

    if (enPausaTransicion || !turnoActivo) {
        ventana.draw(textoFinRound);
    }
}