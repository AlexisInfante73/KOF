#include "combatemusical.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

CombateMusical::CombateMusical() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    // Escenario aleatorio para la pelea posterior
    int escenarioAleatorio = (std::rand() % 4) + 1;
    std::string rutaFondo = "assets/images/fondo_batalla_" + std::to_string(escenarioAleatorio) + ".png";

    if (!texturaEscenario.loadFromFile(rutaFondo)) {
        texturaEscenario.loadFromFile("../" + rutaFondo);
    }
    spriteEscenario.setTexture(texturaEscenario);
    spriteEscenario.setScale(1280.f / texturaEscenario.getSize().x, 720.f / texturaEscenario.getSize().y);

    if (!fuenteUI.loadFromFile("assets/fonts/COMIC.TTF")) {
        fuenteUI.loadFromFile("../assets/fonts/COMIC.TTF");
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

    // --- TEXTOS DE RESERVA (TAMAÑO MÁXIMO ARCADE DE 48) ---
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

    golpeImpactadoEsteTurno = false;
    botGolpeImpactadoEsteTurno = false;
    rondaTerminada = false;
    floatProximaDecision = 0.3f;
    botQuiereAgacharse = false;
    botQuiereDefenderse = false; 

    // Ajustar tamaños de almacenamiento
    equipoJ1.resize(3);
    equipoJ2.resize(3);

    // --- ARRANCAR EN LA PANTALLA DE SELECCIÓN ---
    estadoActual = EstadoJuego::SeleccionPersonajes;
    inicializarPantallaSeleccion();
}

void CombateMusical::inicializarPantallaSeleccion() {
    if (!texturaSeleccion.loadFromFile("assets/images/fondo_seleccion.png")) {
        texturaSeleccion.loadFromFile("../assets/images/fondo_seleccion.png");
    }
    spriteSeleccion.setTexture(texturaSeleccion);
    spriteSeleccion.setScale(1280.f / texturaSeleccion.getSize().x, 720.f / texturaSeleccion.getSize().y);

    // Calibrado a las dimensiones exactas de tus tarjetas arcade (74x115 píxeles)
    selectorCuadrula.setSize(sf::Vector2f(74.f, 115.f));
    selectorCuadrula.setFillColor(sf::Color::Transparent);
    selectorCuadrula.setOutlineColor(sf::Color::Cyan); // Inicia J1 (Cyan)
    selectorCuadrula.setOutlineThickness(4.f);

    filaSeleccionada = 0;
    colSeleccionada = 0;

    tiempoSeleccionRestante = 15.f; // Ajustado a tus 15 segundos del .h
    txtTiempoSeleccion.setFont(fuenteUI);
    txtTiempoSeleccion.setCharacterSize(42);
    txtTiempoSeleccion.setFillColor(sf::Color(0, 255, 255)); // Cyan neón digital

    // Limpiamos los contenedores dinámicos para que no tengan basura previa
    equipoJ1.clear();
    equipoJ2.clear();
    relojSeleccion.restart();
}

void CombateMusical::procesarEntrada(sf::Event& evento) {
    if (estadoActual == EstadoJuego::SeleccionPersonajes) {
        if (evento.type == sf::Event::KeyPressed) {
            // Movimiento por la matriz 2x6
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

            // Confirmación de selección (Enter / Espacio)
            if (evento.key.code == sf::Keyboard::Enter || evento.key.code == sf::Keyboard::Space) {
                std::vector<std::string> listaNombres = {
                    "KYO", "TERRY", "RYO", "IORI", "KULA", "K'",
                    "MAI", "ATHENA", "KIM", "JOE", "BENIMARU", "CHIZURU"
                };
                int indice = (filaSeleccionada * 6) + colSeleccionada;
                std::string nombreElegido = listaNombres[indice];

                // Llenar primero el equipo del Jugador 1
                if (equipoJ1.size() < 3) {
                    Personaje p;
                    p.inicializar(nombreElegido, "assets/portraits/" + nombreElegido + ".png", sf::Color::Cyan, 300.f);
                    equipoJ1.push_back(p);
                    
                    if (equipoJ1.size() == 3) {
                        selectorCuadrula.setOutlineColor(sf::Color::Red); // El cursor cambia a rojo para el Bot
                    }
                }
                // Luego llenar el equipo del Bot (J2)
                else if (equipoJ2.size() < 3) {
                    Personaje p;
                    p.inicializar(nombreElegido, "assets/portraits/" + nombreElegido + ".png", sf::Color::Red, 980.f);
                    equipoJ2.push_back(p);

                    if (equipoJ2.size() == 3) {
                        // Selección lista -> Disparar e iniciar el combate en sí
                        reiniciarRelojes(2);
                        estadoActual = EstadoJuego::Combate;
                    }
                }
            }
        }
        return; // Detener procesamiento para que las teclas de selección no afecten la pelea
    }

    // --- ENTRADAS DURANTE EL COMBATE (PROPIO DE TU CÓDIGO) ---
    if (rondaTerminada) return; 
    if (evento.type == sf::Event::KeyPressed) {
        if (evento.key.code == sf::Keyboard::Space) equipoJ1[indiceActivoJ1].saltar();
        
        if (evento.key.code == sf::Keyboard::H) { equipoJ1[indiceActivoJ1].lanzarAtaque(1); golpeImpactadoEsteTurno = false; acumularEnergiaJ1(1.f); }
        if (evento.key.code == sf::Keyboard::J) { equipoJ1[indiceActivoJ1].lanzarAtaque(2); golpeImpactadoEsteTurno = false; acumularEnergiaJ1(1.f); }
        if (evento.key.code == sf::Keyboard::K) { equipoJ1[indiceActivoJ1].lanzarAtaque(3); golpeImpactadoEsteTurno = false; acumularEnergiaJ1(1.f); }
        if (evento.key.code == sf::Keyboard::L) { equipoJ1[indiceActivoJ1].lanzarAtaque(4); golpeImpactadoEsteTurno = false; acumularEnergiaJ1(1.f); }
        
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
    if (estadoActual == EstadoJuego::SeleccionPersonajes) {
        // Manejo del contador regresivo de selección
        if (relojSeleccion.getElapsedTime().asSeconds() >= 1.f) {
            if (tiempoSeleccionRestante > 0.f) {
                tiempoSeleccionRestante -= 1.f;
            } else {
                // Autocompletado aleatorio en caso de agotarse el tiempo
                std::vector<std::string> listaReserva = {"KYO", "TERRY", "RYO", "IORI", "KULA", "K'"};
                while(equipoJ1.size() < 3) {
                    Personaje p; p.inicializar(listaReserva[std::rand()%6], "", sf::Color::Cyan, 300.f);
                    equipoJ1.push_back(p);
                }
                while(equipoJ2.size() < 3) {
                    Personaje p; p.inicializar(listaReserva[std::rand()%6], "", sf::Color::Red, 980.f);
                    equipoJ2.push_back(p);
                }
                reiniciarRelojes(2);
                estadoActual = EstadoJuego::Combate;
            }
            relojSeleccion.restart();
        }
        int segs = static_cast<int>(tiempoSeleccionRestante);
        txtTiempoSeleccion.setString(segs < 10 ? "0" + std::to_string(segs) : std::to_string(segs));
        return; 
    }

    // --- ACTUALIZACIÓN DURANTE EL COMBATE ---
    if (rondaTerminada) {
        if (relojEsperaRonda.getElapsedTime().asSeconds() >= 3.0f) {
            if (indiceActivoJ1 >= 3 || indiceActivoJ2 >= 3) { 
                // Fin del juego: Volver a selección
                inicializarPantallaSeleccion(); 
                return; 
            }
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

    // --- PROCESAMIENTO DE DAÑO JUGADOR 1 AL BOT ---
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
                
                if (equipoJ2[indiceActivoJ2].getVida() <= 0.f) {
                    barraVidaJ2.setSize(sf::Vector2f(0.f, 25.f)); // Forzado de vaciado instantáneo
                    avanzarSiguienteRonda(1); 
                    return; 
                }
            }
            golpeImpactadoEsteTurno = true; 
        }
    }

    bool j1EnGuardia = false;
    if (x1 < x2 && sf::Keyboard::isKeyPressed(sf::Keyboard::A)) j1EnGuardia = true;
    if (x1 > x2 && sf::Keyboard::isKeyPressed(sf::Keyboard::D)) j1EnGuardia = true;

    // --- PROCESAMIENTO DE DAÑO BOT AL JUGADOR 1 ---
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
                
                if (equipoJ1[indiceActivoJ1].getVida() <= 0.f) {
                    barraVidaJ1.setSize(sf::Vector2f(0.f, 25.f)); // Forzado de vaciado instantáneo
                    avanzarSiguienteRonda(2); 
                    return; 
                }
            }
            botGolpeImpactadoEsteTurno = true;
        }
    }

    // --- ACTUALIZACIÓN ESTÁNDAR DE LAS BARRAS EN TIEMPO REAL ---
    float vidaJ1Proporcional = (equipoJ1[indiceActivoJ1].getVida() / 250.f) * 420.f;
    barraVidaJ1.setSize(sf::Vector2f(vidaJ1Proporcional > 0.f ? vidaJ1Proporcional : 0.f, 25.f));
    
    float anchoVerdeJ2 = (equipoJ2[indiceActivoJ2].getVida() / 250.f) * 420.f;
    if (anchoVerdeJ2 < 0.f) anchoVerdeJ2 = 0.f;
    barraVidaJ2.setSize(sf::Vector2f(anchoVerdeJ2, 25.f));
    barraVidaJ2.setPosition(1280.f - 135.f - anchoVerdeJ2, 42.f);

    if (nivelesJ1 == 5) {
        barraEspecialJ1.setSize(sf::Vector2f(320.f, 18.f)); 
    } else {
        barraEspecialJ1.setSize(sf::Vector2f((energiaJ1 / 100.f) * 320.f, 18.f));
    }

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
    
    // --- DISTRIBUCIÓN VERTICAL COMPACTADA CON ULTRA TAMAÑO (48) ---
    float inicioY = 75.f;    
    float espaciadoY = 52.f; 

    // Lista Reservas Jugador 1
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

    // Lista Reservas Jugador 2 (Bot)
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

    // --- CRONÓMETRO ---
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
        window.draw(spriteSeleccion);

        // Renderizado del Temporizador central inferior (Posicionado sobre el HUD)
        txtTiempoSeleccion.setPosition(640.f - txtTiempoSeleccion.getLocalBounds().width / 2.f, 485.f);
        window.draw(txtTiempoSeleccion);

        // Dibujar dinámicamente los nombres de la escuadra en los casilleros del marco inferior
        for (size_t i = 0; i < equipoJ1.size(); ++i) {
            sf::Text t(equipoJ1[i].getNombre(), fuenteUI, 22);
            t.setFillColor(sf::Color::White); 
            t.setPosition(125.f + (i * 105.f), 645.f); // Ranuras J1 (Lado Izquierdo)
            window.draw(t);
        }
        for (size_t i = 0; i < equipoJ2.size(); ++i) {
            sf::Text t(equipoJ2[i].getNombre(), fuenteUI, 22);
            t.setFillColor(sf::Color::White); 
            t.setPosition(685.f + (i * 105.f), 645.f); // Ranuras Bot (Lado Derecho)
            window.draw(t);
        }

        // Renderizado del selector arcade dinámico (Matriz de 2 filas x 6 columnas)
        float startGridX = 405.f; float startGridY = 175.f;
        float slotW = 74.f;       float slotH = 115.f;
        float spacingX = 14.f;    float spacingY = 14.f;

        float finalX = startGridX + colSeleccionada * (slotW + spacingX);
        float finalY = startGridY + filaSeleccionada * (slotH + spacingY);
        selectorCuadrula.setPosition(finalX, finalY);

        window.draw(selectorCuadrula);
        return; 
    }

    // --- RENDERIZADO DEL COMBATE RECEPTIVO ---
    window.draw(spriteEscenario);

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
    if (indiceActivoJ1 < 3) window.draw(spriteAvatarJ1);
    if (indiceActivoJ2 < 3) window.draw(spriteAvatarJ2);

    window.draw(fondoEspecialJ1); window.draw(fondoEspecialJ2);
    window.draw(barraEspecialJ1); window.draw(barraEspecialJ2);
    window.draw(txtNivelJ1);      window.draw(txtNivelJ2);

    if (rondaTerminada) window.draw(txtAnuncioKO);
}

void CombateMusical::reiniciarRelojes(int medallas) {
    relojPelea.restart();
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

    floatProximaDecision = 0.5f - (medallas * 0.08f);
    if (floatProximaDecision < 0.04f) floatProximaDecision = 0.04f; 

    // Inicialización definitiva basada en los personajes seleccionados de forma interactiva
    if (equipoJ1.size() >= 3) {
        equipoJ1[0].inicializar(equipoJ1[0].getNombre(), "assets/portraits/" + equipoJ1[0].getNombre() + ".png", sf::Color::Cyan, 300.f);
        equipoJ1[1].inicializar(equipoJ1[1].getNombre(), "assets/portraits/" + equipoJ1[1].getNombre() + ".png", sf::Color(30, 144, 255), 300.f);
        equipoJ1[2].inicializar(equipoJ1[2].getNombre(), "assets/portraits/" + equipoJ1[2].getNombre() + ".png", sf::Color(0, 0, 205), 300.f);
    }
    if (equipoJ2.size() >= 3) {
        equipoJ2[0].inicializar(equipoJ2[0].getNombre(), "assets/portraits/" + equipoJ2[0].getNombre() + ".png", sf::Color::Red, 980.f);
        equipoJ2[1].inicializar(equipoJ2[1].getNombre(), "assets/portraits/" + equipoJ2[1].getNombre() + ".png", sf::Color(255, 69, 0), 980.f);
        equipoJ2[2].inicializar(equipoJ2[2].getNombre(), "assets/portraits/" + equipoJ2[2].getNombre() + ".png", sf::Color(139, 0, 0), 980.f);
    }

    cargarAvatarsUI();
}

void CombateMusical::avanzarSiguienteRonda(int ganadorDeRonda) {
    rondaTerminada = true;
    relojEsperaRonda.restart();

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
        botQuiereAgacharse = false;
        botQuiereDefenderse = false;
        equipoJ2[indiceActivoJ2].setAgachado(false);
        if (x2 > x1) equipoJ2[indiceActivoJ2].caminar(-1.f); else equipoJ2[indiceActivoJ2].caminar(1.f);
    } else {
        if (relojDecisionBot.getElapsedTime().asSeconds() >= floatProximaDecision) {
            relojDecisionBot.restart();
            
            int decision = std::rand() % 100;
            
            if (jugadorAtacando && decision < 55) {
                botQuiereDefenderse = true; 
                botQuiereAgacharse = false;
            }
            else if (nivelesJ2 >= 1 && decision < 25) { 
                nivelesJ2--;
                equipoJ2[indiceActivoJ2].lanzarAtaque(5); 
                botGolpeImpactadoEsteTurno = false;
                botQuiereAgacharse = false;
                botQuiereDefenderse = false;
            } 
            else if (decision < 70) {
                equipoJ2[indiceActivoJ2].lanzarAtaque((std::rand() % 4) + 1);
                botGolpeImpactadoEsteTurno = false;
                botQuiereAgacharse = false;
                botQuiereDefenderse = false;
            } else {
                botQuiereAgacharse = true;
                botQuiereDefenderse = false;
            }
        }

        if (botQuiereAgacharse) {
            equipoJ2[indiceActivoJ2].setAgachado(true);
        } else if (botQuiereDefenderse) {
            equipoJ2[indiceActivoJ2].setAgachado(false);
            if (x2 > x1) equipoJ2[indiceActivoJ2].caminar(1.f); else equipoJ2[indiceActivoJ2].caminar(-1.f);
        } else {
            equipoJ2[indiceActivoJ2].setAgachado(false);
        }
    }
}

// =========================================================
// FUNCIONES FALTANTES PARA RESOLVER EL "UNDEFINED REFERENCE"
// =========================================================

void CombateMusical::acumularEnergiaJ1(float cantidad) {
    energiaJ1 += cantidad;
    if (energiaJ1 >= 100.f) {
        energiaJ1 -= 100.f; // Se reinicia la barra
        if (nivelesJ1 < 5) {
            nivelesJ1++;    // Sube de nivel si es menor a 5
        } else {
            energiaJ1 = 100.f; // Si ya es nivel 5, se queda a tope
        }
    }
}

void CombateMusical::acumularEnergiaJ2(float cantidad) {
    energiaJ2 += cantidad;
    if (energiaJ2 >= 100.f) {
        energiaJ2 -= 100.f;
        if (nivelesJ2 < 5) {
            nivelesJ2++;
        } else {
            energiaJ2 = 100.f;
        }
    }
}

void CombateMusical::cargarAvatarsUI() {
    // Evitamos errores si los equipos aún no están llenos
    if (equipoJ1.empty() || equipoJ2.empty()) return;
    if (indiceActivoJ1 >= 3 || indiceActivoJ2 >= 3) return;

    // Cargar dinámicamente las imágenes de los avatares según el personaje activo
    std::string rutaJ1 = "assets/portraits/" + equipoJ1[indiceActivoJ1].getNombre() + ".png";
    std::string rutaJ2 = "assets/portraits/" + equipoJ2[indiceActivoJ2].getNombre() + ".png";

    if (texturaAvatarJ1.loadFromFile(rutaJ1) || texturaAvatarJ1.loadFromFile("../" + rutaJ1)) {
        spriteAvatarJ1.setTexture(texturaAvatarJ1);
        // Ajustar escala para que quepa en el marco de 85x85
        spriteAvatarJ1.setScale(85.f / texturaAvatarJ1.getSize().x, 85.f / texturaAvatarJ1.getSize().y);
    }

    if (texturaAvatarJ2.loadFromFile(rutaJ2) || texturaAvatarJ2.loadFromFile("../" + rutaJ2)) {
        spriteAvatarJ2.setTexture(texturaAvatarJ2);
        spriteAvatarJ2.setScale(85.f / texturaAvatarJ2.getSize().x, 85.f / texturaAvatarJ2.getSize().y);
    }
}