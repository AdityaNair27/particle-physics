#include "UI.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>

Slider::Slider(sf::Vector2f position, float maxvalue, float minvalue){
    maxValue = maxvalue;
    minValue = minvalue;

    track.setSize({150.0, 4.0});
    track.setFillColor({255, 255, 255});
    track.setPosition({position.x, position.y});

    knob.setRadius(8.0);
    knob.setFillColor({100, 100, 100});
    knob.setOrigin({8.0, 8.0});
    knob.setPosition({position.x + 75, position.y + 2});
}

void Slider::update(sf::RenderWindow& window, float& variable){
    sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    sf::Vector2f distance = mousePosition - knob.getPosition();

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        float squaredDistance = (distance.x * distance.x) + (distance.y * distance.y);
        float squaredRadius = knob.getRadius() * knob.getRadius();
        if(squaredDistance < squaredRadius){
            isDragging = true;
        }
    } else if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        isDragging = false;
    }

    if(isDragging){
        float minX = track.getPosition().x;
        float maxX = minX + track.getSize().x;
        float clampedX = std::clamp(mousePosition.x, minX, maxX);
        
        knob.setPosition({clampedX, knob.getPosition().y});
    }

    float percentage = (knob.getPosition().x - track.getPosition().x) / track.getSize().x;
    variable = std::clamp(minValue + (maxValue - minValue) * percentage, minValue, maxValue);
}

void Slider::update(sf::RenderWindow& window, int& variable){
    float tempFloat = static_cast<float>(variable);
    
    update(window, tempFloat);
    
    variable = static_cast<int>(std::round(tempFloat));
}

void Slider::draw(sf::RenderWindow& window){
    window.draw(track);
    window.draw(knob);
}

void renderSidebar(sf::RenderWindow& window, const int& WINDOW_WIDTH, const int& WINDOW_HEIGHT){
    sf::RectangleShape sidebar;
    sf::Vector2f size = {198, 600}, position = {0, 0};

    sidebar.setFillColor(sf::Color(45, 45, 45));
    sidebar.setSize(size);
    sidebar.setPosition(position);
    sidebar.setOutlineThickness(2);
    sidebar.setOutlineColor(sf::Color(75, 75, 75));
    window.draw(sidebar);
}