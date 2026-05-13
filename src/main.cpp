#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <random>
#include "UI.h"

namespace Config {
    const int WINDOW_WIDTH = 900;
    const int WINDOW_HEIGHT = 600;
    const int CELL_SIZE = 25;
}

namespace SimulationVariable {
    float friction = 0.999f;
    float size = 5;
    int numberOfParticles = 500;
    bool blackHole = false;
}

class Particle{
public:
    float size = SimulationVariable::size;
    sf::Vector2f position, velocity;

    Particle(){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> distX(size, Config::WINDOW_WIDTH - size);
        std::uniform_real_distribution<float> distY(size, Config::WINDOW_HEIGHT - size);
        std::uniform_real_distribution<float> distV(-200.0, 200.0);

        position.x = distX(gen);
        position.y = distY(gen);
        velocity.x = distV(gen);
        velocity.y = distV(gen);
    }
};

void collision(Particle& p1, Particle& p2, float BOUNCE){
    float x = p1.position.x - p2.position.x, y = p1.position.y - p2.position.y;
    float minLength = p1.size + p2.size;
    float actualLength = sqrt((x * x) + (y * y));

    if(actualLength < minLength && actualLength > 0){
        float overlap = (minLength - actualLength) * 0.5f;
        float nx = x / actualLength, ny = y / actualLength;

        p1.position.x += nx * overlap;
        p1.position.y += ny * overlap;
        p2.position.x -= nx * overlap;
        p2.position.y -= ny * overlap;

        float vx = p1.velocity.x - p2.velocity.x;
        float vy = p1.velocity.y - p2.velocity.y;

        float dotProduct = (vx * nx) + (vy * ny);

        if (dotProduct < 0) {
            p1.velocity.x -= dotProduct * nx * BOUNCE;
            p1.velocity.y -= dotProduct * ny * BOUNCE;
            p2.velocity.x += dotProduct * nx * BOUNCE;
            p2.velocity.y += dotProduct * ny * BOUNCE;
        }
    }
}

void windowCollision(Particle &p, float BOUNCE){
    if(p.position.x < p.size){
        p.position.x = p.size;
        p.velocity.x *= -BOUNCE;

    } else if (p.position.x > (Config::WINDOW_WIDTH - p.size)) {
        p.position.x = (Config::WINDOW_WIDTH - p.size);
        p.velocity.x *= -BOUNCE;
    }

    if(p.position.y < p.size){
        p.position.y = p.size;
        p.velocity.y *= -BOUNCE;

    } else if (p.position.y > (Config::WINDOW_HEIGHT - p.size)) {
        p.position.y = (Config::WINDOW_HEIGHT - p.size);
        p.velocity.y *= -BOUNCE;

    }

    if(abs(p.velocity.x) < 0.1){
        p.velocity.x = 0;
    } 
    if(abs(p.velocity.y) < 0.1){
        p.velocity.y = 0;
    }
}

int main(){
    const float BOUNCE = 0.8f;
    
    sf::RenderWindow window(sf::VideoMode({ Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT }), "Window");

    Particle cursor;
    cursor.size = 7.5f;

    sf::Vector2f previousCursorPosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    std::vector<int> grid[Config::WINDOW_WIDTH/Config::CELL_SIZE][Config::WINDOW_HEIGHT/Config::CELL_SIZE];

    std::vector<Particle> particles;

    for(int i = 0; i < SimulationVariable::numberOfParticles; i++){
        particles.emplace_back();
    }

    sf::CircleShape dot;
    dot.setFillColor(sf::Color::White);
    
    sf::Clock clock;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        float dt = clock.restart().asSeconds();

        cursor.position = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        cursor.velocity = (cursor.position - previousCursorPosition) / dt;
        previousCursorPosition = cursor.position;

        for(int x = 0; x < Config::WINDOW_WIDTH/Config::CELL_SIZE; x++){
            for(int y = 0; y < Config::WINDOW_HEIGHT/Config::CELL_SIZE; y++){
                grid[x][y].clear();
            }
        }
        
        for(int i = 0; i < particles.size(); i++){
            int gx = static_cast<int>(particles[i].position.x / Config::CELL_SIZE);
            int gy = static_cast<int>(particles[i].position.y / Config::CELL_SIZE);
        
            gx = std::clamp(gx, 0, (Config::WINDOW_WIDTH / Config::CELL_SIZE) - 1);
            gy = std::clamp(gy, 0, (Config::WINDOW_HEIGHT / Config::CELL_SIZE) - 1);

            grid[gx][gy].push_back(i);
        }

        for (int x = 0; x < Config::WINDOW_WIDTH / Config::CELL_SIZE; x++) {
            for (int y = 0; y < Config::WINDOW_HEIGHT / Config::CELL_SIZE; y++) {
                for(int p1 : grid[x][y]){
                    for(int nx = x - 1; nx <= x + 1; nx++){
                        for(int ny = y - 1; ny <= y + 1; ny++){
                            if (nx >= 0 && nx < Config::WINDOW_WIDTH / Config::CELL_SIZE && ny >= 0 && ny < Config::WINDOW_HEIGHT / Config::CELL_SIZE) {
                                for(int p2 : grid[nx][ny]){
                                    if(p1 > p2){
                                        collision(particles[p1], particles[p2], BOUNCE);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        for(Particle& p : particles){
            collision(p, cursor, BOUNCE);
        }

        for(Particle& p : particles){
            windowCollision(p, BOUNCE);

            p.velocity *= SimulationVariable::friction;
            p.position += p.velocity * dt;

            dot.setRadius(p.size);
            dot.setOrigin({p.size, p.size});
            dot.setPosition(p.position);
            window.draw(dot);
        }

        renderSidebar(window, Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT);

        window.display();
    }

    return 0;
}