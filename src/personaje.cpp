#include "personaje.h"
#include <cmath>

Personaje::Personaje() {
    radioOriginal = 40.f;
    enElSuelo = true;
    estaAgachado = false;
    estaAtacando = false;
    estaRodando = false; 
    estaAturdido = false; // INICIO SIN MAREO
    direccionRodada = 0.f; 
    tiempoAturdimiento = 0.f;
    esSuperAtaque = false;
    tipoAtaque = 0;
    vida = 250.f; 
    nombre = "Anonimo";
    rutaAvatar = "";
    posicion = sf::Vector2f(0.f, 0.f);
    velocidad = sf::Vector2f(0.f, 0.f);
    
    // --- VARIABLES AÉREAS (NUEVO) ---
    estaEnElAire = false;
    estaAtacandoAire = false;
    tipoAtaqueAire = 0;
    
    presionoIzquierdaAntes = false;
    presionoDerechaAntes = false;
    tiempoLimiteDobleToque = 0.25f; 
    estaCorriendo = false;

    cuerpoShape.setRadius(radioOriginal);
    cuerpoShape.setOrigin(radioOriginal, radioOriginal * 2.f); 
}

void Personaje::inicializar(std::string nombrePeleador, std::string rutaImg, sf::Color color, float xInicial) {
    nombre = nombrePeleador;
    rutaAvatar = rutaImg;
    colorBase = color;
    cuerpoShape.setFillColor(colorBase);
    posicion = sf::Vector2f(xInicial, ALTURA_SUELO);
    velocidad = sf::Vector2f(0.f, 0.f);
    vida = 250.f; 
    estaAtacando = false;
    esSuperAtaque = false;
    estaAgachado = false;
    estaRodando = false; 
    estaCorriendo = false;
    estaAturdido = false; 
    
    // --- RESET AÉREO (NUEVO) ---
    estaEnElAire = false;
    estaAtacandoAire = false;
    tipoAtaqueAire = 0;
    
    presionoIzquierdaAntes = false;
    presionoDerechaAntes = false;

    cuerpoShape.setRadius(radioOriginal);
    cuerpoShape.setOrigin(radioOriginal, radioOriginal * 2.f);
    cuerpoShape.setPosition(posicion);
}

void Personaje::verificarDobleToque(sf::Event& evento, float xRival) {
    if (estaAturdido) return; // Si estas mareado, no puedes hacer doble toque

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
    // Si estás aturdido (en combo) o rodando, no te puedes mover
    if (estaAturdido || estaRodando) return; 

    if (estaAgachado && !estaAtacando && direccion != 0.f && enElSuelo) {
        estaRodando = true;
        direccionRodada = direccion;
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
        estaEnElAire = true; // (NUEVO)
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
    if (!estaAtacando && !estaRodando && !estaAturdido && enElSuelo) { 
        estaAtacando = true;
        tipoAtaque = tipo;
        relojAtaque.restart();
        velocidad.x = 0.f; 
        estaCorriendo = false; 

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

// --- LANZAR ATAQUE AÉREO (NUEVO) ---
void Personaje::lanzarAtaqueAereo(int tipo) {
    if (!estaAtacandoAire && !estaAturdido && !enElSuelo) {
        estaAtacandoAire = true;
        tipoAtaqueAire = tipo;
        relojAtaque.restart(); // Usamos el mismo reloj de ataque
        
        // Colores de ejemplo para ataques aéreos (puedes cambiarlos)
        if (tipo == 1) cuerpoShape.setFillColor(sf::Color(150, 255, 255)); 
        if (tipo == 2) cuerpoShape.setFillColor(sf::Color::Cyan);
    }
}

void Personaje::aplicarAturdimiento(float duracion) {
    if (estaRodando) return; // Si estabas rodando, esquivas el ataque (inmune)
    
    estaAturdido = true;
    tiempoAturdimiento = duracion;
    relojAturdimiento.restart();
    
    // Romper las acciones actuales
    estaAtacando = false;
    estaAtacandoAire = false; // (NUEVO) Romper ataque aéreo
    esSuperAtaque = false;
    estaCorriendo = false;
    tipoAtaque = 0;
    tipoAtaqueAire = 0; // (NUEVO)
    velocidad.x = 0.f; // Frenar en seco

    // Efecto visual: se pone gris/oscuro al recibir golpe
    cuerpoShape.setFillColor(sf::Color(150, 150, 150)); 
}

void Personaje::serLanzado(float fuerzaX, float fuerzaY) {
    if (estaRodando) return;
    
    // Al ser lanzado, recibe un aturdimiento largo (ej. 1 segundo para no poder hacer nada)
    aplicarAturdimiento(1.0f); 
    
    velocidad.x = fuerzaX;
    velocidad.y = fuerzaY; // Salta por los aires
    enElSuelo = false;
    estaEnElAire = true; // (NUEVO)
}

void Personaje::actualizar() {
    // 1. Manejar estado de Aturdimiento
    if (estaAturdido) {
        if (relojAturdimiento.getElapsedTime().asSeconds() >= tiempoAturdimiento) {
            estaAturdido = false;
            cuerpoShape.setFillColor(colorBase); // Recupera su color original
        }
        // Si está aturdido en el suelo, su fricción lo frena rápido (si fue lanzado)
        if (enElSuelo && abs(velocidad.x) > 0.f) {
            velocidad.x *= 0.8f; // Fricción
            if (abs(velocidad.x) < 0.5f) velocidad.x = 0.f;
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

    // Ataques terrestres
    if (estaAtacando && !estaAturdido) {
        if (relojAtaque.getElapsedTime().asSeconds() >= DURACION_ATAQUE) {
            estaAtacando = false;
            esSuperAtaque = false;
            tipoAtaque = 0;
            cuerpoShape.setFillColor(colorBase); 
        }
    }

    // Ataques aéreos (NUEVO)
    if (estaAtacandoAire && !estaAturdido) {
        if (relojAtaque.getElapsedTime().asSeconds() >= DURACION_ATAQUE) {
            estaAtacandoAire = false;
            tipoAtaqueAire = 0;
            cuerpoShape.setFillColor(colorBase);
        }
    }

    if (!enElSuelo) {
        velocidad.y += GRAVEDAD;
        estaEnElAire = true; // Asegurar estado aéreo si cae de una plataforma
    }

    if (estaCorriendo && !estaAtacando && !estaAgachado && enElSuelo && !estaRodando && !estaAturdido) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) velocidad.x = VEL_SPRINT;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) velocidad.x = -VEL_SPRINT;
    }

    posicion.x += velocidad.x;
    posicion.y += velocidad.y;
    
    if (!estaAturdido && !estaRodando) {
        velocidad.x = 0.f; // Solo reseteamos X si lo controlamos normal
    }

    if (posicion.y >= ALTURA_SUELO) {
        posicion.y = ALTURA_SUELO;
        if (velocidad.y > 0.f) velocidad.y = 0.f;
        enElSuelo = true;
        estaEnElAire = false; // (NUEVO) Resetea estado aéreo al tocar suelo
        estaAtacandoAire = false; // (NUEVO) Cancela el ataque aéreo si toca el suelo
    }

    if (posicion.x < 40.f) posicion.x = 40.f;
    if (posicion.x > 1240.f) posicion.x = 1240.f;

    cuerpoShape.setPosition(posicion);
}

void Personaje::dibujar(sf::RenderWindow& window) {
    window.draw(cuerpoShape);
}

float Personaje::getPosicionX() const { return posicion.x; }

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

// --- GETTERS AÉREOS (NUEVOS - Para CombateMusical) ---
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