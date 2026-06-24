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

namespace SimulationVariables {
    float friction = 0.999f;
    float size = 5;
    int numberOfParticles = 259;
    bool blackHole = false;
    float blackHoleStrength = 1000000.0f;
}

class Particle{
public:
    float size = SimulationVariables::size;
    sf::Vector2f position, velocity;

    Particle(){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> distX(200 + size, Config::WINDOW_WIDTH - size);
        std::uniform_real_distribution<float> distY(size, Config::WINDOW_HEIGHT - size);
        std::uniform_real_distribution<float> distV(-200.0, 200.0);

        position.x = distX(gen);
        position.y = distY(gen);
        velocity.x = distV(gen);
        velocity.y = distV(gen);
    }
};

void collision(Particle& p1, Particle& p2, float BOUNCE){
    float dx = p1.position.x - p2.position.x, dy = p1.position.y - p2.position.y;
    float minLength = p1.size + p2.size;
    float actualLength = sqrt((dx * dx) + (dy * dy));

    if(actualLength < minLength && actualLength > 0){
        float overlap = (minLength - actualLength) * 0.5f;
        float nx = dx / actualLength, ny = dy / actualLength;

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

void cursorCollision(Particle &p1, Particle &cursor, bool blackHole, float BOUNCE, float dt){
    if(blackHole == false){
        collision(p1, cursor, BOUNCE);
    } else {
        sf::Vector2f dir = cursor.position - p1.position;

        float distSq = dir.x * dir.x + dir.y * dir.y;
        distSq += 100;
        distSq = std::max(distSq, 25.0f);

        float accel = SimulationVariables::blackHoleStrength / distSq;

        float dist = std::sqrt(distSq);
        sf::Vector2f normalDir = dir / dist;

        p1.velocity += normalDir * accel * dt;
    }
}

void windowCollision(Particle &p, float BOUNCE){
    if(p.position.x < 200 + p.size){
        p.position.x = 200 + p.size;
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
    
    sf::RenderWindow window(sf::VideoMode({ Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT }), "Particle Physics Simulator");

    sf::Font font("arial.ttf");

    Particle cursor;
    cursor.size = 7.5f;

    sf::Vector2f previousCursorPosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    std::vector<int> grid[Config::WINDOW_WIDTH/Config::CELL_SIZE][Config::WINDOW_HEIGHT/Config::CELL_SIZE];

    std::vector<Particle> particles;

    for(int i = 0; i < SimulationVariables::numberOfParticles; i++){
        particles.emplace_back();
    }

    sf::CircleShape dot;
    dot.setFillColor(sf::Color::White);
    
    sf::Clock clock;

    Slider frictionSlider({25, 100}, 0.9f, 0.999f);
    Slider sizeSlider({25, 200}, 10, 1);
    Slider particleSlider({25, 300}, 500, 1);
    Slider bhStrengthSlider({25, 400}, 2000000, 1000000);
    Button blackHoleButton({25, 500});

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        if(SimulationVariables::numberOfParticles < particles.size()){
            for(int i = 0; i < particles.size() - SimulationVariables::numberOfParticles; i++){
                particles.pop_back();
            }
        } else if (SimulationVariables::numberOfParticles > particles.size()){
            for(int i = 0; i < SimulationVariables::numberOfParticles - particles.size(); i++){
                particles.emplace_back();
            }
        }

        window.clear();
        float dt = clock.restart().asSeconds();

        sf::Vector2f trueMousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        cursor.velocity = (trueMousePos - previousCursorPosition) / dt;

        if(!SimulationVariables::blackHole){
            sf::Vector2f mouseDelta = trueMousePos - previousCursorPosition;
            int substeps = 12;

            for (int step = 0; step < substeps; step++) {
                float fraction = static_cast<float>(step + 1) / static_cast<float>(substeps);
                sf::Vector2f subStepPos = previousCursorPosition + (mouseDelta * fraction);
                cursor.position = subStepPos;

                for(Particle& p : particles){
                    cursorCollision(p, cursor, SimulationVariables::blackHole, BOUNCE, dt);
                    
                    if (!SimulationVariables::blackHole) {
                        cursor.position = subStepPos;
                    }
                }
            }
        }

        previousCursorPosition = trueMousePos; 
        cursor.position = trueMousePos;

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
            if (SimulationVariables::blackHole) {
                cursorCollision(p, cursor, SimulationVariables::blackHole, BOUNCE, dt);
            }
            
            windowCollision(p, BOUNCE);

            p.velocity *= SimulationVariables::friction;
            p.position += p.velocity * dt;

            p.size = SimulationVariables::size;

            dot.setRadius(p.size);
            dot.setOrigin({p.size, p.size});
            dot.setPosition(p.position);
            window.draw(dot);
        }

        renderSidebar(window, Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT);

        frictionSlider.update(window, SimulationVariables::friction, font);
        frictionSlider.draw(window, "Friction", font);

        sizeSlider.update(window, SimulationVariables::size, font);
        sizeSlider.draw(window, "Size", font);

        particleSlider.update(window, SimulationVariables::numberOfParticles, font);
        particleSlider.draw(window, "Particles", font);

        bhStrengthSlider.update(window, SimulationVariables::blackHoleStrength, font);
        bhStrengthSlider.draw(window, "BH Strength", font);

        blackHoleButton.update(window, SimulationVariables::blackHole, font);
        blackHoleButton.draw(window, "Black Hole", font, SimulationVariables::blackHole);

        window.display();
    }

    return 0;
}