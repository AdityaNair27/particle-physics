#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <cmath>

class Particle{
public:
    float size = 5;
    sf::Vector2f position, velocity;

    Particle(){
        position.x = size + static_cast<float>(rand() % static_cast<int>(799 - size*2));
        position.y = size + static_cast<float>(rand() % static_cast<int>(599 - size*2));
        velocity.x = -200 + static_cast<float>(rand() % static_cast<int>(400));
        velocity.y = -200 + static_cast<float>(rand() % static_cast<int>(400));
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

int main(){
    const int WIDTH = 600;
    const int LENGTH = 800;
    const int CELL_SIZE = 25;
    const float BOUNCE = 0.8f;
    const float FRICTION = 0.999f;

    std::vector<int> grid[LENGTH/CELL_SIZE][WIDTH/CELL_SIZE];

    srand(static_cast<unsigned> (time(NULL)));

    sf::RenderWindow window(sf::VideoMode({ LENGTH, WIDTH }), "Window");

    std::vector<Particle> particles;

    for(int i = 0; i < 500; i++){
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

        float dt = clock.restart().asSeconds();

        window.clear();

        for(int x = 0; x < LENGTH/CELL_SIZE; x++){
            for(int y = 0; y < WIDTH/CELL_SIZE; y++){
                grid[x][y].clear();
            }
        }
        
        for(Particle& p : particles){

            if(p.position.x < p.size){
                p.position.x = p.size;
                p.velocity.x *= -BOUNCE;

            } else if (p.position.x > (LENGTH - p.size)) {
                p.position.x = (LENGTH - p.size);
                p.velocity.x *= -BOUNCE;

            }

            if(p.position.y < p.size){
                p.position.y = p.size;
                p.velocity.y *= -BOUNCE;

            } else if (p.position.y > (WIDTH - p.size)) {
                p.position.y = (WIDTH - p.size);
                p.velocity.y *= -BOUNCE;

            }

            if(abs(p.velocity.x) < 0.1){
                p.velocity.x = 0;
            } else if(abs(p.velocity.y) < 0.1){
                p.velocity.y = 0;
            }

            p.velocity *= FRICTION;
            p.position += p.velocity * dt;

            dot.setRadius(p.size);
            dot.setOrigin({p.size, p.size});
            dot.setPosition(p.position);
            window.draw(dot);
        }
        
        for(int i = 0; i < particles.size(); i++){
            int gx = static_cast<int>(particles[i].position.x / CELL_SIZE);
            int gy = static_cast<int>(particles[i].position.y / CELL_SIZE);
        
            gx = std::clamp(gx, 0, (LENGTH / CELL_SIZE) - 1);
            gy = std::clamp(gy, 0, (WIDTH / CELL_SIZE) - 1);

            grid[gx][gy].push_back(i);
        }

        for (int x = 0; x < LENGTH / CELL_SIZE; x++) {
            for (int y = 0; y < WIDTH / CELL_SIZE; y++) {
                for(int p1 : grid[x][y]){
                    for(int nx = x - 1; nx <= x + 1; nx++){
                        for(int ny = y - 1; ny <= y + 1; ny++){
                            if (nx >= 0 && nx < LENGTH / CELL_SIZE && ny >= 0 && ny < WIDTH / CELL_SIZE) {
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

        window.display();
    }

    return 0;
}