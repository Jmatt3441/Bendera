#include "Player.h"

#include <iostream>
#include <string>
#include <utility>

    // ============================================================
    // CONSTRUCTOR
    // ============================================================

    Player::Player(float x, float y)
        : playerSprite(initialTexture)
    {
        shape.setSize({
            PLAYER_WIDTH,
            PLAYER_HEIGHT
            });

        shape.setPosition({
            x,
            y
            });

        // Invisible collision box
        shape.setFillColor(
            sf::Color(0, 0, 0, 0)
        );

        velocity = {
            0.0f,
            0.0f
        };

        loadAnimations();

        if (!shiavonIdleTextures.empty())
        {
            playerSprite.setTexture(
                shiavonIdleTextures[0],
                true
            );
        }

        playerSprite.setScale({
            0.50f,
            0.50f
            });

        updateSpritePosition();
    }

    // ============================================================
    // LOAD ANIMATIONS
    // ============================================================

    void Player::loadAnimations()
    {
        // Clear everything first.
        shiavonIdleTextures.clear();
        shiavonRunTextures.clear();
        shiavonJumpTextures.clear();
        shiavonFallTextures.clear();

        benderaIdleTextures.clear();
        benderaRunTextures.clear();
        benderaJumpTextures.clear();
        benderaFallTextures.clear();

        // ========================================================
        // SHIAVON IDLE
        // 01 - 04
        // ========================================================

        for (int i = 1; i <= 4; ++i)
        {
            sf::Texture texture;

            std::string number =
                (i < 10 ? "0" : "") +
                std::to_string(i);

            std::string filename =
                "assets/shiavon/shiavon_" +
                number +
                ".png";

            if (!texture.loadFromFile(filename))
            {
                std::cout
                    << "Failed to load: "
                    << filename
                    << "\n";
            }

            shiavonIdleTextures.push_back(
                std::move(texture)
            );
        }

        // ========================================================
        // SHIAVON RUN
        // 05 - 08
        // ========================================================

        for (int i = 5; i <= 8; ++i)
        {
            sf::Texture texture;

            std::string number =
                (i < 10 ? "0" : "") +
                std::to_string(i);

            std::string filename =
                "assets/shiavon/shiavon_" +
                number +
                ".png";

            if (!texture.loadFromFile(filename))
            {
                std::cout
                    << "Failed to load: "
                    << filename
                    << "\n";
            }

            shiavonRunTextures.push_back(
                std::move(texture)
            );
        }

        // ========================================================
        // SHIAVON JUMP
        // 09 - 12
        // ========================================================

        for (int i = 9; i <= 12; ++i)
        {
            sf::Texture texture;

            std::string number =
                (i < 10 ? "0" : "") +
                std::to_string(i);

            std::string filename =
                "assets/shiavon/shiavon_" +
                number +
                ".png";

            if (!texture.loadFromFile(filename))
            {
                std::cout
                    << "Failed to load: "
                    << filename
                    << "\n";
            }

            shiavonJumpTextures.push_back(
                std::move(texture)
            );
        }

        // ========================================================
        // SHIAVON FALL
        // 13 - 17
        // ========================================================

        for (int i = 13; i <= 17; ++i)
        {
            sf::Texture texture;

            std::string filename =
                "assets/shiavon/shiavon_" +
                std::to_string(i) +
                ".png";

            if (!texture.loadFromFile(filename))
            {
                std::cout
                    << "Failed to load: "
                    << filename
                    << "\n";
            }

            shiavonFallTextures.push_back(
                std::move(texture)
            );
        }

        // ========================================================
        // BENDERA
        // 01 - 05
        // ========================================================

        for (int i = 1; i <= 5; ++i)
        {
            sf::Texture texture;

            std::string number =
                (i < 10 ? "0" : "") +
                std::to_string(i);

            std::string filename =
                "assets/bendera/bendera_" +
                number +
                ".png";

            if (!texture.loadFromFile(filename))
            {
                std::cout
                    << "Failed to load Bendera: "
                    << filename
                    << "\n";
            }

            benderaIdleTextures.push_back(
                std::move(texture)
            );
        }

        // For now Bendera uses the same five frames
        // for each animation state.
        benderaRunTextures =
            benderaIdleTextures;

        benderaJumpTextures =
            benderaIdleTextures;

        benderaFallTextures =
            benderaIdleTextures;
    }

    // ============================================================
    // UPDATE
    // ============================================================

    void Player::update(float deltaTime)
    {
        velocity.x = 0.0f;

        // ========================================================
        // BENDERA TIMER
        // ========================================================

        if (
            invincible &&
            invincibilityClock
            .getElapsedTime()
            .asSeconds() >=
            invincibilityDuration)
        {
            invincible = false;

            transformToShiavon();
        }

        // ========================================================
        // SPEED TIMER
        // ========================================================

        if (
            speedBoost &&
            speedClock
            .getElapsedTime()
            .asSeconds() >=
            speedDuration)
        {
            speedBoost = false;
        }

        // ========================================================
        // SUPER JUMP TIMER
        // ========================================================

        if (
            superJump &&
            jumpClock
            .getElapsedTime()
            .asSeconds() >=
            jumpDuration)
        {
            superJump = false;
        }

        // ========================================================
        // MOVEMENT SPEED
        // ========================================================

        float currentSpeed =
            PLAYER_SPEED;

        if (speedBoost)
        {
            currentSpeed *= 1.75f;
        }

        if (
            currentForm ==
            PlayerForm::Bendera)
        {
            currentSpeed *= 1.15f;
        }

        // ========================================================
        // LEFT
        // ========================================================

        if (
            sf::Keyboard::isKeyPressed(
                sf::Keyboard::Key::Left))
        {
            velocity.x =
                -currentSpeed;

            facingRight = false;
        }

        // ========================================================
        // RIGHT
        // ========================================================

        if (
            sf::Keyboard::isKeyPressed(
                sf::Keyboard::Key::Right))
        {
            velocity.x =
                currentSpeed;

            facingRight = true;
        }

        // ========================================================
        // GRAVITY
        // ========================================================

        velocity.y +=
            GRAVITY *
            deltaTime;

        // ========================================================
        // MOVE HITBOX
        // ========================================================

        shape.move({
            velocity.x * deltaTime,
            velocity.y * deltaTime
            });

        // ========================================================
        // WORLD LEFT BOUNDARY
        // ========================================================

        if (
            shape.getPosition().x <
            0.0f)
        {
            shape.setPosition({
                0.0f,
                shape.getPosition().y
                });
        }

        // ========================================================
        // WORLD RIGHT BOUNDARY
        // ========================================================

        if (
            shape.getPosition().x +
            PLAYER_WIDTH >
            WORLD_WIDTH)
        {
            shape.setPosition({
                WORLD_WIDTH -
                PLAYER_WIDTH,

                shape.getPosition().y
                });
        }

        // ========================================================
        // CHOOSE ANIMATION
        // ========================================================

        if (!onGround)
        {
            if (velocity.y < 0.0f)
            {
                setAnimation(
                    AnimationState::Jumping
                );
            }
            else
            {
                setAnimation(
                    AnimationState::Falling
                );
            }
        }
        else if (velocity.x != 0.0f)
        {
            setAnimation(
                AnimationState::Running
            );
        }
        else
        {
            setAnimation(
                AnimationState::Idle
            );
        }

        updateAnimation(deltaTime);

        updateSpritePosition();
    }

    // ============================================================
    // GET CURRENT ANIMATION TEXTURES
    // ============================================================

    std::vector<sf::Texture>*
        Player::getCurrentAnimationTextures()
    {
        if (
            currentForm ==
            PlayerForm::Shiavon)
        {
            switch (currentAnimation)
            {
            case AnimationState::Idle:
                return &shiavonIdleTextures;

            case AnimationState::Running:
                return &shiavonRunTextures;

            case AnimationState::Jumping:
                return &shiavonJumpTextures;

            case AnimationState::Falling:
                return &shiavonFallTextures;
            }
        }

        switch (currentAnimation)
        {
        case AnimationState::Idle:
            return &benderaIdleTextures;

        case AnimationState::Running:
            return &benderaRunTextures;

        case AnimationState::Jumping:
            return &benderaJumpTextures;

        case AnimationState::Falling:
            return &benderaFallTextures;
        }

        return nullptr;
    }

    // ============================================================
    // SET ANIMATION
    // ============================================================

    void Player::setAnimation(
        AnimationState newAnimation)
    {
        if (
            currentAnimation ==
            newAnimation)
        {
            return;
        }

        currentAnimation =
            newAnimation;

        currentFrame = 0;

        animationTimer = 0.0f;
    }

    // ============================================================
    // UPDATE ANIMATION
    // ============================================================

    void Player::updateAnimation(
        float deltaTime)
    {
        std::vector<sf::Texture>* textures =
            getCurrentAnimationTextures();

        if (
            textures == nullptr ||
            textures->empty())
        {
            return;
        }

        float frameTime = 0.12f;

        if (
            currentAnimation ==
            AnimationState::Idle)
        {
            frameTime = 0.25f;
        }
        else if (
            currentAnimation ==
            AnimationState::Running)
        {
            frameTime = 0.10f;
        }

        animationTimer +=
            deltaTime;

        if (
            animationTimer >=
            frameTime)
        {
            animationTimer = 0.0f;

            ++currentFrame;

            if (
                currentFrame >=
                static_cast<int>(
                    textures->size()))
            {
                currentFrame = 0;
            }
        }

        playerSprite.setTexture(
            (*textures)[currentFrame],
            true
        );

        // ========================================================
        // FACE RIGHT / LEFT
        // ========================================================

        sf::FloatRect bounds =
            playerSprite.getLocalBounds();

        if (facingRight)
        {
            playerSprite.setOrigin({
                0.0f,
                0.0f
                });

            playerSprite.setScale({
                0.50f,
                0.50f
                });
        }
        else
        {
            playerSprite.setOrigin({
                bounds.size.x,
                0.0f
                });

            playerSprite.setScale({
                -0.50f,
                0.50f
                });
        }
    }

    // ============================================================
    // TRANSFORM TO BENDERA
    // ============================================================

    void Player::transformToBendera()
    {
        currentForm =
            PlayerForm::Bendera;

        currentFrame = 0;

        animationTimer = 0.0f;

        std::vector<sf::Texture>* textures =
            getCurrentAnimationTextures();

        if (
            textures != nullptr &&
            !textures->empty())
        {
            playerSprite.setTexture(
                (*textures)[0],
                true
            );
        }

        updateSpritePosition();
    }

    // ============================================================
    // TRANSFORM BACK TO SHIAVON
    // ============================================================

    void Player::transformToShiavon()
    {
        currentForm =
            PlayerForm::Shiavon;

        currentFrame = 0;

        animationTimer = 0.0f;

        std::vector<sf::Texture>* textures =
            getCurrentAnimationTextures();

        if (
            textures != nullptr &&
            !textures->empty())
        {
            playerSprite.setTexture(
                (*textures)[0],
                true
            );
        }

        updateSpritePosition();
    }

    // ============================================================
    // UPDATE SPRITE POSITION
    // ============================================================

    void Player::updateSpritePosition()
    {
        sf::FloatRect spriteBounds =
            playerSprite.getGlobalBounds();

        float spriteX =
            shape.getPosition().x +
            PLAYER_WIDTH / 2.0f -
            spriteBounds.size.x / 2.0f;

        float spriteY =
            shape.getPosition().y +
            PLAYER_HEIGHT -
            spriteBounds.size.y;

        playerSprite.setPosition({
            spriteX,
            spriteY
            });
    }

    // ============================================================
    // JUMP
    // ============================================================

    void Player::jump()
    {
        if (!onGround)
        {
            return;
        }

        float currentJumpSpeed =
            JUMP_SPEED;

        if (superJump)
        {
            currentJumpSpeed *= 1.45f;
        }

        if (
            currentForm ==
            PlayerForm::Bendera)
        {
            currentJumpSpeed *= 1.15f;
        }

        velocity.y =
            -currentJumpSpeed;

        onGround = false;

        setAnimation(
            AnimationState::Jumping
        );
    }

    // ============================================================
    // SET ON GROUND
    // ============================================================

    void Player::setOnGround(
        bool value)
    {
        onGround = value;

        if (onGround)
        {
            velocity.y = 0.0f;
        }
    }

    // ============================================================
    // DRAW
    // ============================================================

    void Player::draw(
        sf::RenderWindow& window)
    {
        window.draw(
            playerSprite
        );
    }

    // ============================================================
    // GET COLLISION BOUNDS
    // ============================================================

    sf::FloatRect Player::getBounds() const
    {
        return shape.getGlobalBounds();
    }

    // ============================================================
    // GET POSITION
    // ============================================================

    sf::Vector2f Player::getPosition() const
    {
        return shape.getPosition();
    }

    // ============================================================
    // SET POSITION
    // ============================================================

    void Player::setPosition(
        float x,
        float y)
    {
        shape.setPosition({
            x,
            y
            });

        updateSpritePosition();
    }

    // ============================================================
    // ACTIVATE BENDERA / INVINCIBILITY
    // ============================================================

    void Player::activateInvincibility()
    {
        invincible = true;

        invincibilityClock.restart();

        transformToBendera();
    }

    // ============================================================
    // IS INVINCIBLE
    // ============================================================

    bool Player::isInvincible() const
    {
        return invincible;
    }

    // ============================================================
    // IS BENDERA
    // ============================================================

    bool Player::isBendera() const
    {
        return
            currentForm ==
            PlayerForm::Bendera;
    }

    // ============================================================
    // GET FACING DIRECTION
    //
    //  1 = RIGHT
    // -1 = LEFT
    // ============================================================

    int Player::getFacingDirection() const
    {
        return facingRight ? 1 : -1;
    }

    // Projectile Position
    sf::Vector2f Player::getProjectileSpawnPosition() const
    {
        return sf::Vector2f(
            shape.getPosition().x + PLAYER_WIDTH / 2.f,
            shape.getPosition().y  - 20.0f
        );
    }

    // ============================================================
    // ACTIVATE SPEED BOOST
    // ============================================================

    void Player::activateSpeedBoost()
    {
        speedBoost = true;

        speedClock.restart();
    }

    // ============================================================
    // HAS SPEED BOOST
    // ============================================================

    bool Player::hasSpeedBoost() const
    {
        return speedBoost;
    }

    // ============================================================
    // ACTIVATE SUPER JUMP
    // ============================================================

    void Player::activateSuperJump()
    {
        superJump = true;

        jumpClock.restart();
    }

    // ============================================================
    // HAS SUPER JUMP
    // ============================================================

    bool Player::hasSuperJump() const
    {
        return superJump;
    }
