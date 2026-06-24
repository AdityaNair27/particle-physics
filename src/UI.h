#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Slider {
private:
    sf::RectangleShape track;
    sf::CircleShape knob;

    float maxValue;
    float minValue;
    bool isDragging;
public:
    Slider(sf::Vector2f position, float maxValue, float minValue);
    void update(sf::RenderWindow& window, float& variable, sf::Font generalFont);
    void update(sf::RenderWindow& window, int& variable, sf::Font generalFont);
    void draw(sf::RenderWindow& window, std::string label, sf::Font generalFont);
};

class Button {
private:
    sf::RectangleShape button;

    bool isClicking;
public:
    Button(sf::Vector2f position);
    void update(sf::RenderWindow& window, bool& variable, sf::Font generalFont);
    void draw(sf::RenderWindow& window, std::string label, sf::Font generalFont, bool& blackHole);
};

void renderSidebar(sf::RenderWindow& window, const int& SCREEN_WIDTH, const int& SCREEN_HEIGHT);