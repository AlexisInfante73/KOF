#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    // 1. Crear la ventana del juego (Resolución de 800x600)
    sf::RenderWindow window(sf::VideoMode(800, 600), "Duelo de Bandas - KOF x OsuMania");
    window.setFramerateLimit(60); // Limitar a 60 FPS

    // 2. Crear el escenario (el lugar donde están parados)
    // Un rectángulo de 800 de ancho por 100 de alto
    sf::RectangleShape suelo(sf::Vector2f(800.f, 100.f));
    suelo.setFillColor(sf::Color(50, 50, 50)); // Color gris oscuro
    suelo.setPosition(0.f, 500.f);             // Posicionado abajo en la pantalla

    // 3. Crear los personajes (las dos bolitas estáticas)
    float radioPersonaje = 40.f;

    // Banda Alianza (Izquierda) - Una bolita Azul
    sf::CircleShape liderIzquierdo(radioPersonaje);
    liderIzquierdo.setFillColor(sf::Color::Blue);
    // Posición: X=150, Y=500(suelo) - 80(altura de la bolita)
    liderIzquierdo.setPosition(150.f, 420.f); 

    // Banda Rival (Derecha) - Una bolita Roja
    sf::CircleShape liderDerecho(radioPersonaje);
    liderDerecho.setFillColor(sf::Color::Red);
    // Posición: X=610 (800 - 150 - 40), Y=420
    liderDerecho.setPosition(610.f, 420.f);

    // 4. Bucle principal del juego
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Detectar si se cierra la ventana
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // --- RENDERIZADO (Dibujo) ---
        window.clear(sf::Color(30, 30, 30)); // Fondo casi negro

        // Dibujamos el suelo
        window.draw(suelo);

        // Dibujamos los dos líderes (las bolitas)
        window.draw(liderIzquierdo);
        window.draw(liderDerecho);

        // Mostrar todo en pantalla
        window.display();
    }

    return 0;
}