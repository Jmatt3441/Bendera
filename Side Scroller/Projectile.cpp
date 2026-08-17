#include "Projectile.h"

#include <iostream>

// ============================================================
// STATIC VARIABLES
// ============================================================

sf::Texture Projectile::projectileTexture;

bool Projectile::textureLoaded = false;

// ============================================================
// LOAD PROJECTILE TEXTURE
// ============================================================

void Projectile::loadTexture()
{
    if (textureLoaded)
    {
        return;
    }

    if (!projectileTexture.loadFromFile(
        "assets/effects_powerups/effects_powerups_01.png"))
    {
        std::cout
            << "ERROR: Could not load projectile texture.\n";

        return;
    }

    textureLoaded = true;
}

// ============================================================
// CONSTRUCTOR
// ============================================================

Projectile::Projectile(
    float x,
    float y,
    int directionValue
)
    : projectileSprite(projectileTexture)
{
    loadTexture();

    direction = directionValue;

    // ========================================================
    // PROJECTILE HITBOX
    // ========================================================

    hitbox.setSize({
        40.0f,
        28.0f
        });

    hitbox.setPosition({
        x,
        y
        });

    // Invisible collision box
    hitbox.setFillColor(
        sf::Color(
            0,
            0,
            0,
            0
        )
    );

    // ========================================================
    // PROJECTILE IMAGE
    // ========================================================

    if (textureLoaded)
    {
        projectileSprite.setTexture(
            projectileTexture,
            true
        );
    }

    projectileSprite.setScale({
        0.40f,
        0.40f
        });

    // ========================================================
    // FLIP WHEN SHOOTING LEFT
    // ========================================================

    if (direction < 0)
    {
        sf::FloatRect bounds =
            projectileSprite.getLocalBounds();

        projectileSprite.setOrigin({
            bounds.size.x,
            0.0f
            });

        projectileSprite.setScale({
            -0.40f,
            0.40f
            });
    }

    updateSpritePosition();
}

// ============================================================
// UPDATE
// ============================================================

void Projectile::update(
    float deltaTime)
{
    if (!active)
    {
        return;
    }

    hitbox.move({
        speed *
        static_cast<float>(direction) *
        deltaTime,

        0.0f
        });

    updateSpritePosition();

    // Remove projectile once it leaves the world.
    if (
        hitbox.getPosition().x < -200.0f ||
        hitbox.getPosition().x > 10000.0f)
    {
        active = false;
    }
}

// ============================================================
// UPDATE VISIBLE SPRITE POSITION
// ============================================================

void Projectile::updateSpritePosition()
{
    if (!textureLoaded)
    {
        return;
    }

    sf::FloatRect spriteBounds =
        projectileSprite.getGlobalBounds();

    float spriteX =
        hitbox.getPosition().x +
        hitbox.getSize().x / 2.0f -
        spriteBounds.size.x / 2.0f;

    float spriteY =
        hitbox.getPosition().y +
        hitbox.getSize().y / 2.0f -
        spriteBounds.size.y / 2.0f;

    projectileSprite.setPosition({
        spriteX,
        spriteY
        });
}

// ============================================================
// DRAW
// ============================================================

void Projectile::draw(
    sf::RenderWindow& window)
{
    if (
        active &&
        textureLoaded)
    {
        window.draw(
            projectileSprite
        );
    }
}

// ============================================================
// COLLISION BOUNDS
// ============================================================

sf::FloatRect Projectile::getBounds() const
{
    return hitbox.getGlobalBounds();
}

// ============================================================
// IS PROJECTILE ACTIVE?
// ============================================================

bool Projectile::isActive() const
{
    return active;
}

// ============================================================
// DESTROY PROJECTILE
// ============================================================

void Projectile::destroy()
{
    active = false;
}