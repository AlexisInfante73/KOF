#include "combatemusical.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

CombateMusical::CombateMusical() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
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

    // Texto de reserva (Abajo de las barras de vida en el centro)
    txtContadorOrden.setFont(fuenteUI);
    txtContadorOrden.setCharacterSize(16);
    txtContadorOrden.setFillColor(sf::Color(200, 200, 200));
    txtContadorOrden.setPosition(240.f, 75.f);

    // --- CONFIGURACIÓN DE NOMBRES ---
    txtNombreJ1.setFont(fuenteUI);
    txtNombreJ1.setCharacterSize(22);
    txtNombreJ1.setFillColor(sf::Color::White);
    txtNombreJ1.setOutlineColor(sf::Color::Black);
    txtNombreJ1.setOutlineThickness(1.5f);
    txtNombreJ1.setPosition(135.f, 12.f); // Alineado sobre el inicio de su barra

    txtNombreJ2.setFont(fuenteUI);
    txtNombreJ2.setCharacterSize(22);
    txtNombreJ2.setFillColor(sf::Color::White);
    txtNombreJ2.setOutlineColor(sf::Color::Black);
    txtNombreJ2.setOutlineThickness(1.5f);
    // Nota: La posición X del J2 se recalcula dinámicamente en actualizar() para alinearse a la derecha

    // --- MARCOS DE AVATARS (Extremos Absolutos) ---
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

    // --- CORRECCIÓN DE POSICIÓN: BARRAS DE VIDA ---
    // Bajamos la coordenada Y de 40.f (o de donde estuviera fallando) a 42.f estables
    fondoBarraJ1.setSize(sf::Vector2f(420.f, 25.f));
    fondoBarraJ1.setFillColor(sf::Color(100, 0, 0));
    fondoBarraJ1.setPosition(135.f, 42.f); 

    fondoBarraJ2.setSize(sf::Vector2f(420.f, 25.f));
    fondoBarraJ2.setFillColor(sf::Color(100, 0, 0));
    fondoBarraJ2.setPosition(1280.f - 135.f - 420.f, 42.f); 

    barraVidaJ1.setFillColor(sf::Color::Green);
    barraVidaJ1.setPosition(135.f, 42.f);

    barraVidaJ2.setFillColor(sf::Color::Green);

    // --- HUD INFERIOR (BARRAS DE SÚPER ENERGÍA) ---
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

    equipoJ1.resize(3);
    equipoJ2.resize(3);
    
    reiniciarRelojes(0);
}

void CombateMusical::acumularEnergiaJ1(float cantidad) {
    if (nivelesJ1 >= 3) return; 
    energiaJ1 += cantidad;
    if (energiaJ1 >= 100.f) {
        energiaJ1 -= 100.f;
        nivelesJ1++;
        if (nivelesJ1 > 3) { nivelesJ1 = 3; energiaJ1 = 0.f; }
    }
}

void CombateMusical::acumularEnergiaJ2(float cantidad) {
    if (nivelesJ2 >= 3) return;
    energiaJ2 += cantidad;
    if (energiaJ2 >= 100.f) {
        energiaJ2 -= 100.f;
        nivelesJ2++;
        if (nivelesJ2 > 3) { nivelesJ2 = 3; energiaJ2 = 0.f; }
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
            if (equipoJ1[indiceActivoJ1].getNombre() == "KYO") spriteAvatarJ1.setColor(sf::Color::Cyan);
            else if (equipoJ1[indiceActivoJ1].getNombre() == "TERRY") spriteAvatarJ1.setColor(sf::Color(30, 144, 255));
            else spriteAvatarJ1.setColor(sf::Color::Blue);
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
            if (equipoJ2[indiceActivoJ2].getNombre() == "IORI") spriteAvatarJ2.setColor(sf::Color::Red);
            else if (equipoJ2[indiceActivoJ2].getNombre() == "KULA") spriteAvatarJ2.setColor(sf::Color(255, 69, 0));
            else spriteAvatarJ2.setColor(sf::Color::Magenta);
        }
    }
}

void CombateMusical::procesarEntrada(sf::Event& evento) {
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

    equipoJ1[0].inicializar("KYO",   "assets/portraits/kyo.png",   sf::Color::Cyan, 300.f);
    equipoJ1[1].inicializar("TERRY", "assets/portraits/terry.png", sf::Color(30, 144, 255), 300.f);
    equipoJ1[2].inicializar("RYO",   "assets/portraits/ryo.png",   sf::Color(0, 0, 205), 300.f);

    equipoJ2[0].inicializar("IORI",  "assets/portraits/iori.png",  sf::Color::Red, 980.f);
    equipoJ2[1].inicializar("KULA",  "assets/portraits/kula.png",  sf::Color(255, 69, 0), 980.f);
    equipoJ2[2].inicializar("K'",    "assets/portraits/kash.png",  sf::Color(139, 0, 0), 980.f);

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

    if (distancia > 115.f) {
        botQuiereAgacharse = false;
        equipoJ2[indiceActivoJ2].setAgachado(false);
        if (x2 > x1) equipoJ2[indiceActivoJ2].caminar(-1.f); else equipoJ2[indiceActivoJ2].caminar(1.f);
    } else {
        if (relojDecisionBot.getElapsedTime().asSeconds() >= floatProximaDecision) {
            relojDecisionBot.restart();
            floatProximaDecision = 0.2f + (static_cast<float>(std::rand()) / RAND_MAX) * 0.3f;
            
            int decision = std::rand() % 100;
            
            if (nivelesJ2 >= 1 && decision < 25) { 
                nivelesJ2--;
                equipoJ2[indiceActivoJ2].lanzarAtaque(5); 
                botGolpeImpactadoEsteTurno = false;
                botQuiereAgacharse = false;
            } 
            else if (decision < 70) {
                equipoJ2[indiceActivoJ2].lanzarAtaque((std::rand() % 4) + 1);
                botGolpeImpactadoEsteTurno = false;
                botQuiereAgacharse = false;
            } else {
                botQuiereAgacharse = true;
            }
        }
        if (botQuiereAgacharse) equipoJ2[indiceActivoJ2].setAgachado(true); else equipoJ2[indiceActivoJ2].setAgachado(false);
    }
}

void CombateMusical::actualizar() {
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

    // Lógica de impactos J1 -> J2
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

            if (dmg > 0.f) {
                equipoJ2[indiceActivoJ2].recibirDanio(dmg);
                acumularEnergiaJ2(6.f); 
                if (equipoJ2[indiceActivoJ2].getVida() <= 0.f) { avanzarSiguienteRonda(1); return; }
            }
            golpeImpactadoEsteTurno = true; 
        }
    }

    // Lógica de impactos J2 -> J1
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

            if (dmg > 0.f) {
                equipoJ1[indiceActivoJ1].recibirDanio(dmg);
                acumularEnergiaJ1(6.f); 
                if (equipoJ1[indiceActivoJ1].getVida() <= 0.f) { avanzarSiguienteRonda(2); return; }
            }
            botGolpeImpactadoEsteTurno = true;
        }
    }

    // --- CORRECCIÓN DE ALTURA EN EL ACTUALIZAR ---
    barraVidaJ1.setSize(sf::Vector2f((equipoJ1[indiceActivoJ1].getVida() / 250.f) * 420.f, 25.f));
    
    float anchoVerdeJ2 = (equipoJ2[indiceActivoJ2].getVida() / 250.f) * 420.f;
    barraVidaJ2.setSize(sf::Vector2f(anchoVerdeJ2, 25.f));
    barraVidaJ2.setPosition(1280.f - 135.f - anchoVerdeJ2, 42.f); // Mantiene Y en 42.f fija

    // Comportamiento de las Barras de Súper Energía
    if (nivelesJ1 == 3) {
        barraEspecialJ1.setSize(sf::Vector2f(320.f, 18.f)); 
    } else {
        barraEspecialJ1.setSize(sf::Vector2f((energiaJ1 / 100.f) * 320.f, 18.f));
    }

    if (nivelesJ2 == 3) {
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
    txtNombreJ2.setPosition(1280.f - 135.f - txtNombreJ2.getLocalBounds().width, 12.f); // Alineado con barra derecha
    
    txtContadorOrden.setString("Aliados en reserva: " + std::to_string(3 - (indiceActivoJ1 + 1)) + " | Rivales en reserva: " + std::to_string(3 - (indiceActivoJ2 + 1)));
    txtContadorOrden.setPosition(1280.f / 2.f - txtContadorOrden.getLocalBounds().width / 2.f, 75.f);

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
    window.draw(spriteEscenario);

    if (indiceActivoJ1 < 3) equipoJ1[indiceActivoJ1].dibujar(window);
    if (indiceActivoJ2 < 3) equipoJ2[indiceActivoJ2].dibujar(window);

    // HUD Superior
    window.draw(fondoBarraJ1); window.draw(fondoBarraJ2);
    window.draw(barraVidaJ1); window.draw(barraVidaJ2);
    window.draw(txtCronometro); window.draw(txtContadorOrden);
    window.draw(txtNombreJ1); window.draw(txtNombreJ2);
    window.draw(marcoAvatarJ1); window.draw(marcoAvatarJ2);
    if (indiceActivoJ1 < 3) window.draw(spriteAvatarJ1);
    if (indiceActivoJ2 < 3) window.draw(spriteAvatarJ2);

    // HUD Inferior
    window.draw(fondoEspecialJ1); window.draw(fondoEspecialJ2);
    window.draw(barraEspecialJ1); window.draw(barraEspecialJ2);
    window.draw(txtNivelJ1);      window.draw(txtNivelJ2);

    if (rondaTerminada) window.draw(txtAnuncioKO);
}