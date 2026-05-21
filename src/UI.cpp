#include "UI.h"
#include <SFML/Graphics.hpp>

Slider::Slider(sf::Vector2f position, float maxValue, float minValue){
    track.setSize({150.0, 4.0});
    track.setFillColor({255, 255, 255});
    track.setPosition({position.x, position.y});

    knob.setRadius(8.0);
    knob.setFillColor({100, 100, 100});
    knob.setOrigin({8.0, 8.0});
    knob.setPosition({position.x + 75, position.y + 2});
}

void Slider::update(sf::RenderWindow& window){}

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