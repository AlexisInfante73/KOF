#include "personaje.h"
#include <cmath>

Personaje::Personaje() {
    radioOriginal = 40.f;
    enElSuelo = true;
    estaAgachado = false;
    estaAtacando = false;
    esSuperAtaque = false;
    tipoAtaque = 0;
    vida = 250.f; // <-- MODIFICADO: Antes 128.f
    nombre = "Anonimo";
    rutaAvatar = "";
    posicion = sf::Vector2f(0.f, 0.f);
    velocidad = sf::Vector2f(0.f, 0.f);
    
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
    vida = 250.f; // <-- MODIFICADO: Más salud al iniciar la ronda
    estaAtacando = false;
    esSuperAtaque = false;
    estaAgachado = false;
    cuerpoShape.setRadius(radioOriginal);
    cuerpoShape.setOrigin(radioOriginal, radioOriginal * 2.f);
    cuerpoShape.setPosition(posicion);
}

void Personaje::caminar(float direccion) {
    if (!estaAgachado && !estaAtacando) { 
        velocidad.x = direccion * VEL_CAMINAR;
    } else {
        velocidad.x = 0.f;
    }
}

void Personaje::saltar() {
    if (enElSuelo && !estaAgachado && !estaAtacando) {
        velocidad.y = FUERZA_SALTO;
        enElSuelo = false;
    }
}

void Personaje::setAgachado(bool agachado) {
    if (estaAtacando) return; 

    estaAgachado = agachado;
    if (estaAgachado) {
        cuerpoShape.setRadius(radioOriginal / 2.f);
        cuerpoShape.setOrigin(radioOriginal / 2.f, radioOriginal);
    } else {
        cuerpoShape.setRadius(radioOriginal);
        cuerpoShape.setOrigin(radioOriginal, radioOriginal * 2.f);
    }
}

void Personaje::lanzarAtaque(int tipo) {
    if (!estaAtacando) { 
        estaAtacando = true;
        tipoAtaque = tipo;
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

void Personaje::actualizar() {
    if (estaAtacando) {
        if (relojAtaque.getElapsedTime().asSeconds() >= DURACION_ATAQUE) {
            estaAtacando = false;
            esSuperAtaque = false;
            tipoAtaque = 0;
            cuerpoShape.setFillColor(colorBase); 
        }
    }

    if (!enElSuelo) {
        velocidad.y += GRAVEDAD;
    }

    posicion.x += velocidad.x;
    posicion.y += velocidad.y;
    velocidad.x = 0.f;

    if (posicion.y >= ALTURA_SUELO) {
        posicion.y = ALTURA_SUELO;
        velocidad.y = 0.f;
        enElSuelo = true;
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
float Personaje::getVida() const { return vida; }
std::string Personaje::getNombre() const { return nombre; }
std::string Personaje::getRutaAvatar() const { return rutaAvatar; }

void Personaje::recibirDanio(float cantidad) {
    vida -= cantidad;
    if (vida < 0.f) vida = 0.f; 
}

void Personaje::curarVida(float cantidad) {
    vida += cantidad;
    if (vida > 250.f) vida = 250.f; // <-- MODIFICADO: Límite ajustado a 250.f
}