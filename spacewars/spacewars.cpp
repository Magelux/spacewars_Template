#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <conio.h> // För att läsa tangenttryckningar
#include <thread>  // För std::this_thread
#include <chrono>  // För std::chrono

// Position och storlek
struct AABB {
    int x, y; // Övre vänstra hörnet
    int width, height;

    // Metod för AABB-kollisionsberäkning
    bool intersects(const AABB& other) const {
        return x < other.x + other.width &&
               x + width > other.x &&
               y < other.y + other.height &&
               y + height > other.y;
    }
};

class Ship {
public:
    AABB box;

    Ship(int startX, int startY) {
        box = {startX, startY, 1, 1};
    }

    void moveUp() {
        if (box.y > 0) box.y--;
    }

    void moveDown(int gridHeight) {
        if (box.y < gridHeight - 1) box.y++;
    }
};

class Asteroid {
public:
    AABB box;

    Asteroid(int startX, int startY) {
        box = {startX, startY, 1, 1};
    }

    void moveLeft() {
        box.x--;
    }
};

class Game {
private:
    Ship player;
    std::vector<Asteroid> asteroids;
    bool isRunning;
    int gridWidth, gridHeight;

public:
    Game(int width, int height)
        : player(2, height / 2), gridWidth(width), gridHeight(height), isRunning(true) {}

    void spawnAsteroid() {
        int startY = rand() % gridHeight;
        asteroids.emplace_back(gridWidth - 1, startY);
    }

    void update() {
        // Flytta asteroider åt vänster
        for (auto& asteroid : asteroids) {
            asteroid.moveLeft();
        }

        // Ta bort asteroider som lämnat spelplanen
        asteroids.erase(
            std::remove_if(asteroids.begin(), asteroids.end(),
                           [this](const Asteroid& a) { return a.box.x < 0; }),
            asteroids.end());
    }

    void render() {
        // Rensa konsolen
        system("cls");

        // Rita spelplanen
        for (int y = 0; y < gridHeight; y++) {
            for (int x = 0; x < gridWidth; x++) {
                if (player.box.x == x && player.box.y == y) {
                    std::cout << "^"; // Rymdskeppet
                } else {
                    bool isAsteroid = false;
                    for (const auto& asteroid : asteroids) {
                        if (asteroid.box.x == x && asteroid.box.y == y) {
                            std::cout << "§"; // Asteroid
                            isAsteroid = true;
                            break;
                        }
                    }
                    if (!isAsteroid) {
                        std::cout << "."; // Tomt utrymme
                    }
                }
            }
            std::cout << "\n";
        }
    }

    void handleInput() {
        if (_kbhit()) {
            char input = _getch();
            if (input == 'w' || input == 'W') {
                player.moveUp();
            } else if (input == 's' || input == 'S') {
                player.moveDown(gridHeight);
            }
        }
    }

    void checkCollisions() {
        for (const auto& asteroid : asteroids) {
            if (player.box.intersects(asteroid.box)) {
                isRunning = false;
                break;
            }
        }
    }

    void sleep() {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    void run() {
        while (isRunning) {
            handleInput();
            update();
            checkCollisions();
            render();
            sleep();

            // Slumpmässigt spawna asteroider
            if (rand() % 10 < 2) {
                spawnAsteroid();
                spawnAsteroid();
                spawnAsteroid();
                //spawnAsteroid();
                //spawnAsteroid();
                //spawnAsteroid();
                //spawnAsteroid();
               
            }
        }
        std::cout << "\n\nGame Over!\n";
    }
};

int main() {
    srand(static_cast<unsigned>(time(0)));

    Game game(30, 10);
    game.run();

    return 0;
}