# Bendera

**Bendera** is a C++/SFML side-scrolling action game inspired by classic arcade beat-'em-ups and platform action games.

The player controls **Shiavon**, explores a scrolling combat stage, fights enemies, collects power-ups, and can transform into **Bendera** to use a ranged energy attack.

## Play the game

A Windows build is produced automatically by GitHub Actions.

1. Open the **Actions** tab.
2. Select the latest successful **Build Bendera for Windows** run.
3. Download the **Bendera-Windows-x64** artifact.
4. Extract the ZIP.
5. Run `Bendera.exe`.

The package includes the game assets and font needed at runtime. You do **not** need to install SFML separately.

> Windows SmartScreen may warn about an unsigned indie executable. The build is produced directly from the source in this repository.

## Controls

| Key | Action |
| --- | --- |
| Arrow keys / movement keys implemented in game | Move |
| Space | Jump |
| X | Bendera energy attack (while transformed) |
| Enter | Restart / continue after game-state screens |
| Esc | Quit |

## Gameplay systems

- Side-scrolling camera
- Player movement and jumping
- Enemy behavior and collisions
- Platforms and world collision
- Score and lives
- Power-ups
- Bendera transformation
- Projectile combat
- Game-over and level-complete states
- Sprite and visual asset pipeline

## Technology

- C++
- C++20
- SFML 3
- CMake
- Visual Studio / MSVC
- GitHub Actions

## Build locally

### Requirements

- Windows 10/11
- Visual Studio 2022 with **Desktop development with C++**
- CMake 3.24+
- Git

### Build

```powershell
git clone https://github.com/Jmatt3441/Bendera.git
cd Bendera
cmake -S . -B build -A x64
cmake --build build --config Release
```

The first CMake configure downloads SFML automatically.

The finished executable will be under the Release build output, with `assets` and `arial.ttf` copied beside it automatically.

## Project structure

```text
Bendera/
├── CMakeLists.txt
├── Side Scroller.sln
├── Side Scroller/
│   ├── Game.cpp / Game.h
│   ├── Player.cpp / Player.h
│   ├── Enemy.cpp / Enemy.h
│   ├── Projectile.cpp / Projectile.h
│   ├── PowerUp.cpp / PowerUp.h
│   ├── Platform.cpp / Platform.h
│   ├── assets/
│   └── main.cpp
└── .github/workflows/build-windows.yml
```

## Status

Bendera is under active development. The repository contains the game source, assets, and automated Windows build pipeline.

## Author

**James Matthews**  
GitHub: [@Jmatt3441](https://github.com/Jmatt3441)
