#pragma once

#include <SFML/Graphics.hpp>

class Platform
{
public:
	Platform(float x, float y, float width, float height);

	void draw(sf::RenderWindow& window);

	sf::FloatRect getBounds() const;

private:
	sf::RectangleShape shape;
};
