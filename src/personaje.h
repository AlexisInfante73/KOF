#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <SFML/Graphics.hpp>
#include <string>
#include <map>

// --- CONSTANTES DEL JUEGO ---
#ifndef ALTURA_SUELO
#define ALTURA_SUELO 500.f
#endif
#ifndef VEL_CAMINAR
#define VEL_CAMINAR 5.f
#endif
#ifndef VEL_SPRINT
#define VEL_SPRINT 9.f
#endif
#ifndef VEL_RODADA
#define VEL_RODADA 7.f
#endif
#ifndef FUERZA_SALTO
#define FUERZA_SALTO -15.f
#endif
#ifndef GRAVEDAD
#define GRAVEDAD 0.5f
#endif
#ifndef DURACION_RODADA
#define DURACION_RODADA 0.5f
#endif
#ifndef DURACION_ATAQUE
#define DURACION_ATAQUE 0.3f
#endif

class Personaje {
private:
    // --- ATRIBUTOS DE CONFIGURACIÓN Y ESTADO ---
    float       radioOriginal;
    bool        enElSuelo;
    bool        estaAgachado;
    bool        estaAtacando;
    bool        estaRodando;
    float       direccionRodada;
    bool        esSuperAtaque;
    int         tipoAtaque;
    float       vida;
    int         comboStep; 
    std::string nombre;
    std::string rutaAvatar;
    
    // --- COMBATE AÉREO ---
    bool        estaEnElAire;
    bool        estaAtacandoAire;
    int         tipoAtaqueAire;
    
    // --- ATURDIMIENTO Y LANZAMIENTO ---
    bool        estaAturdido;
    float       tiempoAturdimiento;
    
    // --- FÍSICAS Y POSICIÓN ---
    sf::Vector2f posicion;
    sf::Vector2f velocidad;
    
    // --- VARIABLES PARA EL SPRINT (DOBLE TOQUE) ---
    bool        presionoIzquierdaAntes;
    bool        presionoDerechaAntes;
    float       tiempoLimiteDobleToque;
    bool        estaCorriendo;

    // --- ANIMACIÓN DE SPRITES (MIGRADO A HOJAS INDEPENDIENTES) ---
    int         anchoFrame;
    int         altoFrame;
    int         frameActualCol;
    int         maxFramesAccion; // Cuántos frames reales tiene la tira actual
    sf::Clock   relojAnimacion;
    
    // Almacenamiento dinámico de las hojas de textura extraídas de tu carpeta
    std::map<std::string, sf::Texture> mapaTexturas;
    std::string accionActual; // Guarda qué animación está corriendo ("caminar", "agacharse", etc.)

    // --- COMPONENTES GRÁFICOS Y RELOJES ---
    sf::CircleShape cuerpoShape;
    sf::Sprite       spriteCombate;  
    sf::Color        colorBase;
    sf::Clock        relojDobleToque;
    sf::Clock        relojRodada;
    sf::Clock        relojAtaque;
    sf::Clock        relojAturdimiento; 

    // Métodos internos de control de texturas
    void inyectarTextura(const std::string& claveAccion);

public:
    Personaje();
    
    // --- INICIALIZACIÓN Y LÓGICA PRINCIPAL ---
    void inicializar(std::string nombrePeleador, std::string rutaImg, sf::Color color, float xInicial);
    void verificarDobleToque(sf::Event& evento, float xRival);
    void caminar(float direccion, bool correr = false); 
    void saltar();
    void setAgachado(bool agachado);
    void lanzarAtaque(int tipo);
    void lanzarAtaqueAereo(int tipo);
    void actualizar();
    void dibujar(sf::RenderWindow& window);
    void corregirPosicionX(float deltaX);
    
    // --- SISTEMA DE DAÑO Y COMBOS ---
    void recibirDanio(float cantidad);
    void curarVida(float cantidad);
    void aplicarAturdimiento(float duracion); 
    void serLanzado(float fuerzaX, float fuerzaY); 

    // --- GETTERS ---
    float       getPosicionX() const;
    float       getPosicionY() const; 
    bool        getEstaAgachado() const;
    bool        getEstaAtacando() const;
    bool        getEsSuperAtaque() const;
    int         getTipoAtaque() const;
    bool        getEstaRodando() const;
    bool        getEstaCorriendo() const;
    bool        getEstaAturdido() const; 
    float       getVida() const;
    std::string getNombre() const;
    std::string getRutaAvatar() const;
    
    bool        getEstaEnElAire() const;
    bool        getEstaAtacandoAire() const;
    int         getTipoAtaqueAire() const;
};

#endif // PERSONAJE_H