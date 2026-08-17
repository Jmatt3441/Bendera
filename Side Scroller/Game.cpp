#include "Game.h"

#include <optional>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

// ============================================================
// CONSTRUCTOR
// ============================================================

Game::Game()
    : window(
        sf::VideoMode({
            WINDOW_WIDTH,
            WINDOW_HEIGHT
            }),
        "2D Side Scroller"
    ),

    player(
        100.0f,
        300.0f
    ),

    camera(
        sf::FloatRect(
            {
                0.0f,
                0.0f
            },
            {
                static_cast<float>(WINDOW_WIDTH),
                static_cast<float>(WINDOW_HEIGHT)
            }
        )
    )
{
    window.setFramerateLimit(60);
    projectiles.reserve(50);

    // ========================================================
    // FONT
    // ========================================================

    if (!font.openFromFile("arial.ttf"))
    {
        std::cout
            << "Could not load arial.ttf\n";
    }

    // ========================================================
    // GROUND
    // ========================================================

    ground.setSize({
        WORLD_WIDTH,
        GROUND_HEIGHT
        });

    ground.setPosition({
        0.0f,
        WINDOW_HEIGHT - GROUND_HEIGHT
        });

    ground.setFillColor(
        sf::Color(
            100,
            70,
            40
        )
    );

    // ========================================================
    // GOAL
    // ========================================================

    goal.setSize({
        40.0f,
        150.0f
        });

    goal.setPosition({
        WORLD_WIDTH - 100.0f,

        WINDOW_HEIGHT -
        GROUND_HEIGHT -
        150.0f
        });

    goal.setFillColor(
        sf::Color::Magenta
    );

    // ========================================================
    // PLATFORMS
    // ========================================================

    platforms.emplace_back(
        500.0f,
        430.0f,
        200.0f,
        30.0f
    );

    platforms.emplace_back(
        850.0f,
        350.0f,
        180.0f,
        30.0f
    );

    platforms.emplace_back(
        1200.0f,
        270.0f,
        220.0f,
        30.0f
    );

    platforms.emplace_back(
        1600.0f,
        390.0f,
        250.0f,
        30.0f
    );

    platforms.emplace_back(
        2050.0f,
        300.0f,
        180.0f,
        30.0f
    );

    platforms.emplace_back(
        2450.0f,
        420.0f,
        260.0f,
        30.0f
    );

    platforms.emplace_back(
        2950.0f,
        320.0f,
        220.0f,
        30.0f
    );

    platforms.emplace_back(
        3400.0f,
        240.0f,
        250.0f,
        30.0f
    );

    // ========================================================
    // ENEMIES
    // ========================================================

    enemies.emplace_back(
        750.0f,
        WINDOW_HEIGHT -
        GROUND_HEIGHT -
        40.0f
    );

    enemies.emplace_back(
        1450.0f,
        WINDOW_HEIGHT -
        GROUND_HEIGHT -
        40.0f
    );

    enemies.emplace_back(
        2200.0f,
        WINDOW_HEIGHT -
        GROUND_HEIGHT -
        40.0f
    );

    enemies.emplace_back(
        3100.0f,
        WINDOW_HEIGHT -
        GROUND_HEIGHT -
        40.0f
    );

    // ========================================================
    // POWER UPS
    // ========================================================

    // Gold Orb
    // Turns Shiavon into Bendera
    powerUps.emplace_back(
        580.0f,
        390.0f,
        PowerUpType::Invincibility
    );

    // Blue Orb
    // Speed Boost
    powerUps.emplace_back(
        920.0f,
        310.0f,
        PowerUpType::Speed
    );

    // Green Orb
    // Super Jump
    powerUps.emplace_back(
        1280.0f,
        230.0f,
        PowerUpType::SuperJump
    );

    // Extra Life
    powerUps.emplace_back(
        1700.0f,
        350.0f,
        PowerUpType::ExtraLife
    );
}

// ============================================================
// MAIN GAME LOOP
// ============================================================

void Game::run()
{
    sf::Clock clock;

    while (window.isOpen())
    {
        float deltaTime =
            clock.restart().asSeconds();

        // Prevent a giant physics jump
        // if the game freezes temporarily.
        if (deltaTime > 0.05f)
        {
            deltaTime = 0.05f;
        }

        processEvents();

        if (
            gameState ==
            GameState::Playing)
        {
            update(deltaTime);

            handleCollisions();

            handlePowerUpCollisions();

            handleEnemyCollisions();

            handleProjectileCollisions();

            handleGoalCollision();

            updateCamera();
        }

        render();
    }
}

// ============================================================
// EVENTS / CONTROLS
// ============================================================

void Game::processEvents()
{
    while (
        const std::optional event =
        window.pollEvent())
    {
        // ====================================================
        // WINDOW CLOSE
        // ====================================================

        if (
            event->is<
            sf::Event::Closed>())
        {
            window.close();
        }

        // ====================================================
        // KEYBOARD INPUT
        // ====================================================

        if (
            const auto* keyPressed =
            event->getIf<
            sf::Event::KeyPressed>())
        {
            // =================================================
            // PLAYING
            // =================================================

            if (
                gameState ==
                GameState::Playing)
            {
                // ---------------------------------------------
                // SPACE = JUMP
                // ---------------------------------------------

                if (
                    keyPressed->code ==
                    sf::Keyboard::Key::Space)
                {
                    player.jump();
                }

                // ---------------------------------------------
               // X = BENDERA ENERGY ATTACK
               // ---------------------------------------------

                if (
                    keyPressed->code ==
                    sf::Keyboard::Key::X)
                {
                    // Only Bendera can shoot.
                    if (player.isBendera())
                    {
                        // Get the projectile spawn point
                        // directly from the Player class.
                        sf::Vector2f spawnPosition =
                            player.getProjectileSpawnPosition();

                        int direction =
                            player.getFacingDirection();

                        float projectileX =
                            spawnPosition.x;

                        float projectileY =
                            spawnPosition.y;

                        // Move the projectile slightly
                        // in front of Bendera.
                        if (direction > 0)
                        {
                            projectileX += 30.0f;
                        }
                        else
                        {
                            projectileX -= 30.0f;
                        }

                        projectiles.emplace_back(
                            projectileX,
                            projectileY,
                            direction
                        );
                    }
                }
                // ---------------------------------------------
                // ESC = QUIT
                // ---------------------------------------------

                if (
                    keyPressed->code ==
                    sf::Keyboard::Key::Escape)
                {
                    window.close();
                }
            }

            // =================================================
            // GAME OVER
            // =================================================

            else if (
                gameState ==
                GameState::GameOver)
            {
                // Enter restarts the game.
                if (
                    keyPressed->code ==
                    sf::Keyboard::Key::Enter)
                {
                    restartGame();
                }

                if (
                    keyPressed->code ==
                    sf::Keyboard::Key::Escape)
                {
                    window.close();
                }
            }

            // =================================================
            // LEVEL COMPLETE
            // =================================================

            else if (
                gameState ==
                GameState::LevelComplete)
            {
                if (
                    keyPressed->code ==
                    sf::Keyboard::Key::Enter)
                {
                    startNextLevel();
                }

                if (
                    keyPressed->code ==
                    sf::Keyboard::Key::Escape)
                {
                    window.close();
                }
            }
        }
    }
}

// ============================================================
// UPDATE
// ============================================================

void Game::update(
    float deltaTime)
{
    // Player
    player.update(
        deltaTime
    );

    // Enemies
    for (
        Enemy& enemy :
        enemies)
    {
        enemy.update(
            deltaTime
        );
    }

    // Bendera projectiles
    for (
        Projectile& projectile :
        projectiles)
    {
        projectile.update(
            deltaTime
        );
    }
}

// ============================================================
// GROUND / PLATFORM COLLISIONS
// ============================================================

void Game::handleCollisions()
{
    bool standingOnSomething =
        false;

    float groundTop =
        WINDOW_HEIGHT -
        GROUND_HEIGHT;

    float playerBottom =
        player
        .getPosition()
        .y +
        PLAYER_HEIGHT;

    // ========================================================
    // GROUND
    // ========================================================

    if (
        playerBottom >=
        groundTop)
    {
        player.setPosition(
            player
            .getPosition()
            .x,

            groundTop -
            PLAYER_HEIGHT
        );

        player.setOnGround(
            true
        );

        standingOnSomething =
            true;
    }

    // ========================================================
    // PLATFORMS
    // ========================================================

    for (
        const Platform& platform :
        platforms)
    {
        sf::FloatRect
            playerBounds =
            player.getBounds();

        sf::FloatRect
            platformBounds =
            platform.getBounds();

        auto intersection =
            playerBounds
            .findIntersection(
                platformBounds
            );

        if (intersection)
        {
            float platformTop =
                platformBounds
                .position
                .y;

            float playerBottomNow =
                player
                .getPosition()
                .y +
                PLAYER_HEIGHT;

            // Only treat this as a landing
            // if the player approaches from above.
            if (
                playerBottomNow <=
                platformTop +
                20.0f)
            {
                player.setPosition(
                    player
                    .getPosition()
                    .x,

                    platformTop -
                    PLAYER_HEIGHT
                );

                player.setOnGround(
                    true
                );

                standingOnSomething =
                    true;
            }
        }
    }

    if (!standingOnSomething)
    {
        player.setOnGround(
            false
        );
    }
}

// ============================================================
// POWER-UP COLLISIONS
// ============================================================

void Game::handlePowerUpCollisions()
{
    for (
        PowerUp& powerUp :
        powerUps)
    {
        if (!powerUp.isActive())
        {
            continue;
        }

        auto intersection =
            player
            .getBounds()
            .findIntersection(
                powerUp
                .getBounds()
            );

        if (!intersection)
        {
            continue;
        }

        PowerUpType type =
            powerUp.getType();

        powerUp.collect();

        // Every pickup awards points.
        score += 250;

        // ====================================================
        // GOLD ORB
        // SHIAVON -> BENDERA
        // ====================================================

        if (
            type ==
            PowerUpType::Invincibility)
        {
            player
                .activateInvincibility();
        }

        // ====================================================
        // BLUE ORB
        // SPEED BOOST
        // ====================================================

        else if (
            type ==
            PowerUpType::Speed)
        {
            player
                .activateSpeedBoost();
        }

        // ====================================================
        // GREEN ORB
        // SUPER JUMP
        // ====================================================

        else if (
            type ==
            PowerUpType::SuperJump)
        {
            player
                .activateSuperJump();
        }

        // ====================================================
        // EXTRA LIFE
        // ====================================================

        else if (
            type ==
            PowerUpType::ExtraLife)
        {
            lives++;
        }
    }
}

// ============================================================
// PLAYER / ENEMY COLLISIONS
// ============================================================

void Game::handleEnemyCollisions()
{
    for (
        Enemy& enemy :
        enemies)
    {
        if (!enemy.isAlive())
        {
            continue;
        }

        auto intersection =
            player
            .getBounds()
            .findIntersection(
                enemy
                .getBounds()
            );

        if (!intersection)
        {
            continue;
        }

        // ====================================================
        // BENDERA / INVINCIBILITY
        // ====================================================

        if (player.isInvincible())
        {
            enemy.destroy();

            score += 100;

            continue;
        }

        float playerBottom =
            player
            .getPosition()
            .y +
            PLAYER_HEIGHT;

        float enemyTop =
            enemy
            .getPosition()
            .y;

        // ====================================================
        // STOMP ENEMY
        // ====================================================

        if (
            playerBottom <=
            enemyTop +
            25.0f)
        {
            enemy.destroy();

            score += 100;

            player.setPosition(
                player
                .getPosition()
                .x,

                enemyTop -
                PLAYER_HEIGHT
            );

            player.setOnGround(
                true
            );
        }

        // ====================================================
        // PLAYER GETS HIT
        // ====================================================

        else
        {
            lives--;

            if (lives <= 0)
            {
                gameState =
                    GameState::GameOver;

                return;
            }

            resetPlayer();

            break;
        }
    }
}

// ============================================================
// PROJECTILE / ENEMY COLLISIONS
// ============================================================

void Game::handleProjectileCollisions()
{
    for (
        Projectile& projectile :
        projectiles)
    {
        if (
            !projectile
            .isActive())
        {
            continue;
        }

        for (
            Enemy& enemy :
            enemies)
        {
            if (!enemy.isAlive())
            {
                continue;
            }

            auto intersection =
                projectile
                .getBounds()
                .findIntersection(
                    enemy
                    .getBounds()
                );

            if (intersection)
            {
                // Destroy enemy.
                enemy.destroy();

                // Destroy energy blast.
                projectile.destroy();

                // Award points.
                score += 100;

                break;
            }
        }
    }
}

// ============================================================
// GOAL COLLISION
// ============================================================

void Game::handleGoalCollision()
{
    auto intersection =
        player
        .getBounds()
        .findIntersection(
            goal
            .getGlobalBounds()
        );

    if (intersection)
    {
        gameState =
            GameState::LevelComplete;
    }
}

// ============================================================
// RESET PLAYER POSITION
// ============================================================

void Game::resetPlayer()
{
    player.setPosition(
        100.0f,
        300.0f
    );

    player.setOnGround(
        false
    );

    camera.setCenter({
        WINDOW_WIDTH /
        2.0f,

        WINDOW_HEIGHT /
        2.0f
        });

    // Remove old shots when
    // player respawns.
    projectiles.clear();
}

// ============================================================
// RESTART ENTIRE GAME
// ============================================================

void Game::restartGame()
{
    lives = 3;

    score = 0;

    currentLevel = 1;

    gameState =
        GameState::Playing;

    // Remove all existing projectiles.
    projectiles.clear();

    resetPlayer();

    // ========================================================
    // REBUILD ENEMIES
    // ========================================================

    enemies.clear();

    enemies.emplace_back(
        750.0f,
        WINDOW_HEIGHT -
        GROUND_HEIGHT -
        40.0f
    );

    enemies.emplace_back(
        1450.0f,
        WINDOW_HEIGHT -
        GROUND_HEIGHT -
        40.0f
    );

    enemies.emplace_back(
        2200.0f,
        WINDOW_HEIGHT -
        GROUND_HEIGHT -
        40.0f
    );

    enemies.emplace_back(
        3100.0f,
        WINDOW_HEIGHT -
        GROUND_HEIGHT -
        40.0f
    );

    // ========================================================
    // REBUILD POWER UPS
    // ========================================================

    powerUps.clear();

    powerUps.emplace_back(
        580.0f,
        390.0f,
        PowerUpType::Invincibility
    );

    powerUps.emplace_back(
        920.0f,
        310.0f,
        PowerUpType::Speed
    );

    powerUps.emplace_back(
        1280.0f,
        230.0f,
        PowerUpType::SuperJump
    );

    powerUps.emplace_back(
        1700.0f,
        350.0f,
        PowerUpType::ExtraLife
    );
}

// ============================================================
// START NEXT LEVEL
// ============================================================

void Game::startNextLevel()
{
    currentLevel++;

    gameState =
        GameState::Playing;

    // Remove projectiles from previous level.
    projectiles.clear();

    resetPlayer();

    // ========================================================
    // RESET ENEMIES
    // ========================================================

    enemies.clear();

    enemies.emplace_back(
        750.0f,
        WINDOW_HEIGHT -
        GROUND_HEIGHT -
        40.0f
    );

    enemies.emplace_back(
        1450.0f,
        WINDOW_HEIGHT -
        GROUND_HEIGHT -
        40.0f
    );

    enemies.emplace_back(
        2200.0f,
        WINDOW_HEIGHT -
        GROUND_HEIGHT -
        40.0f
    );

    enemies.emplace_back(
        3100.0f,
        WINDOW_HEIGHT -
        GROUND_HEIGHT -
        40.0f
    );

    // ========================================================
    // RESET POWER UPS
    // ========================================================

    powerUps.clear();

    powerUps.emplace_back(
        580.0f,
        390.0f,
        PowerUpType::Invincibility
    );

    powerUps.emplace_back(
        920.0f,
        310.0f,
        PowerUpType::Speed
    );

    powerUps.emplace_back(
        1280.0f,
        230.0f,
        PowerUpType::SuperJump
    );

    powerUps.emplace_back(
        1700.0f,
        350.0f,
        PowerUpType::ExtraLife
    );
}

// ============================================================
// CAMERA
// ============================================================

void Game::updateCamera()
{
    float halfWindow =
        WINDOW_WIDTH /
        2.0f;

    float playerCenter =
        player
        .getPosition()
        .x +
        PLAYER_WIDTH /
        2.0f;

    float cameraX =
        playerCenter;

    if (
        cameraX <
        halfWindow)
    {
        cameraX =
            halfWindow;
    }

    if (
        cameraX >
        WORLD_WIDTH -
        halfWindow)
    {
        cameraX =
            WORLD_WIDTH -
            halfWindow;
    }

    camera.setCenter({
        cameraX,
        WINDOW_HEIGHT /
        2.0f
        });
}

// ============================================================
// SCORE FORMAT
// ============================================================

std::string
Game::formatScore() const
{
    std::ostringstream stream;

    stream
        << std::setw(6)
        << std::setfill('0')
        << score;

    return stream.str();
}

// ============================================================
// HUD
// ============================================================

void Game::renderHUD()
{
    // ========================================================
    // LIVES
    // ========================================================

    sf::Text livesText(
        font,

        "LIVES: " +
        std::to_string(lives),

        24
    );

    livesText.setFillColor(
        sf::Color::White
    );

    livesText.setPosition({
        30.0f,
        20.0f
        });

    window.draw(
        livesText
    );

    // ========================================================
    // SCORE
    // ========================================================

    sf::Text scoreText(
        font,

        "SCORE: " +
        formatScore(),

        24
    );

    scoreText.setFillColor(
        sf::Color::White
    );

    scoreText.setPosition({
        400.0f,
        20.0f
        });

    window.draw(
        scoreText
    );

    // ========================================================
    // LEVEL
    // ========================================================

    sf::Text levelText(
        font,

        "LEVEL: " +
        std::to_string(
            currentLevel),

        24
    );

    levelText.setFillColor(
        sf::Color::White
    );

    levelText.setPosition({
        820.0f,
        20.0f
        });

    window.draw(
        levelText
    );

    // ========================================================
    // ACTIVE POWER UPS
    // ========================================================

    float powerTextY =
        60.0f;

    // Bendera / Invincible
    if (player.isInvincible())
    {
        sf::Text text(
            font,
            "BENDERA",
            20
        );

        text.setFillColor(
            sf::Color(
                255,
                100,
                220
            )
        );

        text.setPosition({
            420.0f,
            powerTextY
            });

        window.draw(text);

        powerTextY +=
            28.0f;
    }

    // Speed boost
    if (player.hasSpeedBoost())
    {
        sf::Text text(
            font,
            "SPEED BOOST",
            20
        );

        text.setFillColor(
            sf::Color(
                80,
                150,
                255
            )
        );

        text.setPosition({
            420.0f,
            powerTextY
            });

        window.draw(text);

        powerTextY +=
            28.0f;
    }

    // Super Jump
    if (player.hasSuperJump())
    {
        sf::Text text(
            font,
            "SUPER JUMP",
            20
        );

        text.setFillColor(
            sf::Color(
                50,
                255,
                100
            )
        );

        text.setPosition({
            420.0f,
            powerTextY
            });

        window.draw(text);
    }
}

// ============================================================
// GAME OVER SCREEN
// ============================================================

void Game::renderGameOver()
{
    sf::Text gameOverText(
        font,
        "GAME OVER",
        70
    );

    gameOverText.setFillColor(
        sf::Color::Red
    );

    gameOverText.setPosition({
        310.0f,
        180.0f
        });

    window.draw(
        gameOverText
    );

    sf::Text scoreText(
        font,

        "FINAL SCORE: " +
        formatScore(),

        30
    );

    scoreText.setFillColor(
        sf::Color::White
    );

    scoreText.setPosition({
        360.0f,
        300.0f
        });

    window.draw(
        scoreText
    );

    sf::Text restartText(
        font,
        "Press ENTER to Restart",
        26
    );

    restartText.setFillColor(
        sf::Color::White
    );

    restartText.setPosition({
        350.0f,
        380.0f
        });

    window.draw(
        restartText
    );

    sf::Text quitText(
        font,
        "Press ESC to Quit",
        22
    );

    quitText.setFillColor(
        sf::Color(
            180,
            180,
            180
        )
    );

    quitText.setPosition({
        390.0f,
        430.0f
        });

    window.draw(
        quitText
    );
}

// ============================================================
// LEVEL COMPLETE SCREEN
// ============================================================

void Game::renderLevelComplete()
{
    sf::Text completeText(
        font,
        "LEVEL COMPLETE!",
        60
    );

    completeText.setFillColor(
        sf::Color::Yellow
    );

    completeText.setPosition({
        250.0f,
        180.0f
        });

    window.draw(
        completeText
    );

    sf::Text scoreText(
        font,

        "SCORE: " +
        formatScore(),

        30
    );

    scoreText.setFillColor(
        sf::Color::White
    );

    scoreText.setPosition({
        390.0f,
        300.0f
        });

    window.draw(
        scoreText
    );

    sf::Text continueText(
        font,
        "Press ENTER for Next Level",
        26
    );

    continueText.setFillColor(
        sf::Color::White
    );

    continueText.setPosition({
        330.0f,
        380.0f
        });

    window.draw(
        continueText
    );
}

// ============================================================
// RENDER
// ============================================================

void Game::render()
{
    window.clear(
        sf::Color(
            100,
            180,
            255
        )
    );

    // ========================================================
    // PLAYING
    // ========================================================

    if (
        gameState ==
        GameState::Playing)
    {
        // World camera
        window.setView(
            camera
        );

        // Ground
        window.draw(
            ground
        );

        // Platforms
        for (
            Platform& platform :
            platforms)
        {
            platform.draw(
                window
            );
        }

        // Power-ups
        for (
            PowerUp& powerUp :
            powerUps)
        {
            powerUp.draw(
                window
            );
        }

        // Enemies
        for (
            Enemy& enemy :
            enemies)
        {
            enemy.draw(
                window
            );
        }

        // ====================================================
        // ENERGY PROJECTILES
        // ====================================================

        for (
            Projectile& projectile :
            projectiles)
        {
            projectile.draw(
                window
            );
        }

        // Goal
        window.draw(
            goal
        );

        // Player
        player.draw(
            window
        );

        // ====================================================
        // HUD
        // ====================================================

        window.setView(
            window.getDefaultView()
        );

        renderHUD();
    }

    // ========================================================
    // GAME OVER
    // ========================================================

    else if (
        gameState ==
        GameState::GameOver)
    {
        window.setView(
            window.getDefaultView()
        );

        renderGameOver();
    }

    // ========================================================
    // LEVEL COMPLETE
    // ========================================================

    else if (
        gameState ==
        GameState::LevelComplete)
    {
        window.setView(
            window.getDefaultView()
        );

        renderLevelComplete();
    }

    window.display();
}