#include <SFML/Graphics.hpp>

int main(){
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Window");

    sf::CircleShape dot(10.f);
    dot.setFillColor(sf::Color::White);
    dot.setPosition({ 0, 0 });

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        window.clear();

        window.draw(dot);

        window.display();
    }

    return 0;
}