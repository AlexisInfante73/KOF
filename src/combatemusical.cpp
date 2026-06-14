#include "combatemusical.h"
#include <iostream>
#include <cmath>
#include <random> 
#include <cstdio>

CombateMusical::CombateMusical() {
    std::random_device rd; 
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<> distEscenario(1, 4); 
    
    int escenarioAleatorio = distEscenario(gen); 
    std::string nombreFondo = "fondo_batalla_" + std::to_string(escenarioAleatorio) + ".png"; 
    std::string rutaFondo = "assets/images/" + nombreFondo; 

    if (!texturaEscenario.loadFromFile(rutaFondo) && !texturaEscenario.loadFromFile("../" + rutaFondo)) {
        std::cerr << "ERROR: No se pudo cargar la textura del escenario: " << rutaFondo << "\n"; 
    }
    
    if (texturaEscenario.getSize().x > 0 && texturaEscenario.getSize().y > 0) { 
        spriteEscenario.setTexture(texturaEscenario); 
        spriteEscenario.setScale(1280.f / texturaEscenario.getSize().x, 720.f / texturaEscenario.getSize().y); 
    }

    if (!fuenteUI.loadFromFile("assets/fonts/COMIC.TTF")) { 
        if (!fuenteUI.loadFromFile("../assets/fonts/COMIC.TTF")) { 
            std::cerr << "ERROR: No se pudo cargar la fuente COMIC.TTF\n"; 
        }
    }

    // --- CRONÓMETRO CENTRAL ---
    txtCronometro.setFont(fuenteUI); 
    txtCronometro.setCharacterSize(45); 
    txtCronometro.setFillColor(sf::Color::White); 
    txtCronometro.setOutlineColor(sf::Color::Black); 
    txtCronometro.setOutlineThickness(2.f); 
    tiempoRestante = 99; 

    txtAnuncioKO.setFont(fuenteUI); 
    txtAnuncioKO.setCharacterSize(80); 
    txtAnuncioKO.setFillColor(sf::Color::Red); 
    txtAnuncioKO.setOutlineColor(sf::Color::Black); 
    txtAnuncioKO.setOutlineThickness(4.f); 

    // --- TEXTOS DE RESERVA ---
    for (int i = 0; i < 3; i++) { 
        txtReservaJ1[i].setFont(fuenteUI); 
        txtReservaJ1[i].setCharacterSize(48); 
        txtReservaJ1[i].setFillColor(sf::Color(210, 210, 210)); 
        txtReservaJ1[i].setOutlineColor(sf::Color::Black); 
        txtReservaJ1[i].setOutlineThickness(3.f); 

        txtReservaJ2[i].setFont(fuenteUI); 
        txtReservaJ2[i].setCharacterSize(48); 
        txtReservaJ2[i].setFillColor(sf::Color(210, 210, 210)); 
        txtReservaJ2[i].setOutlineColor(sf::Color::Black); 
        txtReservaJ2[i].setOutlineThickness(3.f); 
    }

    // --- CONFIGURACIÓN DE NOMBRES PRINCIPALES ---
    txtNombreJ1.setFont(fuenteUI); 
    txtNombreJ1.setCharacterSize(32); 
    txtNombreJ1.setFillColor(sf::Color::White); 
    txtNombreJ1.setOutlineColor(sf::Color::Black); 
    txtNombreJ1.setOutlineThickness(2.f); 
    txtNombreJ1.setPosition(135.f, 2.f); 

    txtNombreJ2.setFont(fuenteUI); 
    txtNombreJ2.setCharacterSize(32); 
    txtNombreJ2.setFillColor(sf::Color::White); 
    txtNombreJ2.setOutlineColor(sf::Color::Black); 
    txtNombreJ2.setOutlineThickness(2.f); 

    // --- MARCOS DE AVATARS ---
    marcoAvatarJ1.setSize(sf::Vector2f(85.f, 85.f)); 
    marcoAvatarJ1.setFillColor(sf::Color::Transparent); 
    marcoAvatarJ1.setOutlineColor(sf::Color::White); 
    marcoAvatarJ1.setOutlineThickness(3.f); 
    marcoAvatarJ1.setPosition(25.f, 20.f); 
    spriteAvatarJ1.setPosition(25.f, 20.f); 

    marcoAvatarJ2.setSize(sf::Vector2f(85.f, 85.f)); 
    marcoAvatarJ2.setFillColor(sf::Color::Transparent); 
    marcoAvatarJ2.setOutlineColor(sf::Color::White); 
    marcoAvatarJ2.setOutlineThickness(3.f); 
    marcoAvatarJ2.setPosition(1170.f, 20.f); 
    spriteAvatarJ2.setPosition(1170.f, 20.f); 

    // --- BARRAS DE VIDA ---
    fondoBarraJ1.setSize(sf::Vector2f(420.f, 25.f)); 
    fondoBarraJ1.setFillColor(sf::Color(100, 0, 0)); 
    fondoBarraJ1.setPosition(135.f, 42.f); 

    fondoBarraJ2.setSize(sf::Vector2f(420.f, 25.f)); 
    fondoBarraJ2.setFillColor(sf::Color(100, 0, 0)); 
    fondoBarraJ2.setPosition(1280.f - 135.f - 420.f, 42.f); 

    barraVidaJ1.setFillColor(sf::Color::Green); 
    barraVidaJ1.setPosition(135.f, 42.f); 
    barraVidaJ2.setFillColor(sf::Color::Green); 

    // --- HUD INFERIOR (BARRAS DE ENERGÍA) ---
    txtNivelJ1.setFont(fuenteUI); 
    txtNivelJ1.setCharacterSize(38); 
    txtNivelJ1.setFillColor(sf::Color::Yellow); 
    txtNivelJ1.setOutlineColor(sf::Color::Black); 
    txtNivelJ1.setOutlineThickness(2.5f); 
    txtNivelJ1.setPosition(40.f, 630.f); 

    txtNivelJ2.setFont(fuenteUI); 
    txtNivelJ2.setCharacterSize(38); 
    txtNivelJ2.setFillColor(sf::Color::Yellow); 
    txtNivelJ2.setOutlineColor(sf::Color::Black); 
    txtNivelJ2.setOutlineThickness(2.5f); 
    txtNivelJ2.setPosition(1215.f, 630.f); 

    fondoEspecialJ1.setSize(sf::Vector2f(320.f, 18.f)); 
    fondoEspecialJ1.setFillColor(sf::Color(40, 40, 40)); 
    fondoEspecialJ1.setPosition(95.f, 645.f); 

    fondoEspecialJ2.setSize(sf::Vector2f(320.f, 18.f)); 
    fondoEspecialJ2.setFillColor(sf::Color(40, 40, 40)); 
    fondoEspecialJ2.setPosition(1280.f - 95.f - 320.f, 645.f); 

    barraEspecialJ1.setFillColor(sf::Color::Yellow); 
    barraEspecialJ1.setPosition(95.f, 645.f); 
    barraEspecialJ2.setFillColor(sf::Color::Yellow); 

    energiaJ1 = 0.f; energiaJ2 = 0.f; 
    nivelesJ1 = 0;   nivelesJ2 = 0; 

    // --- INICIALIZAR TEXTOS DE COMBO ---
    hitsJ1 = 0; 
    hitsJ2 = 0; 

    txtComboJ1.setFont(fuenteUI); 
    txtComboJ1.setCharacterSize(55); 
    txtComboJ1.setFillColor(sf::Color(255, 165, 0)); 
    txtComboJ1.setOutlineColor(sf::Color::Black); 
    txtComboJ1.setOutlineThickness(3.f); 
    txtComboJ1.setPosition(150.f, 150.f); 

    txtComboJ2.setFont(fuenteUI); 
    txtComboJ2.setCharacterSize(55); 
    txtComboJ2.setFillColor(sf::Color(255, 165, 0)); 
    txtComboJ2.setOutlineColor(sf::Color::Black); 
    txtComboJ2.setOutlineThickness(3.f); 
    txtComboJ2.setPosition(1280.f - 350.f, 150.f); 

    golpeImpactadoEsteTurno = false; 
    botGolpeImpactadoEsteTurno = false; 
    rondaTerminada = false; 
    floatProximaDecision = 0.3f; 
    botQuiereAgacharse = false; 
    botQuiereDefenderse = false; 
    
    indiceActivoJ1 = 0; 
    indiceActivoJ2 = 0; 

    cargarRoster(); 
    estadoActual = EstadoJuego::SeleccionPersonajes; 
    inicializarPantallaSeleccion(); 
}

void CombateMusical::cargarRoster() {
    roster[0] = {"KAI", "assets/portraits/kai.png", sf::Color::Cyan}; 
    roster[1] = {"LUCIEN", "assets/portraits/lucien.png", sf::Color(255, 140, 0)}; 

    for (int i = 2; i < 12; i++) { 
        roster[i] = {"BLOQUEADO", "", sf::Color(40, 40, 40)}; 
    }
}

void CombateMusical::inicializarPantallaSeleccion() {
    if (!texturaSeleccion.loadFromFile("assets/images/fondo_seleccion.png")) { 
        if (!texturaSeleccion.loadFromFile("../assets/images/fondo_seleccion.png")) { 
            std::cerr << "ERROR: No se pudo cargar el fondo de seleccion.\n"; 
        }
    }
    
    if (texturaSeleccion.getSize().x > 0 && texturaSeleccion.getSize().y > 0) { 
        spriteSeleccion.setTexture(texturaSeleccion); 
        spriteSeleccion.setScale(1280.f / texturaSeleccion.getSize().x, 720.f / texturaSeleccion.getSize().y); 
    }

    selectorCuadrula.setSize(sf::Vector2f(124.f, 168.f)); 
    selectorCuadrula.setFillColor(sf::Color(0, 255, 255, 70)); 
    selectorCuadrula.setOutlineColor(sf::Color::Cyan); 
    selectorCuadrula.setOutlineThickness(4.f); 

    fondoOcultarTiempo.setSize(sf::Vector2f(120.f, 40.f)); 
    fondoOcultarTiempo.setFillColor(sf::Color(10, 15, 35)); 
    fondoOcultarTiempo.setPosition(580.f, 505.f); 

    txtTiempoSeleccion.setFont(fuenteUI); 
    txtTiempoSeleccion.setCharacterSize(42); 
    txtTiempoSeleccion.setFillColor(sf::Color::White); 
    txtTiempoSeleccion.setPosition(585.f, 501.f); 

    for(int i=0; i<3; i++) { 
        vistasPreviasJ1[i].setSize(sf::Vector2f(58.f, 58.f)); 
        vistasPreviasJ1[i].setOutlineColor(sf::Color::White); 
        vistasPreviasJ1[i].setOutlineThickness(2.f); 
        vistasPreviasJ1[i].setFillColor(sf::Color::Transparent); 

        vistasPreviasJ2[i].setSize(sf::Vector2f(58.f, 58.f)); 
        vistasPreviasJ2[i].setOutlineColor(sf::Color::White); 
        vistasPreviasJ2[i].setOutlineThickness(2.f); 
        vistasPreviasJ2[i].setFillColor(sf::Color::Transparent); 
    }

    filaSeleccionada = 0; 
    colSeleccionada = 0; 
    tiempoSeleccionRestante = 15.f; 
    turnoJugador1 = true; 

    equipoJ1.clear(); 
    equipoJ2.clear(); 
    relojSeleccion.restart(); 
    relojDeltaTime.restart(); 
}

void CombateMusical::seleccionarPersonajeActual() {
    int indiceActual = filaSeleccionada * 6 + colSeleccionada; 

    if (turnoJugador1 && equipoJ1.size() < 3) { 
        bool yaElegido = false; 
        for (const auto& p : equipoJ1) { 
            if (p.getNombre() == roster[indiceActual].nombre) { 
                yaElegido = true; 
                break; 
            }
        }

        if (!yaElegido) { 
            Personaje p; 
            p.inicializar(roster[indiceActual].nombre, roster[indiceActual].rutaAvatar, roster[indiceActual].colorRelleno, 300.f); 
            equipoJ1.push_back(p); 
            
            vistasPreviasJ1[equipoJ1.size() - 1].setFillColor(roster[indiceActual].colorRelleno); 

            if (equipoJ1.size() == 3) { 
                turnoJugador1 = false; 
                tiempoSeleccionRestante = 15.f; 
                relojSeleccion.restart(); 
            }
        }
    }
}

void CombateMusical::procesarEntrada(sf::Event& evento) {
    if (estadoActual == EstadoJuego::SeleccionPersonajes) { 
        if (evento.type == sf::Event::KeyPressed && turnoJugador1) { 
            if (evento.key.code == sf::Keyboard::Left || evento.key.code == sf::Keyboard::A) { 
                if (colSeleccionada > 0) colSeleccionada--; 
            }
            if (evento.key.code == sf::Keyboard::Right || evento.key.code == sf::Keyboard::D) { 
                if (colSeleccionada < 5) colSeleccionada++; 
            }
            if (evento.key.code == sf::Keyboard::Up || evento.key.code == sf::Keyboard::W) { 
                if (filaSeleccionada > 0) filaSeleccionada--; 
            }
            if (evento.key.code == sf::Keyboard::Down || evento.key.code == sf::Keyboard::S) { 
                if (filaSeleccionada < 1) filaSeleccionada++; 
            }

            if (evento.key.code == sf::Keyboard::Enter || evento.key.code == sf::Keyboard::Space) { 
                seleccionarPersonajeActual(); 
            }
        }
        return; 
    }

    if (rondaTerminada) return; 
    
    if (evento.type == sf::Event::KeyPressed) { 
        if (evento.key.code == sf::Keyboard::Space) equipoJ1[indiceActivoJ1].saltar(); 
        
        if (evento.key.code == sf::Keyboard::H) { equipoJ1[indiceActivoJ1].lanzarAtaque(1); golpeImpactadoEsteTurno = false; } 
        if (evento.key.code == sf::Keyboard::J) { equipoJ1[indiceActivoJ1].lanzarAtaque(2); golpeImpactadoEsteTurno = false; } 
        if (evento.key.code == sf::Keyboard::K) { equipoJ1[indiceActivoJ1].lanzarAtaque(3); golpeImpactadoEsteTurno = false; } 
        if (evento.key.code == sf::Keyboard::L) { equipoJ1[indiceActivoJ1].lanzarAtaque(4); golpeImpactadoEsteTurno = false; } 
        
        if (evento.key.code == sf::Keyboard::E) { 
            if (nivelesJ1 >= 1 && !equipoJ1[indiceActivoJ1].getEstaAtacando()) { 
                nivelesJ1--; 
                equipoJ1[indiceActivoJ1].lanzarAtaque(5); 
                golpeImpactadoEsteTurno = false; 
            }
        }
    }
}

void CombateMusical::actualizar() {
    float deltaTime = relojDeltaTime.restart().asSeconds(); 

    if (estadoActual == EstadoJuego::SeleccionPersonajes) { 
        float xInicial = 415.f; 
        float yInicial = 124.f; 
        float espacioX = 142.f; 
        float espacioY = 195.f; 
        selectorCuadrula.setPosition(xInicial + colSeleccionada * espacioX, yInicial + filaSeleccionada * espacioY); 

        if (turnoJugador1) { 
            if (tiempoSeleccionRestante > 0.f) { 
                tiempoSeleccionRestante -= deltaTime; 
                if (tiempoSeleccionRestante < 0.f) tiempoSeleccionRestante = 0.f; 
            } else { 
                std::random_device rd; std::mt19937 gen(rd()); std::uniform_int_distribution<> distAuto(0, 11); 
                while(equipoJ1.size() < 3) { 
                    int id = distAuto(gen); 
                    bool existe = false; 
                    for (const auto& pj : equipoJ1) if (pj.getNombre() == roster[id].nombre) existe = true; 
                    if (!existe) { 
                        Personaje p; 
                        p.inicializar(roster[id].nombre, roster[id].rutaAvatar, roster[id].colorRelleno, 300.f); 
                        equipoJ1.push_back(p); 
                        vistasPreviasJ1[equipoJ1.size() - 1].setFillColor(roster[id].colorRelleno); 
                    }
                }
                turnoJugador1 = false; 
                tiempoSeleccionRestante = 15.f; 
                relojSeleccion.restart(); 
            }
        }

        char buffer[16]; 
        snprintf(buffer, sizeof(buffer), "%05.2f", tiempoSeleccionRestante); 
        txtTiempoSeleccion.setString(buffer); 

        if (!turnoJugador1 && relojSeleccion.getElapsedTime().asSeconds() > 0.8f) { 
            if (equipoJ2.size() < 3) { 
                std::random_device rd; std::mt19937 gen(rd()); std::uniform_int_distribution<> distAuto(0, 11); 
                int idAzar = distAuto(gen); 
                bool existe = false; 
                for (const auto& pj : equipoJ2) if (pj.getNombre() == roster[idAzar].nombre) existe = true; 
                if (!existe) { 
                    Personaje bot; 
                    bot.inicializar(roster[idAzar].nombre, roster[idAzar].rutaAvatar, roster[idAzar].colorRelleno, 980.f); 
                    equipoJ2.push_back(bot); 
                    vistasPreviasJ2[equipoJ2.size() - 1].setFillColor(roster[idAzar].colorRelleno); 
                    relojSeleccion.restart(); 
                }
            } else { 
                reiniciarRelojes(0); 
                estadoActual = EstadoJuego::Combate; 
            }
        }

        for(size_t i = 0; i < equipoJ1.size(); i++) vistasPreviasJ1[i].setPosition(94.f + (i * 88.f), 612.f); 
        for(size_t i = 0; i < equipoJ2.size(); i++) vistasPreviasJ2[i].setPosition(952.f + (i * 88.f), 612.f); 

        return; 
    }

    if (rondaTerminada) { 
        if (relojEsperaRonda.getElapsedTime().asSeconds() >= 3.0f) { 
            if (indiceActivoJ1 >= 3 || indiceActivoJ2 >= 3) { reiniciarRelojes(0); return; } 
            equipoJ1[indiceActivoJ1].corregirPosicionX(300.f - equipoJ1[indiceActivoJ1].getPosicionX()); 
            equipoJ2[indiceActivoJ2].corregirPosicionX(980.f - equipoJ2[indiceActivoJ2].getPosicionX()); 
            tiempoRestante = 99; 
            rondaTerminada = false; 
            golpeImpactadoEsteTurno = false; 
            botGolpeImpactadoEsteTurno = false; 
            cargarAvatarsUI(); 
            relojPelea.restart(); 
        }
        return; 
    }

    if (indiceActivoJ1 >= 3 || indiceActivoJ2 >= 3) return; 

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) equipoJ1[indiceActivoJ1].caminar(-1.f); 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) equipoJ1[indiceActivoJ1].caminar(1.f); 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) equipoJ1[indiceActivoJ1].setAgachado(true); else equipoJ1[indiceActivoJ1].setAgachado(false); 

    actualizarIABot(); 
    equipoJ1[indiceActivoJ1].actualizar(); 
    equipoJ2[indiceActivoJ2].actualizar(); 

    float x1 = equipoJ1[indiceActivoJ1].getPosicionX(); 
    float x2 = equipoJ2[indiceActivoJ2].getPosicionX(); 
    if (std::abs(x1 - x2) < 80.f) { 
        float solapamiento = 80.f - std::abs(x1 - x2); 
        float dir = (x1 < x2) ? -1.f : 1.f; 
        equipoJ1[indiceActivoJ1].corregirPosicionX(dir * solapamiento / 2.f); 
        equipoJ2[indiceActivoJ2].corregirPosicionX(-dir * solapamiento / 2.f); 
    }

    bool j2EnGuardia = false; 
    if (botQuiereDefenderse) j2EnGuardia = true; 

    if (equipoJ1[indiceActivoJ1].getEstaAtacando() && !golpeImpactadoEsteTurno) { 
        if (std::abs(equipoJ1[indiceActivoJ1].getPosicionX() - equipoJ2[indiceActivoJ2].getPosicionX()) <= 125.f) { 
            int tipo = equipoJ1[indiceActivoJ1].getTipoAtaque(); 
            bool rivalEvadiendo = equipoJ2[indiceActivoJ2].getEstaAgachado(); 
            float dmg = 0.f; 

            if (tipo == 1) { dmg = 7.f;  acumularEnergiaJ1(5.f); } 
            if (tipo == 3) { dmg = 9.f;  acumularEnergiaJ1(6.f); } 
            if (tipo == 2 && !rivalEvadiendo) { dmg = 12.f; acumularEnergiaJ1(7.f); } 
            if (tipo == 4 && !rivalEvadiendo) { dmg = 15.f; acumularEnergiaJ1(9.f); } 
            if (tipo == 5) { dmg = 45.f; } 

            if (j2EnGuardia && tipo != 5) { 
                dmg = dmg * 0.2f; 
            }

            if (dmg > 0.f) { 
                equipoJ2[indiceActivoJ2].recibirDanio(dmg); 
                acumularEnergiaJ2(6.f); 
                
                hitsJ1++; 
                relojComboJ1.restart(); 
                txtComboJ1.setString(std::to_string(hitsJ1) + " HITS!"); 
                hitsJ2 = 0; 

                if (equipoJ2[indiceActivoJ2].getVida() <= 0.f) { barraVidaJ2.setSize(sf::Vector2f(0.f, 25.f)); avanzarSiguienteRonda(1); return; } 
            }
            golpeImpactadoEsteTurno = true; 
        }
    }

    bool j1EnGuardia = false; 
    if (x1 < x2 && sf::Keyboard::isKeyPressed(sf::Keyboard::A)) j1EnGuardia = true; 
    if (x1 > x2 && sf::Keyboard::isKeyPressed(sf::Keyboard::D)) j1EnGuardia = true; 

    if (equipoJ2[indiceActivoJ2].getEstaAtacando() && !botGolpeImpactadoEsteTurno) { 
        if (std::abs(equipoJ2[indiceActivoJ2].getPosicionX() - equipoJ1[indiceActivoJ1].getPosicionX()) <= 125.f) { 
            int tipo = equipoJ2[indiceActivoJ2].getTipoAtaque(); 
            bool jugador1Evadiendo = equipoJ1[indiceActivoJ1].getEstaAgachado(); 
            float dmg = 0.f; 

            if (tipo == 1) { dmg = 7.f;  acumularEnergiaJ2(5.f); } 
            if (tipo == 3) { dmg = 9.f;  acumularEnergiaJ2(6.f); } 
            if (tipo == 2 && !jugador1Evadiendo) { dmg = 12.f; acumularEnergiaJ2(7.f); } 
            if (tipo == 4 && !jugador1Evadiendo) { dmg = 15.f; acumularEnergiaJ2(9.f); } 
            if (tipo == 5) { dmg = 45.f; } 

            if (j1EnGuardia && tipo != 5) { 
                dmg = dmg * 0.2f; 
            }

            if (dmg > 0.f) { 
                equipoJ1[indiceActivoJ1].recibirDanio(dmg); 
                acumularEnergiaJ1(6.f); 
                
                hitsJ2++; 
                relojComboJ2.restart(); 
                txtComboJ2.setString(std::to_string(hitsJ2) + " HITS!"); 
                hitsJ1 = 0; 

                if (equipoJ1[indiceActivoJ1].getVida() <= 0.f) { barraVidaJ1.setSize(sf::Vector2f(0.f, 25.f)); avanzarSiguienteRonda(2); return; } 
            }
            botGolpeImpactadoEsteTurno = true; 
        }
    }

    if (hitsJ1 > 0 && relojComboJ1.getElapsedTime().asSeconds() > 1.5f) hitsJ1 = 0; 
    if (hitsJ2 > 0 && relojComboJ2.getElapsedTime().asSeconds() > 1.5f) hitsJ2 = 0; 

    barraVidaJ1.setSize(sf::Vector2f((equipoJ1[indiceActivoJ1].getVida() / 250.f) * 420.f > 0.f ? (equipoJ1[indiceActivoJ1].getVida() / 250.f) * 420.f : 0.f, 25.f)); 
    float anchoVerdeJ2 = (equipoJ2[indiceActivoJ2].getVida() / 250.f) * 420.f; 
    if (anchoVerdeJ2 < 0.f) anchoVerdeJ2 = 0.f; 
    barraVidaJ2.setSize(sf::Vector2f(anchoVerdeJ2, 25.f)); 
    barraVidaJ2.setPosition(1280.f - 135.f - anchoVerdeJ2, 42.f); 

    if (nivelesJ1 == 5) barraEspecialJ1.setSize(sf::Vector2f(320.f, 18.f)); 
    else barraEspecialJ1.setSize(sf::Vector2f((energiaJ1 / 100.f) * 320.f, 18.f)); 

    if (nivelesJ2 == 5) { 
        barraEspecialJ2.setSize(sf::Vector2f(320.f, 18.f)); 
        barraEspecialJ2.setPosition(1280.f - 95.f - 320.f, 645.f); 
    } else { 
        float anchoEspJ2 = (energiaJ2 / 100.f) * 320.f; 
        barraEspecialJ2.setSize(sf::Vector2f(anchoEspJ2, 18.f)); 
        barraEspecialJ2.setPosition(1280.f - 95.f - anchoEspJ2, 645.f); 
    }

    txtNivelJ1.setString(std::to_string(nivelesJ1)); 
    txtNivelJ2.setString(std::to_string(nivelesJ2)); 
    txtNombreJ1.setString(equipoJ1[indiceActivoJ1].getNombre()); 
    txtNombreJ2.setString(equipoJ2[indiceActivoJ2].getNombre()); 
    txtNombreJ2.setPosition(1280.f - 135.f - txtNombreJ2.getLocalBounds().width, 2.f); 
    
    float inicioY = 75.f; float espaciadoY = 52.f; 
    int filaJ1 = 0; 
    for (int i = 0; i < 3; i++) { 
        if (i < indiceActivoJ1) continue; 
        if (i != indiceActivoJ1) { 
            txtReservaJ1[filaJ1].setString(equipoJ1[i].getNombre()); 
            txtReservaJ1[filaJ1].setPosition(135.f, inicioY + (filaJ1 * espaciadoY)); 
            txtReservaJ1[filaJ1].setFillColor(sf::Color(190, 190, 190)); 
            filaJ1++; 
        }
    }
    for(int i = filaJ1; i < 3; i++) { txtReservaJ1[i].setString(""); } 

    int filaJ2 = 0; 
    for (int i = 0; i < 3; i++) { 
        if (i < indiceActivoJ2) continue; 
        if (i != indiceActivoJ2) { 
            txtReservaJ2[filaJ2].setString(equipoJ2[i].getNombre()); 
            float posX = 1280.f - 135.f - txtReservaJ2[filaJ2].getLocalBounds().width; 
            txtReservaJ2[filaJ2].setPosition(posX, inicioY + (filaJ2 * espaciadoY)); 
            txtReservaJ2[filaJ2].setFillColor(sf::Color(190, 190, 190)); 
            filaJ2++; 
        }
    }
    for(int i = filaJ2; i < 3; i++) { txtReservaJ2[i].setString(""); } 

    if (tiempoRestante > 0) { 
        if (relojPelea.getElapsedTime().asSeconds() >= 1.f) { tiempoRestante--; relojPelea.restart(); } 
    } else { 
        if (equipoJ1[indiceActivoJ1].getVida() >= equipoJ2[indiceActivoJ2].getVida()) avanzarSiguienteRonda(1); else avanzarSiguienteRonda(2); 
        return; 
    }
    txtCronometro.setString(std::to_string(tiempoRestante)); 
    txtCronometro.setPosition(1280.f / 2.f - txtCronometro.getLocalBounds().width / 2.f, 25.f); 
}

void CombateMusical::dibujar(sf::RenderWindow& window) {
    if (estadoActual == EstadoJuego::SeleccionPersonajes) { 
        if (texturaSeleccion.getSize().x > 0) window.draw(spriteSeleccion); 
        
        window.draw(fondoOcultarTiempo); 
        window.draw(txtTiempoSeleccion); 

        if (turnoJugador1) window.draw(selectorCuadrula); 

        for (size_t i = 0; i < equipoJ1.size(); i++) window.draw(vistasPreviasJ1[i]); 
        for (size_t i = 0; i < equipoJ2.size(); i++) window.draw(vistasPreviasJ2[i]); 

        return; 
    }

    if (texturaEscenario.getSize().x > 0) window.draw(spriteEscenario); 

    if (indiceActivoJ1 < 3) equipoJ1[indiceActivoJ1].dibujar(window); 
    if (indiceActivoJ2 < 3) equipoJ2[indiceActivoJ2].dibujar(window); 

    window.draw(fondoBarraJ1); window.draw(fondoBarraJ2); 
    window.draw(barraVidaJ1); window.draw(barraVidaJ2); 
    window.draw(txtCronometro); 
    
    for (int i = 0; i < 3; i++) { 
        if (txtReservaJ1[i].getString() != "") window.draw(txtReservaJ1[i]); 
        if (txtReservaJ2[i].getString() != "") window.draw(txtReservaJ2[i]); 
    }

    window.draw(txtNombreJ1); window.draw(txtNombreJ2); 
    window.draw(marcoAvatarJ1); window.draw(marcoAvatarJ2); 
    
    if (indiceActivoJ1 < 3 && texturaAvatarJ1.getSize().x > 0) window.draw(spriteAvatarJ1); 
    if (indiceActivoJ2 < 3 && texturaAvatarJ2.getSize().x > 0) window.draw(spriteAvatarJ2); 

    window.draw(fondoEspecialJ1); window.draw(fondoEspecialJ2); 
    window.draw(barraEspecialJ1); window.draw(barraEspecialJ2); 
    window.draw(txtNivelJ1); window.draw(txtNivelJ2); 

    if (hitsJ1 > 1) window.draw(txtComboJ1); 
    if (hitsJ2 > 1) window.draw(txtComboJ2); 

    if (rondaTerminada) window.draw(txtAnuncioKO); 
}

void CombateMusical::reiniciarRelojes(int medallas) {
    relojPelea.restart(); 
    relojDeltaTime.restart(); 
    tiempoRestante = 99; 
    golpeImpactadoEsteTurno = false; 
    botGolpeImpactadoEsteTurno = false; 
    rondaTerminada = false; 
    indiceActivoJ1 = 0; 
    indiceActivoJ2 = 0; 
    
    energiaJ1 = 0.f; energiaJ2 = 0.f; 
    nivelesJ1 = 0; nivelesJ2 = 0; 

    botQuiereAgacharse = false; 
    botQuiereDefenderse = false; 
    hitsJ1 = 0; 
    hitsJ2 = 0; 

    cargarAvatarsUI(); 
}

void CombateMusical::avanzarSiguienteRonda(int ganadorDeRonda) {
    rondaTerminada = true; 
    relojEsperaRonda.restart(); 
    hitsJ1 = 0; hitsJ2 = 0; 

    if (ganadorDeRonda == 1) { 
        txtAnuncioKO.setString("K.O. - ROUND GANADO"); 
        equipoJ1[indiceActivoJ1].curarVida(40.f); 
        indiceActivoJ2++; 
    } else { 
        txtAnuncioKO.setString("K.O. - BOT GANADOR"); 
        equipoJ2[indiceActivoJ2].curarVida(40.f); 
        indiceActivoJ1++; 
    }
    txtAnuncioKO.setPosition(1280.f / 2.f - txtAnuncioKO.getLocalBounds().width / 2.f, 720.f / 2.f - 100.f); 
}

void CombateMusical::actualizarIABot() {
    if (rondaTerminada || indiceActivoJ2 >= 3) return; 
    float x1 = equipoJ1[indiceActivoJ1].getPosicionX(); 
    float x2 = equipoJ2[indiceActivoJ2].getPosicionX(); 
    float distancia = std::abs(x1 - x2); 

    if (equipoJ2[indiceActivoJ2].getEstaAtacando()) return; 
    bool jugadorAtacando = equipoJ1[indiceActivoJ1].getEstaAtacando(); 

    if (distancia > 115.f) { 
        botQuiereAgacharse = false; botQuiereDefenderse = false; 
        equipoJ2[indiceActivoJ2].setAgachado(false); 
        if (x2 > x1) equipoJ2[indiceActivoJ2].caminar(-1.f); else equipoJ2[indiceActivoJ2].caminar(1.f); 
    } else { 
        if (relojDecisionBot.getElapsedTime().asSeconds() >= floatProximaDecision) { 
            relojDecisionBot.restart(); 
            std::random_device rd; std::mt19937 gen(rd()); std::uniform_int_distribution<> dist(0, 99); 
            int decision = dist(gen); 
            
            if (jugadorAtacando && decision < 55) { 
                botQuiereDefenderse = true; botQuiereAgacharse = false; 
            } else if (nivelesJ2 >= 1 && decision < 25) { 
                nivelesJ2--; equipoJ2[indiceActivoJ2].lanzarAtaque(5); 
                botGolpeImpactadoEsteTurno = false; botQuiereAgacharse = false; botQuiereDefenderse = false; 
            } else if (decision < 70) { 
                equipoJ2[indiceActivoJ2].lanzarAtaque((dist(gen) % 4) + 1); 
                botGolpeImpactadoEsteTurno = false; botQuiereAgacharse = false; botQuiereDefenderse = false; 
            } else { 
                botQuiereAgacharse = true; botQuiereDefenderse = false; 
            }
        }
        if (botQuiereAgacharse) equipoJ2[indiceActivoJ2].setAgachado(true); 
        else if (botQuiereDefenderse) { 
            equipoJ2[indiceActivoJ2].setAgachado(false); 
            if (x2 > x1) equipoJ2[indiceActivoJ2].caminar(1.f); else equipoJ2[indiceActivoJ2].caminar(-1.f); 
        } else equipoJ2[indiceActivoJ2].setAgachado(false); 
    }
}

void CombateMusical::acumularEnergiaJ1(float cantidad) {
    energiaJ1 += cantidad; 
    if (energiaJ1 >= 100.f) { 
        energiaJ1 -= 100.f; 
        if (nivelesJ1 < 5) nivelesJ1++; 
        else energiaJ1 = 100.f; 
    }
}

void CombateMusical::acumularEnergiaJ2(float cantidad) {
    energiaJ2 += cantidad; 
    if (energiaJ2 >= 100.f) { 
        energiaJ2 -= 100.f; 
        if (nivelesJ2 < 5) nivelesJ2++; 
        else energiaJ2 = 100.f; 
    }
}

void CombateMusical::cargarAvatarsUI() {
    if (indiceActivoJ1 < 3) { 
        std::string ruta = equipoJ1[indiceActivoJ1].getRutaAvatar(); 
        sf::Image imgJ1;
        
        if (imgJ1.loadFromFile(ruta) || imgJ1.loadFromFile("../" + ruta)) { 
            // Forzamos la transparencia del color de fondo gris (115,115,115) exacto de la hoja de sprites
            sf::Color colorGrisFondo(115, 115, 115);
            imgJ1.createMaskFromColor(colorGrisFondo);
            
            texturaAvatarJ1.loadFromImage(imgJ1);
            spriteAvatarJ1.setTexture(texturaAvatarJ1, true); 
            
            // Evaluamos matemáticamente qué personaje es para aplicar su grilla exacta del primer frame (Idle)
            unsigned int anchoF = 0;
            unsigned int altoF = 0;
            if (ruta.find("kai") != std::string::npos) {
                anchoF = 128;
                altoF = 140;
            } else {
                anchoF = 130;
                altoF = 150;
            }

            // Recortamos el frame superior izquierdo (rostro estático) para que no arrastre restos de la animación
            spriteAvatarJ1.setTextureRect(sf::IntRect(0, 0, anchoF, altoF)); 
            spriteAvatarJ1.setScale(85.f / anchoF, 85.f / altoF); 
            
            spriteAvatarJ1.setColor(sf::Color::White); 
            std::cout << "[LOG] Avatar UI J1 cargado con exito: " << ruta << "\n"; 
        } else { 
            sf::Image img; img.create(85, 85, sf::Color(50, 50, 50)); texturaAvatarJ1.loadFromImage(img); 
            spriteAvatarJ1.setTexture(texturaAvatarJ1, true); spriteAvatarJ1.setScale(1.f, 1.f); 
            spriteAvatarJ1.setColor(sf::Color::Cyan); 
            std::cerr << "[ERROR] No se encontro la imagen J1 en: " << ruta << "\n"; 
        }
    }
    
    if (indiceActivoJ2 < 3) { 
        std::string ruta = equipoJ2[indiceActivoJ2].getRutaAvatar(); 
        sf::Image imgJ2;
        
        if (imgJ2.loadFromFile(ruta) || imgJ2.loadFromFile("../" + ruta)) { 
            // Forzamos la transparencia del color de fondo gris (115,115,115) exacto de la hoja de sprites
            sf::Color colorGrisFondo(115, 115, 115);
            imgJ2.createMaskFromColor(colorGrisFondo);
            
            texturaAvatarJ2.loadFromImage(imgJ2);
            spriteAvatarJ2.setTexture(texturaAvatarJ2, true); 
            
            // Evaluamos matemáticamente qué personaje es para aplicar su grilla exacta del primer frame (Idle)
            unsigned int anchoF = 0;
            unsigned int altoF = 0;
            if (ruta.find("kai") != std::string::npos) {
                anchoF = 128;
                altoF = 140;
            } else {
                anchoF = 130;
                altoF = 150;
            }

            spriteAvatarJ2.setTextureRect(sf::IntRect(0, 0, anchoF, altoF)); 
            spriteAvatarJ2.setScale(85.f / anchoF, 85.f / altoF); 
            
            spriteAvatarJ2.setColor(sf::Color::White); 
            std::cout << "[LOG] Avatar UI J2 cargado con exito: " << ruta << "\n"; 
        } else { 
            sf::Image img; img.create(85, 85, sf::Color(50, 50, 50)); texturaAvatarJ2.loadFromImage(img); 
            spriteAvatarJ2.setTexture(texturaAvatarJ2, true); spriteAvatarJ2.setScale(1.f, 1.f); 
            spriteAvatarJ2.setColor(sf::Color::Red); 
            std::cerr << "[ERROR] No se encontro la imagen J2 en: " << ruta << "\n2"; 
        }
    }
}