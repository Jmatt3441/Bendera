#include "Enemy.h"

#include <iostream>
#include <string>
#include <utility>

// ============================================================
// CONSTRUCTOR
// ============================================================

Enemy::Enemy(float x, float y)
    : enemySprite(initialTexture)
{
    // =========================================
    // INVISIBLE COLLISION BOX
    // =========================================

    shape.setSize({
        45.0f,
        90.0f
        });

    shape.setPosition({
        x,
        y - 50.0f
        });

    // Make the old red rectangle invisible.
    shape.setFillColor(
        sf::Color(
            0,
            0,
            0,
            0
        )
    );

    // Enemy patrol boundaries.
    leftLimit =
        x - 120.0f;

    rightLimit =
        x + 120.0f;

    // =========================================
    // LOAD ENEMY ART
    // =========================================

    loadAnimations();

    if (!walkTextures.empty())
    {
        enemySprite.setTexture(
            walkTextures[0],
            true
        );
    }

    // Start at half-size.
    // We can adjust this after seeing
    // the thug beside Shiavon.
    enemySprite.setScale({
        0.50f,
        0.50f
        });

    updateSpritePosition();
}

// ============================================================
// LOAD ANIMATION FRAMES
// ============================================================

void Enemy::loadAnimations()
{
    // We'll use enemy frames 01 - 06
    // for the first walking/patrol animation.

    for (int i = 1; i <= 6; i++)
    {
        sf::Texture texture;

        std::string number;

        if (i < 10)
        {
            number =
                "0" +
                std::to_string(i);
        }
        else
        {
            number =
                std::to_string(i);
        }

        std::string filename =
            std::string(
                "assets/enemies/enemies_"
            ) +
            number +
            ".png";

        if (!texture.loadFromFile(
            filename))
        {
            std::cout
                << "Failed to load enemy sprite: "
                << filename
                << "\n";
        }

        walkTextures.push_back(
            std::move(texture)
        );
    }
}

// ============================================================
// UPDATE
// ============================================================

void Enemy::update(float deltaTime)
{
    if (!alive)
    {
        return;
    }

    // =========================================
    // MOVE
    // =========================================

    shape.move({
        speed *
        static_cast<float>(direction) *
        deltaTime,

        0.0f
        });

    // =========================================
    // TURN AROUND AT LEFT LIMIT
    // =========================================

    if (
        shape.getPosition().x <=
        leftLimit)
    {
        direction = 1;
    }

    // =========================================
    // TURN AROUND AT RIGHT LIMIT
    // =========================================

    if (
        shape.getPosition().x >=
        rightLimit)
    {
        direction = -1;
    }

    // =========================================
    // ANIMATION
    // =========================================

    updateAnimation(
        deltaTime
    );

    // =========================================
    // KEEP SPRITE ON HITBOX
    // =========================================

    updateSpritePosition();
}

// ============================================================
// UPDATE ANIMATION
// ============================================================

void Enemy::updateAnimation(
    float deltaTime)
{
    if (walkTextures.empty())
    {
        return;
    }

    animationTimer +=
        deltaTime;

    if (
        animationTimer >=
        frameTime)
    {
        animationTimer =
            0.0f;

        currentFrame++;

        if (
            currentFrame >=
            static_cast<int>(
                walkTextures.size()))
        {
            currentFrame = 0;
        }
    }

    enemySprite.setTexture(
        walkTextures[
            currentFrame
        ],
        true
    );

    // =========================================
    // FLIP BASED ON DIRECTION
    // =========================================

    sf::FloatRect bounds =
        enemySprite
        .getLocalBounds();

    // Moving right
    if (direction == 1)
    {
        enemySprite.setOrigin({
            0.0f,
            0.0f
            });

        enemySprite.setScale({
            0.50f,
            0.50f
            });
    }

    // Moving left
    else
    {
        enemySprite.setOrigin({
            bounds.size.x,
            0.0f
            });

        enemySprite.setScale({
            -0.50f,
            0.50f
            });
    }
}

// ============================================================
// ALIGN SPRITE WITH HITBOX
// ============================================================

void Enemy::updateSpritePosition()
{
    sf::FloatRect spriteBounds =
        enemySprite
        .getGlobalBounds();

    float spriteX =
        shape.getPosition().x +
        shape.getSize().x /
        2.0f -
        spriteBounds.size.x /
        2.0f;

    float spriteY =
        shape.getPosition().y +
        shape.getSize().y -
        spriteBounds.size.y;

    enemySprite.setPosition({
        spriteX,
        spriteY
        });
}

// ============================================================
// DRAW
// ============================================================

void Enemy::draw(
    sf::RenderWindow& window)
{
    if (alive)
    {
        window.draw(
            enemySprite
        );

        // For collision debugging,
        // temporarily uncomment:
        //
        // window.draw(shape);
    }
}

// ============================================================
// COLLISION BOUNDS
// ============================================================

sf::FloatRect
Enemy::getBounds() const
{
    // Still use the invisible
    // rectangle for collisions.
    return shape
        .getGlobalBounds();
}

// ============================================================
// POSITION
// ============================================================

sf::Vector2f
Enemy::getPosition() const
{
    return shape
        .getPosition();
}

// ============================================================
// ALIVE STATE
// ============================================================

bool Enemy::isAlive() const
{
    return alive;
}

// ============================================================
// DESTROY ENEMY
// ============================================================

void Enemy::destroy()
{
    alive = false;
}