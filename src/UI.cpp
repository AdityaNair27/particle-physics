#include "UI.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>
#include <string>

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

void Slider::update(sf::RenderWindow& window, float& variable, sf::Font generalFont){
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
    
    float calculatedValue = minValue + (maxValue - minValue) * percentage;

    float actualMin = std::min(minValue, maxValue);
    float actualMax = std::max(minValue, maxValue);
    
    variable = std::clamp(calculatedValue, actualMin, actualMax);
}

void Slider::update(sf::RenderWindow& window, int& variable, sf::Font generalFont){
    float tempFloat = static_cast<float>(variable);
    
    update(window, tempFloat, generalFont);
    
    variable = static_cast<int>(std::round(tempFloat));
}

void Slider::draw(sf::RenderWindow& window, std::string label, sf::Font generalFont){
    window.draw(track);
    window.draw(knob);

    sf::Text text(generalFont, label, 24);

    sf::FloatRect textBounds = text.getLocalBounds();

    text.setOrigin(text.getLocalBounds().getCenter());
    float what = static_cast<float>(track.getPosition().x + 0.5 * track.getSize().x);
    float why = static_cast<float>(track.getPosition().y - 30.0f);
    text.setPosition({what, why});
    window.draw(text);
}

Button::Button(sf::Vector2f position){
    value = false;

    button.setPosition(position);
    button.setSize({150, 50});
    button.setFillColor(sf::Color(255, 255, 255));
    button.setOutlineThickness(5);
    button.setOutlineColor(sf::Color(75, 75, 75));
};

void Button::update(sf::RenderWindow& window, bool& variable, sf::Font generalFont){
    sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        if(mousePosition.x > button.getPosition().x && mousePosition.x < button.getPosition().x + button.getSize().x){
            if(mousePosition.y > button.getPosition().y && mousePosition.y < button.getPosition().y + button.getSize().y){
                isClicking = true;
                value = !value;
            }
        }
    } else if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        isClicking = false;
    }

    variable = value;
};

void Button::draw(sf::RenderWindow& window, std::string label, sf::Font generalFont){
    window.draw(button);

    sf::Text text(generalFont, label, 24);

    sf::FloatRect textBounds = text.getLocalBounds();

    text.setOrigin(text.getLocalBounds().getCenter());
    float what = static_cast<float>(button.getPosition().x + 0.5 * button.getSize().x);
    float why = static_cast<float>(button.getPosition().y - 30.0f);
    text.setPosition({what, why});
    window.draw(text);
};

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