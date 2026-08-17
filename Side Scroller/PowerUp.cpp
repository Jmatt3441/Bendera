#include "PowerUp.h"

PowerUp::PowerUp(
    float x,
    float y,
    PowerUpType type)
    : type(type)
{
    // Extra life is slightly larger.
    if (type == PowerUpType::ExtraLife)
    {
        shape.setRadius(18.0f);
    }
    else
    {
        shape.setRadius(15.0f);
    }

    shape.setPosition({
        x,
        y
        });

    // =========================
    // POWER-UP COLORS
    // =========================

    switch (type)
    {
    case PowerUpType::Invincibility:

        // Gold
        shape.setFillColor(
            sf::Color(255, 215, 0)
        );

        break;

    case PowerUpType::Speed:

        // Blue
        shape.setFillColor(
            sf::Color(0, 100, 255)
        );

        break;

    case PowerUpType::SuperJump:

        // Green
        shape.setFillColor(
            sf::Color(0, 255, 100)
        );

        break;

    case PowerUpType::ExtraLife:

        // Red for now.
        // We'll replace this circle with
        // a heart sprite later.
        shape.setFillColor(
            sf::Color::Red
        );

        break;
    }

    shape.setOutlineThickness(3.0f);

    shape.setOutlineColor(
        sf::Color::White
    );
}

void PowerUp::draw(sf::RenderWindow& window)
{
    if (active)
    {
        window.draw(shape);
    }
}

sf::FloatRect PowerUp::getBounds() const
{
    return shape.getGlobalBounds();
}

PowerUpType PowerUp::getType() const
{
    return type;
}

bool PowerUp::isActive() const
{
    return active;
}

void PowerUp::collect()
{
    active = false;
}