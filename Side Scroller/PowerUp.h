#pragma once

#include <SFML/Graphics.hpp>

enum class PowerUpType
{
    Invincibility,
    Speed,
    SuperJump,
    ExtraLife
};

class PowerUp
{
public:
    PowerUp(float x, float y, PowerUpType type);

    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;

    PowerUpType getType() const;

    bool isActive() const;

    void collect();

private:
    sf::CircleShape shape;

    PowerUpType type;

    bool active = true;
};