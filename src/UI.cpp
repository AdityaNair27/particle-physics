#include "UI.h"
#include <SFML/Graphics.hpp>

void renderSidebar(sf::RenderWindow& window, const int& WINDOW_WIDTH, const int& WINDOW_HEIGHT){
    sf::RectangleShape Sidebar;
    sf::Vector2f size = {200, 600}, position = {0, 0};

    Sidebar.setFillColor(sf::Color(89, 89, 89));
    Sidebar.setSize(size);
    Sidebar.setPosition(position);
    window.draw(Sidebar);
}