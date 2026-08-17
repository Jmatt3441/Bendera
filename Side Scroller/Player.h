#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "Constants.h"

class Player
{
public:
    Player(float x, float y);

    void update(float deltaTime);
    void draw(sf::RenderWindow& window);

    void jump();
    void setOnGround(bool value);

    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;

    void setPosition(float x, float y);

    // Power-ups
    void activateInvincibility();
    void activateSpeedBoost();
    void activateSuperJump();

    bool isInvincible() const;
    bool hasSpeedBoost() const;
    bool hasSuperJump() const;

    // Bendera / projectile support
    bool isBendera() const;
    int getFacingDirection() const;

    sf::Vector2f getProjectileSpawnPosition() const; 
private:
    // =========================
    // COLLISION / MOVEMENT
    // =========================

    sf::RectangleShape shape;
    sf::Vector2f velocity;

    bool onGround = false;

    // =========================
    // SFML 3 SPRITE SETUP
    // =========================

    sf::Texture initialTexture;

    // =========================
    // SHIAVON TEXTURES
    // =========================

    std::vector<sf::Texture> shiavonIdleTextures;
    std::vector<sf::Texture> shiavonRunTextures;
    std::vector<sf::Texture> shiavonJumpTextures;
    std::vector<sf::Texture> shiavonFallTextures;

    // =========================
    // BENDERA TEXTURES
    // =========================

    std::vector<sf::Texture> benderaIdleTextures;
    std::vector<sf::Texture> benderaRunTextures;
    std::vector<sf::Texture> benderaJumpTextures;
    std::vector<sf::Texture> benderaFallTextures;

    // Visible character
    sf::Sprite playerSprite;

    // =========================
    // PLAYER FORM
    // =========================

    enum class PlayerForm
    {
        Shiavon,
        Bendera
    };

    PlayerForm currentForm =
        PlayerForm::Shiavon;

    // =========================
    // ANIMATION
    // =========================

    enum class AnimationState
    {
        Idle,
        Running,
        Jumping,
        Falling
    };

    AnimationState currentAnimation =
        AnimationState::Idle;

    int currentFrame = 0;

    float animationTimer = 0.0f;

    bool facingRight = true;

    void loadAnimations();

    void updateAnimation(
        float deltaTime
    );

    void setAnimation(
        AnimationState newAnimation
    );

    void updateSpritePosition();

    std::vector<sf::Texture>*
        getCurrentAnimationTextures();

    // =========================
    // TRANSFORMATION
    // =========================

    void transformToBendera();
    void transformToShiavon();

    // =========================
    // INVINCIBILITY / BENDERA
    // =========================

    bool invincible = false;

    sf::Clock invincibilityClock;

    float invincibilityDuration =
        8.0f;

    // =========================
    // SPEED BOOST
    // =========================

    bool speedBoost = false;

    sf::Clock speedClock;

    float speedDuration =
        10.0f;

    // =========================
    // SUPER JUMP
    // =========================

    bool superJump = false;

    sf::Clock jumpClock;

    float jumpDuration =
        10.0f;
};