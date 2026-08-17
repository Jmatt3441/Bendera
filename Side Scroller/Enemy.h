#pragma once

#include <SFML/Graphics.hpp>

#include <vector>

class Enemy
{
public:
    Enemy(float x, float y);

    void update(float deltaTime);

    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;

    sf::Vector2f getPosition() const;

    bool isAlive() const;

    void destroy();

private:

    // =========================================
    // COLLISION BOX
    // =========================================

    sf::RectangleShape shape;

    // =========================================
    // ENEMY SPRITES
    // =========================================

    // SFML 3 requires a Sprite
    // to be constructed with a texture.
    sf::Texture initialTexture;

    std::vector<sf::Texture> walkTextures;

    sf::Sprite enemySprite;

    // =========================================
    // ANIMATION
    // =========================================

    int currentFrame = 0;

    float animationTimer = 0.0f;

    float frameTime = 0.12f;

    void loadAnimations();

    void updateAnimation(float deltaTime);

    void updateSpritePosition();

    // =========================================
    // MOVEMENT
    // =========================================

    float speed = 100.0f;

    float leftLimit = 0.0f;

    float rightLimit = 0.0f;

    int direction = 1;

    // =========================================
    // STATE
    // =========================================

    bool alive = true;
};