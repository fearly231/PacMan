#include "BlueGhost.h"
#include <iostream>
#include <map>
#include <queue>
#include <algorithm>
#include <random>

BlueGhost::BlueGhost(int startX, int startY, sf::Texture& normalTexture, sf::Texture& fearedTexture)
    : Ghost(startX, startY, normalTexture), normalTexture(normalTexture), fearedTexture(fearedTexture) {
    gridPos = { startX, startY };
    currentDir = { 0, 0 };
    nextDir = { 0, 0 };
    pixelPos = sf::Vector2f(gridPos.x * Board::TILE_SIZE + Board::TILE_SIZE / 2, gridPos.y * Board::TILE_SIZE + Board::TILE_SIZE / 2);
    sprite = sf::Sprite(normalTexture);
    sprite.setTextureRect(sf::IntRect({ 0, 0 }, { frameSize, frameHeight }));
    sprite.setOrigin(sf::Vector2f(static_cast<float>(frameSize) / 2.f, static_cast<float>(frameHeight) / 2.f));
    sprite.setScale(sf::Vector2f(1.5f, 1.5f));
    sprite.setPosition(pixelPos);
}

void BlueGhost::update(Board& board, float deltaTime, const sf::Vector2i& pacmanPos, const sf::Vector2i& pacmanDir, const sf::Vector2i& blinkyPos) {
    sf::Vector2f targetPixel(gridPos.x * Board::TILE_SIZE + Board::TILE_SIZE / 2,
        gridPos.y * Board::TILE_SIZE + Board::TILE_SIZE / 2);

    if (std::abs(pixelPos.x - targetPixel.x) < 2 && std::abs(pixelPos.y - targetPixel.y) < 2) {
        pixelPos = targetPixel;

        auto path = getPathTo(board, gridPos, pacmanPos, pacmanDir, blinkyPos);
        if (!path.empty()) {
            sf::Vector2i nextStep = path[0];
            currentDir = nextStep - gridPos;
            gridPos = nextStep;
        }

        if (pacmanPos == gridPos && !isFeared(board)) {
            std::cout << "KONIEC GRY, PRZEGRA£EŒ!" << std::endl;
        }
    }

    if (pacmanPos == gridPos && isFeared(board)) {
        std::cout << "Zjad³eœ BlueGhosta" << std::endl;
        gridPos.x = Board::WIDTH / 2 - 1;
        gridPos.y = Board::HEIGHT / 2 - 1;

        pixelPos = sf::Vector2f(gridPos.x * Board::TILE_SIZE + Board::TILE_SIZE / 2,
            gridPos.y * Board::TILE_SIZE + Board::TILE_SIZE / 2);

        sprite.setPosition(pixelPos);
        currentDir = { 0, 0 };
    }

    sf::Vector2f movement(currentDir.x * speed * deltaTime, currentDir.y * speed * deltaTime);
    pixelPos += movement;
    sprite.setPosition(pixelPos);

    if (gridPos.x <= 0) {
        gridPos.x = Board::WIDTH - 2;
        pixelPos.x = gridPos.x * Board::TILE_SIZE + 2 + Board::TILE_SIZE / 2;
    }
    else if (gridPos.x >= Board::WIDTH - 1) {
        gridPos.x = 1;
        pixelPos.x = gridPos.x * Board::TILE_SIZE + 2 + Board::TILE_SIZE / 2;
    }
}

std::vector<sf::Vector2i> BlueGhost::getPathTo(Board& board, sf::Vector2i start, sf::Vector2i pacmanPos, sf::Vector2i pacmanDir, sf::Vector2i blinkyPos) {
    std::vector<sf::Vector2i> directions = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    };

    // Tryb "FEARED" – Inky porusza siê najdalej od Pacmana
    if (isFeared(board)) {
        speed = 50.f;
        sprite = sf::Sprite(fearedTexture);
        sprite.setTextureRect(sf::IntRect({ 0, 0 }, { frameSize, frameHeight }));
        sprite.setOrigin(sf::Vector2f(static_cast<float>(frameSize) / 2.f, static_cast<float>(frameHeight) / 2.f));
        sprite.setScale(sf::Vector2f(1.5f, 1.5f));
        sprite.setPosition(pixelPos);
        // Ucieka w losowym kierunku
        std::vector<sf::Vector2i> options;
        for (const auto& dir : directions) {
            sf::Vector2i next = start + dir;
            if (!board.isWall(next.x, next.y)) {
                options.push_back(next);
            }
        }

        if (options.empty()) return {};

        // Losowy wybór kierunku z dostêpnych opcji
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, options.size() - 1);
        sf::Vector2i randomDirection = options[dis(gen)];

        return { randomDirection };
    }

    // Tryb "CHASE" – Inky pod¹¿a za Pac-Manem w kierunku okreœlonym przez Blinky'ego
    speed = 80.f;
    sprite = sf::Sprite(normalTexture);
    sprite.setTextureRect(sf::IntRect({ 0, 0 }, { frameSize, frameHeight }));
    sprite.setOrigin(sf::Vector2f(static_cast<float>(frameSize) / 2.f, static_cast<float>(frameHeight) / 2.f));
    sprite.setScale(sf::Vector2f(1.5f, 1.5f));
    sprite.setPosition(pixelPos);

    // Oblicz cel Inky'ego w zale¿noœci od pozycji Pac-Mana i Blinky'ego
    sf::Vector2i targetPos = pacmanPos + pacmanDir * 4; // Dwa pola przed Pac-Manem w kierunku, w którym siê porusza

    // Teraz robimy BFS, by znaleŸæ œcie¿kê do tego punktu
    std::queue<sf::Vector2i> frontier;
    std::map<sf::Vector2i, sf::Vector2i, Vec2iLess> cameFrom;

    frontier.push(start);
    cameFrom[start] = start;

    while (!frontier.empty()) {
        sf::Vector2i current = frontier.front();
        frontier.pop();

        if (current == targetPos) break;

        for (const auto& dir : directions) {
            sf::Vector2i next = current + dir;

            if (!board.isWall(next.x, next.y) && cameFrom.find(next) == cameFrom.end()) {
                frontier.push(next);
                cameFrom[next] = current;
            }
        }
    }

    std::vector<sf::Vector2i> path;
    if (cameFrom.find(targetPos) == cameFrom.end()) return path;

    for (sf::Vector2i current = targetPos; current != start; current = cameFrom[current]) {
        path.push_back(current);
    }

    std::reverse(path.begin(), path.end());
    return path;
}

void BlueGhost::draw(sf::RenderWindow& window) {
    sprite.setPosition(pixelPos);
    window.draw(sprite);
}

bool BlueGhost::isFeared(Board& board) {
    return board.isCherryActive();
}