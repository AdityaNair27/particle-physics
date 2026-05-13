#include "UI.h"
#include <SFML/Graphics.hpp>

void renderSidebar(sf::RenderWindow& window, const int& WINDOW_WIDTH, const int& WINDOW_HEIGHT){
    sf::RectangleShape sidebar;
    sf::Vector2f size = {198, 600}, position = {0, 0};

    sidebar.setFillColor(sf::Color(45, 45, 45));
    sidebar.setSize(size);
    sidebar.setPosition(position);
    sidebar.setOutlineThickness(2);
    sidebar.setOutlineColor(sf::Color(100, 100, 100));
    window.draw(sidebar);
}