#include "combatemusical.h"
#include <cstdlib>
#include <cmath>
#include <iostream>

CombateMusical::CombateMusical() {
    tiempoSpawn = 0.5f;
    strNombreJ1 = "JUGADOR 1";
    strNombreJ2 = "JUGADOR 2";

    vidaJ1 = 20000;
    vidaJ2 = 20000;
    tiempoRonda = 30.f;              
    tiempoGlobalJ1 = 300.f;     
    
    scoreJ1 = 0;
    scoreJ2 = 0;
    puntosParaSiguienteAtaque = 0;
    
    comboJ1 = 0; 
    comboJ2 = 0;
    
    turnoActivo = true;
    esTurnoJ1 = true; 
    enPausaTransicion = false;
    tiempoEsperaTurno = 0.f;
    
    // Configuración de juego estándar por defecto
    faseMedallas = false; 
    medallasActuales = 1; 
    velocidadFijaPartida = 320.f;

    sinergiaTotalE1 = 12;
    sinergiaTotalE2 = 12;
    cargaEspecialE1 = 0.f;
    cargaEspecialE2 = 0.f;
    especialActivadoVisual = false;
    tiempoVisualEspecial = 0.f;

    for (int i = 0; i < 4; i++) {
        carrilPresionado[i] = false;
        teclaBloqueada[i] = false;
    }

    if (!fuenteHUD.loadFromFile("assets/fonts/COMIC.TTF")) {
        fuenteHUD.loadFromFile("../assets/fonts/COMIC.TTF");
    }

    // HUD General del juego rítmico
    textoVidaJ1.setFont(fuenteHUD);
    textoVidaJ1.setCharacterSize(26);
    textoVidaJ1.setFillColor(sf::Color::Blue);
    textoVidaJ1.setPosition(40.f, 30.f);

    textoScoreJ1.setFont(fuenteHUD);
    textoScoreJ1.setCharacterSize(22);
    textoScoreJ1.setFillColor(sf::Color::White);
    textoScoreJ1.setPosition(40.f, 70.f);

    textoRelojGlobal.setFont(fuenteHUD);
    textoRelojGlobal.setCharacterSize(22);
    textoRelojGlobal.setFillColor(sf::Color(135, 206, 235)); 
    textoRelojGlobal.setPosition(40.f, 100.f);

    textoVidaJ2.setFont(fuenteHUD);
    textoVidaJ2.setCharacterSize(26);
    textoVidaJ2.setFillColor(sf::Color::Red);
    textoVidaJ2.setPosition(910.f, 30.f); 

    textoScoreJ2.setFont(fuenteHUD);
    textoScoreJ2.setCharacterSize(22);
    textoScoreJ2.setFillColor(sf::Color::White);
    textoScoreJ2.setPosition(910.f, 70.f); 

    textoRelojRonda.setFont(fuenteHUD);
    textoRelojRonda.setCharacterSize(44);
    textoRelojRonda.setFillColor(sf::Color::Yellow);

    textoIndicadorTurno.setFont(fuenteHUD);
    textoIndicadorTurno.setCharacterSize(22);

    textoJuicio.setFont(fuenteHUD);
    textoJuicio.setCharacterSize(24);

    textoCombo.setFont(fuenteHUD);
    textoCombo.setCharacterSize(22);
    textoCombo.setFillColor(sf::Color(255, 165, 0)); 

    textoFinRound.setFont(fuenteHUD);
    textoFinRound.setCharacterSize(48);
    textoFinRound.setFillColor(sf::Color::Magenta);

    textoEquiposIntegrantes.setFont(fuenteHUD);
    textoEquiposIntegrantes.setCharacterSize(14);
    textoEquiposIntegrantes.setFillColor(sf::Color(180, 180, 180));

    textoMedidorEspecial.setFont(fuenteHUD);
    textoMedidorEspecial.setCharacterSize(18);
    textoMedidorEspecial.setFillColor(sf::Color::Green);
    textoMedidorEspecial.setPosition(40.f, 130.f);

    textoAlertaEspecial.setFont(fuenteHUD);
    textoAlertaEspecial.setCharacterSize(52);
    textoAlertaEspecial.setFillColor(sf::Color::Red);

    for (int i = 0; i <= 4; i++) {
        sf::RectangleShape linea;
        linea.setSize(sf::Vector2f(2.f, 720.f));
        linea.setFillColor(sf::Color(80, 80, 80)); 
        float posX = 480.f + (i * 80.f);
        linea.setPosition(posX, 0.f);
        lineasCarriles.push_back(linea);
    }

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

void CombateMusical::configurarNombres(const std::string& Pale1, const std::string& Pale2) {
    strNombreJ1 = Pale1;
    strNombreJ2 = Pale2;
}

int CombateMusical::calcularSinergiaEquipo(const std::vector<std::string>& equipo) {
    if(equipo.size() < 3) return 12;
    int sinergiaBase = 12; 
    bool tieneKyo = false, tieneIori = false, tieneChizuru = false;
    bool tieneTerry = false, tieneAndy = false, tieneJoe = false;
    bool tieneRyo = false, tieneRobert = false, tieneYuri = false;

    for(const auto& personaje : equipo) {
        if(personaje == "Kyo") tieneKyo = true;
        if(personaje == "Iori") tieneIori = true;
        if(personaje == "Chizuru") tieneChizuru = true;
        if(personaje == "Terry") tieneTerry = true;
        if(personaje == "Andy") tieneAndy = true;
        if(personaje == "Joe") tieneJoe = true;
        if(personaje == "Ryo") tieneRyo = true;
        if(personaje == "Robert") tieneRobert = true;
        if(personaje == "Yuri") tieneYuri = true;
    }

    if(tieneKyo && tieneIori && tieneChizuru) sinergiaBase += 35; 
    else if(tieneTerry && tieneAndy && tieneJoe) sinergiaBase += 25;
    else if(tieneRyo && tieneRobert && tieneYuri) sinergiaBase += 25;
    else if(tieneKyo && tieneIori) sinergiaBase += 18;

    int totalLetras = equipo[0].size() + equipo[1].size() + equipo[2].size();
    return sinergiaBase + (totalLetras % 10);
}

void CombateMusical::configurarEquipos(const std::vector<std::string>& e1, const std::vector<std::string>& e2) {
    std::vector<std::string> filtradoE1;
    for (const auto& personaje : e1) {
        if (std::find(filtradoE1.begin(), filtradoE1.end(), personaje) == filtradoE1.end()) filtradoE1.push_back(personaje);
    }
    if (filtradoE1.size() < 3) {
        if (std::find(filtradoE1.begin(), filtradoE1.end(), "Kyo") == filtradoE1.end()) filtradoE1.push_back("Kyo");
        if (filtradoE1.size() < 3 && std::find(filtradoE1.begin(), filtradoE1.end(), "Benimaru") == filtradoE1.end()) filtradoE1.push_back("Benimaru");
        if (filtradoE1.size() < 3 && std::find(filtradoE1.begin(), filtradoE1.end(), "Goro") == filtradoE1.end()) filtradoE1.push_back("Goro");
    }

    std::vector<std::string> filtradoE2;
    for (const auto& personaje : e2) {
        if (std::find(filtradoE2.begin(), filtradoE2.end(), personaje) == filtradoE2.end()) filtradoE2.push_back(personaje);
    }
    if (filtradoE2.size() < 3) {
        if (std::find(filtradoE2.begin(), filtradoE2.end(), "Iori") == filtradoE2.end()) filtradoE2.push_back("Iori");
        if (filtradoE2.size() < 3 && std::find(filtradoE2.begin(), filtradoE2.end(), "Mature") == filtradoE2.end()) filtradoE2.push_back("Mature");
        if (filtradoE2.size() < 3 && std::find(filtradoE2.begin(), filtradoE2.end(), "Vice") == filtradoE2.end()) filtradoE2.push_back("Vice");
    }

    integrantesE1 = filtradoE1;
    integrantesE2 = filtradoE2;
    sinergiaTotalE1 = calcularSinergiaEquipo(filtradoE1);
    sinergiaTotalE2 = calcularSinergiaEquipo(filtradoE2);
}

void CombateMusical::reiniciarRelojes(int medallas) {
    relojDelta.restart();
    relojSpawn.restart();
    vidaJ1 = 20000;
    vidaJ2 = 20000;
    tiempoRonda = 30.f;
    tiempoGlobalJ1 = 300.f; 
    scoreJ1 = 0;
    scoreJ2 = 0;
    puntosParaSiguienteAtaque = 0;
    comboJ1 = 0; 
    comboJ2 = 0;
    turnoActivo = true;
    esTurnoJ1 = true;
    enPausaTransicion = false;
    cargaEspecialE1 = 0.f;
    cargaEspecialE2 = 0.f;
    especialActivadoVisual = false;
    
    // Guardamos las medallas elegidas externamente y calculamos su velocidad exacta
    medallasActuales = medallas;
    int vEquivalente = 1 + ((medallasActuales - 1) * 2);
    velocidadFijaPartida = 320.f + ((vEquivalente - 1) * 40.f);
    
    textoJuicio.setString("");
    textoCombo.setString("");
    textoFinRound.setString("");
}

void CombateMusical::limpiarNotas() {
    notas.clear();
}

void CombateMusical::procesarEntrada() {
    if (!turnoActivo || enPausaTransicion) {
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
    if (vidaJ1 <= 0 || tiempoGlobalJ1 <= 0.f || vidaJ2 <= 0) {
        turnoActivo = false;
        if (vidaJ1 <= 0 || tiempoGlobalJ1 <= 0.f) {
            vidaJ1 = 0;
            textoFinRound.setString(strNombreJ2 + " GANA");
        } else {
            vidaJ2 = 0;
            textoFinRound.setString(strNombreJ1 + " GANA");
        }
        sf::FloatRect bounds = textoFinRound.getLocalBounds();
        textoFinRound.setPosition(640.f - (bounds.width / 2.f), 260.f);
        return;
    }

    float dt = relojDelta.restart().asSeconds();

    if (especialActivadoVisual) {
        tiempoVisualEspecial -= dt;
        if (tiempoVisualEspecial <= 0.f) especialActivadoVisual = false;
    }

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

    tiempoRonda -= dt;
    if (esTurnoJ1) tiempoGlobalJ1 -= dt;

    if (tiempoRonda <= 0.f) {
        tiempoRonda = 0.f;
        turnoActivo = false;
        enPausaTransicion = true;
        tiempoEsperaTurno = 2.0f; 
        
        if (esTurnoJ1) {
            esTurnoJ1 = false;
            textoFinRound.setString("TURNO " + strNombreJ2);
            textoFinRound.setFillColor(sf::Color(218, 112, 214)); 
        } else {
            esTurnoJ1 = true;
            textoFinRound.setString("TURNO " + strNombreJ1);
            textoFinRound.setFillColor(sf::Color::Cyan);
        }
        
        sf::FloatRect bounds = textoFinRound.getLocalBounds();
        textoFinRound.setPosition(640.f - (bounds.width / 2.f), 260.f);
        return;
    }

    if (relojSpawn.getElapsedTime().asSeconds() >= tiempoSpawn) {
        int carrilAleatorio = rand() % 4; 
        notas.push_back(Nota(carrilAleatorio, velocidadFijaPartida));
        relojSpawn.restart();
    }

    int vVisual = 1 + ((medallasActuales - 1) * 2);
    std::string strMedallas = (medallasActuales == 1) ? " MEDALLA" : " MEDALLAS";
    
    if (esTurnoJ1) {
        textoIndicadorTurno.setString("ATACAS TU | " + std::to_string(medallasActuales) + strMedallas + " (V." + std::to_string(vVisual) + ")");
        textoIndicadorTurno.setFillColor(sf::Color::Cyan);
    } else {
        textoIndicadorTurno.setString(strNombreJ2 + " ATACANDO | " + std::to_string(medallasActuales) + strMedallas + " (V." + std::to_string(vVisual) + ")");
        textoIndicadorTurno.setFillColor(sf::Color(218, 112, 214)); 
    }
    sf::FloatRect boundsTurno = textoIndicadorTurno.getLocalBounds();
    textoIndicadorTurno.setPosition(640.f - (boundsTurno.width / 2.f), 80.f);

    textoRelojRonda.setString(std::to_string(static_cast<int>(tiempoRonda)) + "s");
    sf::FloatRect boundsReloj = textoRelojRonda.getLocalBounds();
    textoRelojRonda.setPosition(640.f - (boundsReloj.width / 2.f), 20.f);

    int minutesM = static_cast<int>(tiempoGlobalJ1) / 60;
    int segundosM = static_cast<int>(tiempoGlobalJ1) % 60;
    std::string stringTiempoGlobal = "TIME: " + std::to_string(minutesM) + ":" + (segundosM < 10 ? "0" : "") + std::to_string(segundosM);

    textoVidaJ1.setString(strNombreJ1 + " HP: " + std::to_string(vidaJ1));
    textoScoreJ1.setString("SCORE: " + std::to_string(scoreJ1));
    textoScoreJ2.setString("SCORE: " + std::to_string(scoreJ2));
    textoRelojGlobal.setString(stringTiempoGlobal);
    textoVidaJ2.setString(strNombreJ2 + " HP: " + std::to_string(vidaJ2));
    textoMedidorEspecial.setString("SP1: " + std::to_string(static_cast<int>(cargaEspecialE1)) + "% | SP2: " + std::to_string(static_cast<int>(cargaEspecialE2)) + "%");

    int comboAMostrar = esTurnoJ1 ? comboJ1 : comboJ2;
    if (comboAMostrar > 0) textoCombo.setString("COMBO: " + std::to_string(comboAMostrar));
    else textoCombo.setString("");
    sf::FloatRect boundsCombo = textoCombo.getLocalBounds();
    textoCombo.setPosition(640.f - (boundsCombo.width / 2.f), 150.f);

    if (relojJuicio.getElapsedTime().asSeconds() > 0.4f) textoJuicio.setString("");

    for (size_t i = 0; i < 4; ) { 
        if (i >= notas.size()) break;

        notas[i].cuerpo.move(0.f, notas[i].velocidad * dt);
        float posYNota = notas[i].cuerpo.getPosition().y;
        int carrilNota = notas[i].carril;

        if (esTurnoJ1) notas[i].cuerpo.setFillColor(sf::Color(0, 0, 139)); 
        else notas[i].cuerpo.setFillColor(sf::Color(148, 0, 211)); 

        if (carrilPresionado[carrilNota]) {
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
                    aciertoValido = true; 
                }

                if (esTurnoJ1) scoreJ1 += puntosGanados;
                else scoreJ2 += puntosGanados;
                
                puntosParaSiguienteAtaque += puntosGanados;
                relojJuicio.restart();

                if (aciertoValido) {
                    if (esTurnoJ1) {
                        comboJ1++;
                        float factorSinergia = sinergiaTotalE1 / 20.f;
                        cargaEspecialE1 += (1.5f * factorSinergia);
                        if(cargaEspecialE1 > 100.f) cargaEspecialE1 = 100.f;

                        if (comboJ1 >= 80 && cargaEspecialE1 >= 100.f) {
                            vidaJ2 -= 1000; 
                            cargaEspecialE1 = 0.f; 
                            especialActivadoVisual = true;
                            tiempoVisualEspecial = 1.5f;
                            textoAlertaEspecial.setString("TEAM SPECIAL ATTACK! -1000 HP");
                            sf::FloatRect bSp = textoAlertaEspecial.getLocalBounds();
                            textoAlertaEspecial.setPosition(640.f - (bSp.width / 2.f), 350.f);
                        }
                        if (comboJ1 % 10 == 0) {
                            vidaJ1 += 100;
                            if (vidaJ1 > 20000) vidaJ1 = 20000; 
                        }
                    } 
                    else { 
                        comboJ2++;
                        float factorSinergia = sinergiaTotalE2 / 20.f;
                        cargaEspecialE2 += (1.5f * factorSinergia);
                        if(cargaEspecialE2 > 100.f) cargaEspecialE2 = 100.f;

                        if (comboJ2 >= 80 && cargaEspecialE2 >= 100.f) {
                            vidaJ1 -= 1000; 
                            cargaEspecialE2 = 0.f; 
                            especialActivadoVisual = true;
                            tiempoVisualEspecial = 1.5f;
                            textoAlertaEspecial.setString("RIVAL SPECIAL ATTACK! -1000 HP");
                            sf::FloatRect bSp = textoAlertaEspecial.getLocalBounds();
                            textoAlertaEspecial.setPosition(640.f - (bSp.width / 2.f), 350.f);
                        }
                        if (comboJ2 % 10 == 0) {
                            vidaJ2 += 100;
                            if (vidaJ2 > 20000) vidaJ2 = 20000; 
                        }
                    }
                }

                sf::FloatRect boundsJuicio = textoJuicio.getLocalBounds();
                textoJuicio.setPosition(640.f - (boundsJuicio.width / 2.f), 120.f);

                while (puntosParaSiguienteAtaque >= 500) {
                    if (esTurnoJ1) vidaJ2 -= 100;
                    else vidaJ1 -= 100;
                    puntosParaSiguienteAtaque -= 500;
                }

                carrilPresionado[carrilNota] = false;
                notas.erase(notas.begin() + i);
                continue;
            }
        }

        if (posYNota > 720.f) {
            textoJuicio.setFillColor(sf::Color::Red);
            textoJuicio.setString("MISS");
            relojJuicio.restart();

            if (esTurnoJ1) {
                vidaJ1 -= 50; 
                comboJ1 = 0; 
            } else {
                vidaJ2 -= 50; 
                comboJ2 = 0; 
            }

            sf::FloatRect boundsJuicio = textoJuicio.getLocalBounds();
            textoJuicio.setPosition(640.f - (boundsJuicio.width / 2.f), 120.f);
            notas.erase(notas.begin() + i);
        } else {
            i++;
        }
    }
}

void CombateMusical::dibujar(sf::RenderWindow& ventana) {
    for (auto& linea : lineasCarriles) ventana.draw(linea);
    for (auto& circulo : circulosImpacto) ventana.draw(circulo);
    for (auto& nota : notas) ventana.draw(nota.cuerpo);

    ventana.draw(textoVidaJ1);
    ventana.draw(textoScoreJ1);
    ventana.draw(textoScoreJ2);
    ventana.draw(textoRelojGlobal); 
    ventana.draw(textoVidaJ2);
    ventana.draw(textoRelojRonda);
    ventana.draw(textoIndicadorTurno);
    ventana.draw(textoJuicio);
    ventana.draw(textoCombo); 
    ventana.draw(textoMedidorEspecial);

    if (especialActivadoVisual) ventana.draw(textoAlertaEspecial);

    if(integrantesE1.size() >= 3) {
        textoEquiposIntegrantes.setString("["+integrantesE1[0]+", "+integrantesE1[1]+", "+integrantesE1[2]+"]");
        textoEquiposIntegrantes.setPosition(40.f, 58.f);
        ventana.draw(textoEquiposIntegrantes);
    }
    if(integrantesE2.size() >= 3) {
        textoEquiposIntegrantes.setString("["+integrantesE2[0]+", "+integrantesE2[1]+", "+integrantesE2[2]+"]");
        textoEquiposIntegrantes.setPosition(910.f, 58.f);
        ventana.draw(textoEquiposIntegrantes);
    }

    if (enPausaTransicion || !turnoActivo) ventana.draw(textoFinRound);
    }