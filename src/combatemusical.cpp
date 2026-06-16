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
    std::string rutaFondo = "assets/images/fondo_batalla_" + std::to_string(escenarioAleatorio) + ".png";

    if (!texturaEscenario.loadFromFile(rutaFondo)) {
        if (!texturaEscenario.loadFromFile("../" + rutaFondo)) {
            std::cerr << "ERROR: No se pudo cargar la textura del escenario: " << rutaFondo << "\n";
        }
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

    fondoBarraJ1.setSize(sf::Vector2f(420.f, 25.f));
    fondoBarraJ1.setFillColor(sf::Color(100, 0, 0));
    fondoBarraJ1.setPosition(135.f, 42.f); 

    fondoBarraJ2.setSize(sf::Vector2f(420.f, 25.f));
    fondoBarraJ2.setFillColor(sf::Color(100, 0, 0));
    fondoBarraJ2.setPosition(1280.f - 135.f - 420.f, 42.f); 

    barraVidaJ1.setFillColor(sf::Color::Green);
    barraVidaJ1.setPosition(135.f, 42.f);
    barraVidaJ2.setFillColor(sf::Color::Green);

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
    hitsJ1 = 0;      hitsJ2 = 0;

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
    botQuiereAgacharse = false;
    botQuiereDefenderse = false; 
    
    indiceActivoJ1 = 0;
    indiceActivoJ2 = 0;

    // Inicializamos la dificultad en Cero
    rachaVictorias = 0;

    cargarRoster();
    estadoActual = EstadoJuego::SeleccionPersonajes;
    inicializarPantallaSeleccion();
}

void CombateMusical::cargarRoster() {
    roster[0] = {"KYO", "assets/portraits/KYO.png", sf::Color::Cyan};
    roster[1] = {"TERRY", "assets/portraits/TERRY.png", sf::Color(30, 144, 255)};
    roster[2] = {"RYO", "assets/portraits/RYO.png", sf::Color(0, 0, 205)};
    roster[3] = {"IORI", "assets/portraits/IORI.png", sf::Color::Red};
    roster[4] = {"KULA", "assets/portraits/KULA.png", sf::Color(255, 69, 0)};
    roster[5] = {"K'", "assets/portraits/K'.png", sf::Color(139, 0, 0)};
    roster[6] = {"MAI", "assets/portraits/MAI.png", sf::Color::Red};
    roster[7] = {"ATHENA", "assets/portraits/ATHENA.png", sf::Color::Magenta};
    roster[8] = {"KIM", "assets/portraits/KIM.png", sf::Color::White};
    roster[9] = {"JOE", "assets/portraits/JOE.png", sf::Color::Yellow};
    roster[10]= {"BENIMARU", "assets/portraits/BENIMARU.png", sf::Color::Yellow};
    roster[11]= {"CHIZURU", "assets/portraits/CHIZURU.png", sf::Color::White};
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

    // --- GESTIÓN DE MÚSICA DE MENÚ ---
    if (musicaCombate.getStatus() == sf::Music::Playing) musicaCombate.stop();

    // Elegimos aleatoriamente entre menu1 y menu2
    std::random_device rd_m;
    std::mt19937 gen_m(rd_m());
    std::string cancionMenu = (gen_m() % 2 == 0) ? "assets/sounds/menu1.ogg" : "assets/sounds/menu2.ogg";

    if (musicaMenu.openFromFile(cancionMenu)) {
        musicaMenu.setLoop(true);
        musicaMenu.setVolume(50.f); // Volumen moderado para el menú
        musicaMenu.play();
    }

    selectorCuadrula.setSize(sf::Vector2f(104.f, 164.f)); 
    selectorCuadrula.setFillColor(sf::Color(0, 255, 255, 70)); 
    selectorCuadrula.setOutlineColor(sf::Color::Cyan); 
    selectorCuadrula.setOutlineThickness(4.f);

    fondoOcultarTiempo.setSize(sf::Vector2f(120.f, 40.f));
    fondoOcultarTiempo.setFillColor(sf::Color(10, 15, 35)); 
    fondoOcultarTiempo.setPosition(580.f, 520.f); 

    txtTiempoSeleccion.setFont(fuenteUI);
    txtTiempoSeleccion.setCharacterSize(42);
    txtTiempoSeleccion.setFillColor(sf::Color::White); 
    txtTiempoSeleccion.setPosition(585.f, 520.f); 

    for(int i=0; i<3; i++) {
        vistasPreviasJ1[i].setSize(sf::Vector2f(75.f, 75.f));
        vistasPreviasJ1[i].setOutlineColor(sf::Color::White);
        vistasPreviasJ1[i].setOutlineThickness(2.f);
        vistasPreviasJ1[i].setFillColor(sf::Color::Transparent);

        vistasPreviasJ2[i].setSize(sf::Vector2f(75.f, 75.f));
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

    if (rondaTerminada || indiceActivoJ1 >= 3 || indiceActivoJ2 >= 3) return; 
    
    // Corregido: Llamada necesaria para habilitar la detección de Sprint/Doble toque
    equipoJ1[indiceActivoJ1].verificarDobleToque(evento, equipoJ2[indiceActivoJ2].getPosicionX());

    if (evento.type == sf::Event::KeyPressed) {
        if (evento.key.code == sf::Keyboard::Space) equipoJ1[indiceActivoJ1].saltar();
        
        // --- MECÁNICA ANTI-SPAM DE BOTONES ---
        // Modificado para permitir combos si el personaje está en la ventana de tiempo correcta
        bool puedeAtacar = (!equipoJ1[indiceActivoJ1].getEstaAtacando() || equipoJ1[indiceActivoJ1].puedeHacerCombo()) && 
                           !equipoJ1[indiceActivoJ1].getEstaAturdido() && 
                           relojCooldownJ1.getElapsedTime().asSeconds() > 0.15f;

        if (puedeAtacar) {
            // Reiniciamos el cooldown del controlador al detectar una intención de ataque válida
            relojCooldownJ1.restart();

            if (evento.key.code == sf::Keyboard::H) { equipoJ1[indiceActivoJ1].lanzarAtaque(1); golpeImpactadoEsteTurno = false; }
            if (evento.key.code == sf::Keyboard::J) { equipoJ1[indiceActivoJ1].lanzarAtaque(2); golpeImpactadoEsteTurno = false; }
            if (evento.key.code == sf::Keyboard::K) { equipoJ1[indiceActivoJ1].lanzarAtaque(3); golpeImpactadoEsteTurno = false; }
            if (evento.key.code == sf::Keyboard::L) { equipoJ1[indiceActivoJ1].lanzarAtaque(4); golpeImpactadoEsteTurno = false; }
            
            if (evento.key.code == sf::Keyboard::E) {
                if (nivelesJ1 >= 1) {
                    // Ahora solo consume energía si el ataque realmente se ejecutó
                    if (equipoJ1[indiceActivoJ1].lanzarAtaque(5)) {
                        nivelesJ1--;
                        golpeImpactadoEsteTurno = false;
                    }
                }
            }
        }
    }
}

void CombateMusical::actualizar() {
    float deltaTime = relojDeltaTime.restart().asSeconds(); 

    if (estadoActual == EstadoJuego::SeleccionPersonajes) {
        // --- RE-CUADRADO DE LA SELECCIÓN ---
        // Ajustamos xInicial y espacioX para que la cuadrícula de 6 columnas esté centrada
        float xInicial = 313.f; 
        float yInicial = 145.f; 
        float espacioX = 110.f; 
        float espacioY = 170.f; 
        
        selectorCuadrula.setSize(sf::Vector2f(104.f, 164.f)); // Ajustamos el tamaño del selector al nuevo espacio
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
                reiniciarRelojes();
                estadoActual = EstadoJuego::Combate;

                // --- CAMBIO DE MÚSICA A COMBATE ---
                musicaMenu.stop();
                if (musicaCombate.openFromFile("assets/sounds/combate.ogg")) {
                    musicaCombate.setLoop(true);
                    musicaCombate.setVolume(60.f);
                    musicaCombate.play();
                }
            }
        }

        // Reposicionamos los cuadros de los personajes elegidos para que sean simétricos
        // J1 a la izquierda, J2 a la derecha (creciendo hacia el centro)
        for(int i = 0; i < 3; i++) vistasPreviasJ1[i].setPosition(100.f + (i * 85.f), 615.f); 
        for(int i = 0; i < 3; i++) vistasPreviasJ2[i].setPosition(1280.f - 100.f - 75.f - (i * 85.f), 615.f); 

        return; 
    }

    // --- CONTINÚA ACTUALIZACIÓN ORIGINAL DE COMBATE ---
    if (rondaTerminada) {
        if (relojEsperaRonda.getElapsedTime().asSeconds() >= 3.0f) {
            
            // Si todo un equipo fue derrotado, evaluamos la Racha de Victorias
            if (indiceActivoJ1 >= 3 || indiceActivoJ2 >= 3) { 
                if (indiceActivoJ2 >= 3) {
                    rachaVictorias++; // Ganaste, sube la dificultad
                } else {
                    rachaVictorias = 0; // Perdiste, se reinicia la dificultad
                }
                
                estadoActual = EstadoJuego::SeleccionPersonajes; // Volvemos al menú
                inicializarPantallaSeleccion();
                return; 
            }

            // Si es solo cambio de personaje
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
                equipoJ2[indiceActivoJ2].aplicarAturdimiento(0.15f); // Hitstun para evitar contra-spam
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
                equipoJ1[indiceActivoJ1].aplicarAturdimiento(0.15f); // Hitstun para el jugador
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

void CombateMusical::reiniciarRelojes() {
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
        // --- DIFICULTAD ESCALABLE DE LA IA ---
        // A mayor racha de victorias, reacciona más rápido (hasta un máximo de 0.05 segundos)
        float probReaccion = 0.5f - (rachaVictorias * 0.08f);
        if (probReaccion < 0.05f) probReaccion = 0.05f; 

        if (relojDecisionBot.getElapsedTime().asSeconds() >= probReaccion) {
            relojDecisionBot.restart();
            std::random_device rd; std::mt19937 gen(rd()); std::uniform_int_distribution<> dist(0, 99);
            int decision = dist(gen);
            
            // A mayor racha, más posibilidad de defenderse y lanzar especiales
            int probDefensa = 55 + (rachaVictorias * 8);
            if (probDefensa > 90) probDefensa = 90;

            int probEspecial = 25 + (rachaVictorias * 5);

            if (jugadorAtacando && decision < probDefensa) {
                botQuiereDefenderse = true; botQuiereAgacharse = false;
            } else if (nivelesJ2 >= 1 && decision < probEspecial) { 
                if (equipoJ2[indiceActivoJ2].lanzarAtaque(5)) {
                    nivelesJ2--; 
                    botGolpeImpactadoEsteTurno = false; botQuiereAgacharse = false; botQuiereDefenderse = false;
                }
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
        if (texturaAvatarJ1.loadFromFile(ruta) || texturaAvatarJ1.loadFromFile("../" + ruta)) {
            spriteAvatarJ1.setTexture(texturaAvatarJ1, true);
            spriteAvatarJ1.setScale(85.f / texturaAvatarJ1.getSize().x, 85.f / texturaAvatarJ1.getSize().y);
            spriteAvatarJ1.setColor(sf::Color::White);
        } else {
            sf::Image img; img.create(85, 85, sf::Color(50, 50, 50)); texturaAvatarJ1.loadFromImage(img);
            spriteAvatarJ1.setTexture(texturaAvatarJ1, true); spriteAvatarJ1.setScale(1.f, 1.f);
            spriteAvatarJ1.setColor(sf::Color::Cyan);
        }
    }
    if (indiceActivoJ2 < 3) {
        std::string ruta = equipoJ2[indiceActivoJ2].getRutaAvatar();
        if (texturaAvatarJ2.loadFromFile(ruta) || texturaAvatarJ2.loadFromFile("../" + ruta)) {
            spriteAvatarJ2.setTexture(texturaAvatarJ2, true);
            spriteAvatarJ2.setScale(85.f / texturaAvatarJ2.getSize().x, 85.f / texturaAvatarJ2.getSize().y);
            spriteAvatarJ2.setColor(sf::Color::White);
        } else {
            sf::Image img; img.create(85, 85, sf::Color(50, 50, 50)); texturaAvatarJ2.loadFromImage(img);
            spriteAvatarJ2.setTexture(texturaAvatarJ2, true); spriteAvatarJ2.setScale(1.f, 1.f);
            spriteAvatarJ2.setColor(sf::Color::Red);
        }
    }
}