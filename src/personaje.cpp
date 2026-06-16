#include "personaje.h"
#include <cmath>
#include <iostream>
#include <cstdlib>

// --- CONSTANTES DEL JUEGO (Movidas aquí para evitar errores de macros) ---
const float ALTURA_SUELO = 520.f;
const float VEL_CAMINAR = 4.f;
const float VEL_SPRINT = 7.f;
const float VEL_RODADA = 9.f;
const float FUERZA_SALTO = -16.f;
const float GRAVEDAD = 0.8f;
const float DURACION_RODADA = 0.4f;
const float DURACION_ATAQUE = 0.3f;
const float COOLDOWN_ATAQUE_POST = 0.2f; // Tiempo de espera extra tras un ataque

Personaje::Personaje() {
    radioOriginal = 40.f; // Tamaño de tu bolita
    enElSuelo = true;
    estaAgachado = false;
    estaAtacando = false;
    estaRodando = false; 
    estaAturdido = false; 
    estaCongelado = false;
    direccionRodada = 0.f; 
    tiempoAturdimiento = 0.f;
    esSuperAtaque = false;
    tipoAtaque = 0;
    vida = 250.f; 
    comboStep = 0;
    nombre = "Anonimo";
    rutaAvatar = ""; // Inicializada por seguridad
    posicion = sf::Vector2f(0.f, 0.f);
    velocidad = sf::Vector2f(0.f, 0.f);
    
    estaEnElAire = false;
    estaAtacandoAire = false;
    tipoAtaqueAire = 0;
    
    presionoIzquierdaAntes = false;
    presionoDerechaAntes = false;
    tiempoLimiteDobleToque = 0.25f; 
    estaCorriendo = false;
}

// Inicializador adaptado
void Personaje::inicializar(std::string nombrePeleador, std::string rutaImg, sf::Color color, float xInicial) {
    nombre = nombrePeleador;
    rutaAvatar = rutaImg; // <-- Guardamos la ruta
    colorBase = color;
    
    // Configuración de la forma geométrica
    cuerpoShape.setRadius(radioOriginal);
    cuerpoShape.setOrigin(radioOriginal, radioOriginal); // Centrar el origen
    cuerpoShape.setFillColor(colorBase);
    cuerpoShape.setOutlineThickness(3.f); // Contorno para que resalte
    cuerpoShape.setOutlineColor(sf::Color::White);
    cuerpoShape.setScale(1.f, 1.f); 
    
    // Generar un combo aleatorio de 3 pasos para este personaje
    comboSecreto.clear();
    for(int i=0; i<3; i++) {
        // 1: Golpe Leve, 2: Golpe Fuerte, 3: Patada Leve, 4: Patada Fuerte
        comboSecreto.push_back((std::rand() % 4) + 1);
    }

    // Configuración de la sombra
    sombraShape.setRadius(radioOriginal);
    sombraShape.setOrigin(radioOriginal, radioOriginal);
    sombraShape.setFillColor(sf::Color(0, 0, 0, 100)); // Negro semitransparente
    sombraShape.setScale(1.0f, 0.3f); // Inicialmente un óvalo plano

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

    cuerpoShape.setPosition(posicion);
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
        
        // Efecto visual de rodar (la bolita se hace más pequeña)
        cuerpoShape.setScale(0.7f, 0.7f);
        return;
    }

    if (!estaAgachado && !estaAtacando) { 
        float velActual = (estaCorriendo || correr) ? VEL_SPRINT : VEL_CAMINAR;
        velocidad.x = direccion * velActual;
        
        // Voltear la figura según la dirección
        if (direccion > 0) {
            cuerpoShape.setScale(std::abs(cuerpoShape.getScale().x), cuerpoShape.getScale().y);
        } else if (direccion < 0) {
            cuerpoShape.setScale(-std::abs(cuerpoShape.getScale().x), cuerpoShape.getScale().y);
        }

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
        // Efecto visual: bolita pequeña
        cuerpoShape.setScale(0.7f, 0.7f);
    } else {
        // Vuelve a la normalidad (círculo)
        cuerpoShape.setScale(1.f, 1.f);
    }
}

bool Personaje::lanzarAtaque(int tipo) {
    bool puedeHacerCombo = false;
    if (estaAtacando) {
        float tiempoTranscurrido = relojAtaque.getElapsedTime().asSeconds();
        if (tiempoTranscurrido > DURACION_ATAQUE * 0.6f && comboStep < 3) {
            puedeHacerCombo = true;
        }
    }

    // Lógica Anti-Spam: 
    // Si no es un combo, verificamos que haya pasado la duración del ataque + el delay de recuperación
    if (!puedeHacerCombo && relojCooldownAtaque.getElapsedTime().asSeconds() < (DURACION_ATAQUE + COOLDOWN_ATAQUE_POST)) {
        return false;
    }

    if ((!estaAtacando || puedeHacerCombo) && !estaRodando && !estaAturdido && enElSuelo) {
        estaAtacando = true;
        tipoAtaque = tipo;
        comboStep = puedeHacerCombo ? comboStep + 1 : 1;
        relojAtaque.restart();
        velocidad.x = 0.f; 

        // Reiniciamos el cooldown global cada vez que se inicia un ataque (o parte de un combo)
        relojCooldownAtaque.restart();

        // Tinte de colores para indicar ataques
        if (tipo == 1) cuerpoShape.setFillColor(sf::Color::White);   // Golpe Leve
        if (tipo == 2) cuerpoShape.setFillColor(sf::Color::Yellow);  // Golpe Fuerte
        if (tipo == 3) cuerpoShape.setFillColor(sf::Color::Cyan);    // Patada Leve
        if (tipo == 4) cuerpoShape.setFillColor(sf::Color::Magenta); // Patada Fuerte
        if (tipo == 5) {
            esSuperAtaque = true;
            cuerpoShape.setFillColor(sf::Color::White);
            cuerpoShape.setScale(1.3f, 1.3f); // Se hace más grande
        }
        return true;
    }
    return false;
}

void Personaje::setCongelado(bool congelado) {
    estaCongelado = congelado;
    if (congelado) velocidad = {0.f, 0.f};
}

bool Personaje::puedeHacerCombo() const {
    if (!estaAtacando) return false;
    float tiempoTranscurrido = relojAtaque.getElapsedTime().asSeconds();
    return (tiempoTranscurrido > DURACION_ATAQUE * 0.6f && comboStep < 3);
}

void Personaje::lanzarAtaqueAereo(int tipo) {
    if (!estaAtacandoAire && !estaAturdido && !enElSuelo) {
        estaAtacandoAire = true;
        tipoAtaqueAire = tipo;
        relojAtaque.restart(); 
        
        cuerpoShape.setFillColor(sf::Color::White);
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

    // Visualmente aturdido (Tinte gris oscuro)
    cuerpoShape.setFillColor(sf::Color(100, 100, 100)); 
    cuerpoShape.setScale(1.f, 1.f);
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
    if (estaCongelado) return; // Si está congelado, no procesamos físicas ni tiempos

    // Restaurar aturdimiento
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

    // Restaurar rodada
    if (estaRodando) {
        if (relojRodada.getElapsedTime().asSeconds() >= DURACION_RODADA) {
            estaRodando = false;
            cuerpoShape.setScale(1.f, 1.f);
            cuerpoShape.setFillColor(colorBase); 
        } else {
            velocidad.x = direccionRodada * VEL_RODADA;
            // Transparencia al rodar usando setFillColor
            cuerpoShape.setFillColor(sf::Color(colorBase.r, colorBase.g, colorBase.b, 150));
        }
    }

    // Restaurar ataques
    if (estaAtacando && !estaAturdido) {
        if (relojAtaque.getElapsedTime().asSeconds() >= DURACION_ATAQUE) {
            estaAtacando = false;
            esSuperAtaque = false;
            comboStep = 0;
            tipoAtaque = 0;
            cuerpoShape.setFillColor(colorBase); 
            cuerpoShape.setScale(estaAgachado ? sf::Vector2f(0.7f, 0.7f) : sf::Vector2f(1.f, 1.f));
        }
    }

    if (estaAtacandoAire && !estaAturdido) {
        if (relojAtaque.getElapsedTime().asSeconds() >= DURACION_ATAQUE) {
            estaAtacandoAire = false;
            tipoAtaqueAire = 0;
            cuerpoShape.setFillColor(colorBase);
        }
    }

    // Físicas
    if (!enElSuelo) {
        velocidad.y += GRAVEDAD;
        estaEnElAire = true; 
    }

    posicion.x += velocidad.x;
    posicion.y += velocidad.y;
    
    if (!estaAturdido && !estaRodando && !estaCorriendo && !estaEnElAire) {
        velocidad.x = 0.f; 
    }

    // Colisión con el suelo
    if (posicion.y >= ALTURA_SUELO) {
        posicion.y = ALTURA_SUELO;
        if (velocidad.y > 0.f) velocidad.y = 0.f;
        enElSuelo = true;
        estaEnElAire = false; 
        estaAtacandoAire = false; 
    }

    // Límites de la pantalla (ajusta según la resolución de tu ventana)
    if (posicion.x < 40.f) posicion.x = 40.f;
    if (posicion.x > 1240.f) posicion.x = 1240.f;

    // Actualizamos la posición visual de las formas
    cuerpoShape.setPosition(posicion);
    sombraShape.setPosition(posicion.x, ALTURA_SUELO + radioOriginal - 5.f);
}

void Personaje::dibujar(sf::RenderWindow& window) {
    if (enElSuelo) {
        window.draw(sombraShape);
    }
    window.draw(cuerpoShape);
}

void Personaje::corregirPosicionX(float deltaX) {
    posicion.x += deltaX;
}

void Personaje::recibirDanio(float cantidad) {
    vida -= cantidad;
    if (vida < 0.f) vida = 0.f;
}

void Personaje::curarVida(float cantidad) {
    vida += cantidad;
    if (vida > 250.f) vida = 250.f;
}

float Personaje::getPosicionX() const { return posicion.x; }
float Personaje::getPosicionY() const { return posicion.y; }
bool Personaje::getEstaAgachado() const { return estaAgachado; }
bool Personaje::getEstaAtacando() const { return estaAtacando; }
bool Personaje::getEsSuperAtaque() const { return esSuperAtaque; }
int Personaje::getTipoAtaque() const { return tipoAtaque; }
bool Personaje::getEstaCongelado() const { return estaCongelado; }
bool Personaje::getEstaRodando() const { return estaRodando; }
bool Personaje::getEstaCorriendo() const { return estaCorriendo; }
bool Personaje::getEstaAturdido() const { return estaAturdido; }
float Personaje::getVida() const { return vida; }
sf::Color Personaje::getColorBase() const { return colorBase; }
std::string Personaje::getNombre() const { return nombre; }
std::string Personaje::getRutaAvatar() const { return rutaAvatar; }
bool Personaje::getEstaEnElAire() const { return estaEnElAire; }
bool Personaje::getEstaAtacandoAire() const { return estaAtacandoAire; }
int Personaje::getTipoAtaqueAire() const { return tipoAtaqueAire; }