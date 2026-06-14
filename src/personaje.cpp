#include "Personaje.h"
#include <cmath>
#include <iostream>

Personaje::Personaje() {
    radioOriginal = 40.f;
    enElSuelo = true;
    estaAgachado = false;
    estaAtacando = false;
    estaRodando = false; 
    estaAturdido = false; 
    direccionRodada = 0.f; 
    tiempoAturdimiento = 0.f;
    esSuperAtaque = false;
    tipoAtaque = 0;
    vida = 250.f; 
    comboStep = 0;
    nombre = "Anonimo";
    rutaAvatar = "";
    posicion = sf::Vector2f(0.f, 0.f);
    velocidad = sf::Vector2f(0.f, 0.f);
    
    estaEnElAire = false;
    estaAtacandoAire = false;
    tipoAtaqueAire = 0;
    
    presionoIzquierdaAntes = false;
    presionoDerechaAntes = false;
    tiempoLimiteDobleToque = 0.25f; 
    estaCorriendo = false;
    
    anchoFrame = 150; 
    altoFrame = 200;  
    frameActualCol = 0;
    maxFramesAccion = 7;
    accionActual = "caminar";

    cuerpoShape.setRadius(radioOriginal);
    cuerpoShape.setOrigin(radioOriginal, radioOriginal * 2.f); 
}

void Personaje::inicializar(std::string nombrePeleador, std::string rutaImg, sf::Color color, float xInicial) {
    nombre = nombrePeleador;
    rutaAvatar = rutaImg;
    colorBase = color;
    cuerpoShape.setFillColor(colorBase);
    frameActualCol = 0;

    // --- DETECTOR DINÁMICO DE CARPETAS ---
    std::string nombreLower = nombrePeleador;
    for (auto& c : nombreLower) c = std::tolower(c);
    std::string carpetaBase = "assets/portraits/" + nombreLower + "/";

    // Mapeo con los archivos de Kai (Asegúrate de que los nombres coincidan exactamente con tus archivos)
    std::map<std::string, std::string> archivosMapeados = {
        { "caminar",   "kai_caminar.jpg" }, // Cambiado a .jpg basándome en tus capturas
        { "agacharse", "Kai_agacharse.jpg" }, 
        { "carga",     "kai_carga.jpg" },
        { "arrojado",  "kai_arrojado.jpg" },
        { "derrota",   "kai_derrota.jpg" }
    };

    // Cargamos las imágenes y BORRAMOS EL GRIS
    for (auto const& [accion, archivo] : archivosMapeados) {
        sf::Image img;
        std::string rutaFinal = carpetaBase + archivo;
        
        // Intenta cargar con .jpg o .png por si cambiaste la extensión
        if (img.loadFromFile(rutaFinal) || img.loadFromFile("../" + rutaFinal) || 
            img.loadFromFile(carpetaBase + accion + ".png")) {
            
            // Borra el fondo gris. 
            // NOTA: Si sigue viéndose gris, es porque el tono de tu imagen no es exactamente 128,128,128.
            img.createMaskFromColor(sf::Color(128, 128, 128)); 
            // img.createMaskFromColor(sf::Color(115, 115, 115)); // Descomenta esta línea y borra la de arriba si el gris no desaparece
            
            sf::Texture tex;
            tex.loadFromImage(img);
            mapaTexturas[accion] = tex;
        } else {
            std::cout << "[ERROR GRAVE]: No se pudo cargar la hoja: " << rutaFinal << std::endl;
        }
    }

    accionActual = "caminar";
    if (!mapaTexturas.empty() && mapaTexturas.find("caminar") != mapaTexturas.end()) {
        inyectarTextura("caminar");
        spriteCombate.setScale(2.2f, 2.2f); // Escalado ideal de combate
    }
    
    posicion = sf::Vector2f(xInicial, ALTURA_SUELO);
    velocidad = sf::Vector2f(0.f, 0.f);
    vida = 250.f; 
    comboStep = 0;
    estaAtacando = false;
    esSuperAtaque = false;
    estaAgachado = false;
    estaRodando = false; 
    estaCorriendo = false;
    estaAturdido = false; 
    estaEnElAire = false;
    estaAtacandoAire = false;
    tipoAtaqueAire = 0;
    presionoIzquierdaAntes = false;
    presionoDerechaAntes = false;

    cuerpoShape.setRadius(radioOriginal);
    cuerpoShape.setOrigin(radioOriginal, radioOriginal * 2.f);
    cuerpoShape.setPosition(posicion);
}

void Personaje::inyectarTextura(const std::string& claveAccion) {
    if (accionActual == claveAccion) return; // Evitamos recargas innecesarias
    
    accionActual = claveAccion;
    frameActualCol = 0; // Reiniciamos contador de animación
    
    if (mapaTexturas.find(claveAccion) != mapaTexturas.end()) {
        spriteCombate.setTexture(mapaTexturas[claveAccion]);
        
        // --- CÁLCULO MATEMÁTICO AUTOMÁTICO DE TAMAÑOS ---
        sf::Vector2u tamanoTextura = mapaTexturas[claveAccion].getSize();
        
        if (claveAccion == "caminar") {
            maxFramesAccion = 7; 
            altoFrame = tamanoTextura.y; 
            anchoFrame = tamanoTextura.x / 7; 
        } else if (claveAccion == "agacharse") {
            maxFramesAccion = 4; 
            altoFrame = tamanoTextura.y; 
            anchoFrame = tamanoTextura.x / 4;
        } else if (claveAccion == "carga") {
            maxFramesAccion = 5; 
            altoFrame = tamanoTextura.y; 
            anchoFrame = tamanoTextura.x / 5;
        } else if (claveAccion == "derrota") {
            maxFramesAccion = 5; 
            altoFrame = tamanoTextura.y; 
            anchoFrame = tamanoTextura.x / 5;
        } else if (claveAccion == "arrojado") {
            // ¡CASO ESPECIAL! Esta imagen tiene 2 filas y 6 columnas
            maxFramesAccion = 12; 
            altoFrame = tamanoTextura.y / 2; 
            anchoFrame = tamanoTextura.x / 6;
        }

        spriteCombate.setOrigin(anchoFrame / 2.f, (float)altoFrame);
    }
}

void Personaje::verificarDobleToque(sf::Event& evento, float xRival) {
    if (estaAturdido) return; 

    if (evento.type == sf::Event::KeyPressed) {
        bool rivalALaDerecha = (xRival > posicion.x);
        if (evento.key.code == sf::Keyboard::D) {
            if (rivalALaDerecha) { 
                if (relojDobleToque.getElapsedTime().asSeconds() < tiempoLimiteDobleToque && presionoDerechaAntes) {
                    estaCorriendo = true;
                } else {
                    relojDobleToque.restart();
                    presionoDerechaAntes = true;
                    presionoIzquierdaAntes = false;
                }
            }
        }
        else if (evento.key.code == sf::Keyboard::A) {
            if (!rivalALaDerecha) { 
                if (relojDobleToque.getElapsedTime().asSeconds() < tiempoLimiteDobleToque && presionoIzquierdaAntes) {
                    estaCorriendo = true;
                } else {
                    relojDobleToque.restart();
                    presionoIzquierdaAntes = true;
                    presionoDerechaAntes = false;
                }
            }
        }
    }

    if (evento.type == sf::Event::KeyReleased) {
        if (evento.key.code == sf::Keyboard::D || evento.key.code == sf::Keyboard::A) {
            estaCorriendo = false;
        }
    }
}

void Personaje::caminar(float direccion, bool correr) {
    if (estaAturdido || estaRodando) return; 

    if (estaAgachado && !estaAtacando && direccion != 0.f && enElSuelo) {
        estaRodando = true;
        direccionRodada = (direccion > 0.f) ? 1.f : -1.f;
        relojRodada.restart();
        estaAgachado = false; 
        
        cuerpoShape.setRadius(radioOriginal / 2.f);
        cuerpoShape.setOrigin(radioOriginal / 2.f, radioOriginal);
        return;
    }

    if (!estaAgachado && !estaAtacando) { 
        float velActual = (estaCorriendo || correr) ? VEL_SPRINT : VEL_CAMINAR;
        velocidad.x = direccion * velActual;
    } else {
        velocidad.x = 0.f;
    }
}

void Personaje::saltar() {
    if (enElSuelo && !estaAgachado && !estaAtacando && !estaRodando && !estaAturdido) {
        velocidad.y = FUERZA_SALTO; 
        enElSuelo = false;
        estaEnElAire = true; 
        estaCorriendo = false; 
    }
}

void Personaje::setAgachado(bool agachado) {
    if (estaAtacando || estaRodando || estaAturdido) return; 
    if (agachado && !enElSuelo) return;

    estaAgachado = agachado;
    if (estaAgachado) {
        estaCorriendo = false; 
        cuerpoShape.setRadius(radioOriginal / 2.f);
        cuerpoShape.setOrigin(radioOriginal / 2.f, radioOriginal);
    } else {
        cuerpoShape.setRadius(radioOriginal);
        cuerpoShape.setOrigin(radioOriginal, radioOriginal * 2.f);
    }
}

void Personaje::lanzarAtaque(int tipo) {
    bool puedeHacerCombo = false;
    if (estaAtacando) {
        float tiempoTranscurrido = relojAtaque.getElapsedTime().asSeconds();
        if (tiempoTranscurrido > DURACION_ATAQUE * 0.6f && comboStep < 3) {
            puedeHacerCombo = true;
        }
    }

    if ((!estaAtacando || puedeHacerCombo) && !estaRodando && !estaAturdido && enElSuelo) { 
        estaAtacando = true;
        tipoAtaque = tipo;
        comboStep = puedeHacerCombo ? comboStep + 1 : 1;
        relojAtaque.restart();
        velocidad.x = 0.f; 

        if (tipo == 1) cuerpoShape.setFillColor(sf::Color(255, 255, 150)); 
        if (tipo == 2) cuerpoShape.setFillColor(sf::Color::Yellow);        
        if (tipo == 3) cuerpoShape.setFillColor(sf::Color(255, 150, 255)); 
        if (tipo == 4) cuerpoShape.setFillColor(sf::Color::Magenta);        
        
        if (tipo == 5) {
            esSuperAtaque = true;
            cuerpoShape.setFillColor(sf::Color::White);
        }
    }
}

void Personaje::lanzarAtaqueAereo(int tipo) {
    if (!estaAtacandoAire && !estaAturdido && !enElSuelo) {
        estaAtacandoAire = true;
        tipoAtaqueAire = tipo;
        relojAtaque.restart(); 
        
        if (tipo == 1) cuerpoShape.setFillColor(sf::Color(150, 255, 255)); 
        if (tipo == 2) cuerpoShape.setFillColor(sf::Color::Cyan);
    }
}

void Personaje::aplicarAturdimiento(float duracion) {
    if (estaRodando) return; 
    
    estaAturdido = true;
    tiempoAturdimiento = duracion;
    relojAturdimiento.restart();
    
    estaAtacando = false;
    estaAtacandoAire = false; 
    esSuperAtaque = false;
    estaCorriendo = false;
    tipoAtaque = 0;
    tipoAtaqueAire = 0; 
    velocidad.x = 0.f; 

    cuerpoShape.setFillColor(sf::Color(150, 150, 150)); 
}

void Personaje::serLanzado(float fuerzaX, float fuerzaY) {
    if (estaRodando) return;
    
    aplicarAturdimiento(1.0f); 
    velocidad.x = fuerzaX;
    velocidad.y = fuerzaY; 
    enElSuelo = false;
    estaEnElAire = true; 
}

void Personaje::actualizar() {
    // --- LÓGICA DE ESTADOS Y FÍSICAS ---
    if (estaAturdido) {
        if (relojAturdimiento.getElapsedTime().asSeconds() >= tiempoAturdimiento) {
            estaAturdido = false;
            cuerpoShape.setFillColor(colorBase); 
        }
        if (enElSuelo && std::abs(velocidad.x) > 0.f) {
            velocidad.x *= 0.8f; 
            if (std::abs(velocidad.x) < 0.5f) velocidad.x = 0.f;
        }
    }

    if (estaRodando) {
        if (relojRodada.getElapsedTime().asSeconds() >= DURACION_RODADA) {
            estaRodando = false;
            cuerpoShape.setRadius(radioOriginal);
            cuerpoShape.setOrigin(radioOriginal, radioOriginal * 2.f);
            cuerpoShape.setFillColor(colorBase); 
        } else {
            velocidad.x = direccionRodada * VEL_RODADA;
            cuerpoShape.setFillColor(sf::Color(colorBase.r, colorBase.g, colorBase.b, 130));
        }
    }

    if (estaAtacando && !estaAturdido) {
        if (relojAtaque.getElapsedTime().asSeconds() >= DURACION_ATAQUE) {
            estaAtacando = false;
            esSuperAtaque = false;
            comboStep = 0;
            tipoAtaque = 0;
            cuerpoShape.setFillColor(colorBase); 
        }
    }

    if (estaAtacandoAire && !estaAturdido) {
        if (relojAtaque.getElapsedTime().asSeconds() >= DURACION_ATAQUE) {
            estaAtacandoAire = false;
            tipoAtaqueAire = 0;
            cuerpoShape.setFillColor(colorBase);
        }
    }

    if (!enElSuelo) {
        velocidad.y += GRAVEDAD;
        estaEnElAire = true; 
    }

    posicion.x += velocidad.x;
    posicion.y += velocidad.y;
    
    if (!estaAturdido && !estaRodando && !estaCorriendo && !estaEnElAire) {
        velocidad.x = 0.f; 
    }

    if (posicion.y >= ALTURA_SUELO) {
        posicion.y = ALTURA_SUELO;
        if (velocidad.y > 0.f) velocidad.y = 0.f;
        enElSuelo = true;
        estaEnElAire = false; 
        estaAtacandoAire = false; 
    }

    if (posicion.x < 40.f) posicion.x = 40.f;
    if (posicion.x > 1240.f) posicion.x = 1240.f;

    // --- INTERRUPTOR DE MÁQUINA DE ANIMACIÓN ---
    if (!mapaTexturas.empty()) {
        if (vida <= 0.f)                       inyectarTextura("derrota");
        else if (estaAturdido)                 inyectarTextura("arrojado");
        else if (esSuperAtaque)                inyectarTextura("carga"); 
        else if (estaAtacando)                 inyectarTextura("carga"); 
        else if (estaRodando)                  inyectarTextura("agacharse");
        else if (estaAgachado)                 inyectarTextura("agacharse");
        else if (std::abs(velocidad.x) > 0.1f) inyectarTextura("caminar");
        else                                   inyectarTextura("caminar"); // Frame 0 = Idle

        // Reloj del refresco de los frames
        if (relojAnimacion.getElapsedTime().asSeconds() > 0.11f) {
            frameActualCol++;
            
            if (vida <= 0.f && frameActualCol >= maxFramesAccion) {
                frameActualCol = maxFramesAccion - 1;
            } else {
                frameActualCol %= maxFramesAccion;
            }
            relojAnimacion.restart();
        }

        // --- SISTEMA DE RECORTES (SOPORTA FILAS Y COLUMNAS) ---
        int columnaCalculada = frameActualCol;
        int filaCalculada = 0;

        // Si la acción es arrojado, dividimos la animación en 2 filas
        if (accionActual == "arrojado") {
            columnaCalculada = frameActualCol % 6; 
            filaCalculada = frameActualCol / 6;
        }

        spriteCombate.setTextureRect(sf::IntRect(
            columnaCalculada * anchoFrame, 
            filaCalculada * altoFrame, 
            anchoFrame, 
            altoFrame
        ));
        
        // Espejado fluido de dirección
        if (velocidad.x > 0) spriteCombate.setScale(2.2f, 2.2f);
        else if (velocidad.x < 0) spriteCombate.setScale(-2.2f, 2.2f);
    }

    cuerpoShape.setPosition(posicion);
}

void Personaje::dibujar(sf::RenderWindow& window) {
    if (!mapaTexturas.empty()) {
        spriteCombate.setPosition(posicion);
        window.draw(spriteCombate);
    } else {
        window.draw(cuerpoShape);
    }
}

float Personaje::getPosicionX() const { return posicion.x; }
float Personaje::getPosicionY() const { return posicion.y; }

void Personaje::corregirPosicionX(float deltaX) {
    posicion.x += deltaX;
    if (posicion.x < 40.f) posicion.x = 40.f;
    if (posicion.x > 1240.f) posicion.x = 1240.f;
    cuerpoShape.setPosition(posicion);
}

bool Personaje::getEstaAgachado() const { return estaAgachado; }
bool Personaje::getEstaAtacando() const { return estaAtacando; }
bool Personaje::getEsSuperAtaque() const { return esSuperAtaque; }
int Personaje::getTipoAtaque() const { return tipoAtaque; }
bool Personaje::getEstaRodando() const { return estaRodando; } 
bool Personaje::getEstaCorriendo() const { return estaCorriendo; }
bool Personaje::getEstaAturdido() const { return estaAturdido; }
float Personaje::getVida() const { return vida; }
std::string Personaje::getNombre() const { return nombre; }
std::string Personaje::getRutaAvatar() const { return rutaAvatar; }

bool Personaje::getEstaEnElAire() const { return estaEnElAire; }
bool Personaje::getEstaAtacandoAire() const { return estaAtacandoAire; }
int Personaje::getTipoAtaqueAire() const { return tipoAtaqueAire; }

void Personaje::recibirDanio(float cantidad) {
    if (estaRodando) return; 
    vida -= cantidad;
    if (vida < 0.f) vida = 0.f; 
}

void Personaje::curarVida(float cantidad) {
    vida += cantidad;
    if (vida > 250.f) vida = 250.f; 
}