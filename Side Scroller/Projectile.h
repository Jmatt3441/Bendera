#pragma once

#include <SFML/Graphics.hpp>

class Projectile
{
public:
    Projectile(
        float x,
        float y,
        int direction
    );

    void update(float deltaTime);
    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;
    bool isActive() const;
    void destroy();

private:
    // Collision box (slightly taller for better visibility / easier hits)
    sf::RectangleShape hitbox;

    static sf::Texture projectileTexture;
    static bool textureLoaded;

    sf::Sprite projectileSprite;

    float speed = 700.0f;
    int direction = 1;
    bool active = true;

    static void loadTexture();
    void updateSpritePosition();
};