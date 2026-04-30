#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>

class Particle{
public:
    float size = 5.0f;
    sf::Vector2f position = {0, 0}, velocity = {static_cast<float>(300.0), static_cast<float>(300.0)}, acceleration = {0, 0};

    Particle(){
        position = { 1 + static_cast<float>(rand() % static_cast<int>(799 - size*2)), 1 + static_cast<float>(rand() % static_cast<int>(599 - size*2)) };
    }
};

int main(){
    srand(static_cast<unsigned> (time(NULL)));

    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Window");

    std::vector<Particle> particles;

    for(int i = 0; i < 100; i++){
        particles.emplace_back();
    }

    sf::CircleShape dot;
    dot.setFillColor(sf::Color::White);
    
    sf::Clock clock;

    while (window.isOpen()) {

        float dt = clock.restart().asSeconds();

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        
        for(auto& p : particles){

            if(p.position.x < 0){
                p.position.x = 0;
                p.velocity.x *= -1;
                p.acceleration.x *= -1;
    
                p.velocity.x += p.acceleration.x;
            } else if (p.position.x > (800 - p.size*2)) {
                p.position.x = (800 - p.size*2);
                p.velocity.x *= -1;
                p.acceleration.x *= -1;
    
                p.velocity.x += p.acceleration.x;
            }

            if(p.position.y < 0){
                p.position.y = 0;
                p.velocity.y *= -1;
                p.acceleration.y *= -1;
    
                p.velocity.y += p.acceleration.y;
            } else if (p.position.y > (600 - p.size*2)) {
                p.position.y = (600 - p.size*2);
                p.velocity.y *= -1;
                p.acceleration.y *= -1;
    
                p.velocity.y += p.acceleration.y;
            }

            p.position += p.velocity * dt;

            dot.setRadius(p.size);
            dot.setPosition(p.position);
            window.draw(dot);
        }

        window.display();
    }

    return 0;
}