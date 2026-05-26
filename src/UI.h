#pragma once
#include <SFML/Graphics.hpp>

class Slider {
private:
    sf::RectangleShape track;
    sf::CircleShape knob;

    float maxValue;
    float minValue;
    float currentValue;
    bool isDragging;
public:
    Slider(sf::Vector2f position, float maxValue, float minValue);
    void update(sf::RenderWindow& window, float& variable);
    void update(sf::RenderWindow& window, int& variable);
    void draw(sf::RenderWindow& window);
};

void renderSidebar(sf::RenderWindow& window, const int& SCREEN_WIDTH, const int& SCREEN_HEIGHT);