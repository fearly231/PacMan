#include "OrangeGhost.h"
#include <iostream>
#include <map>
#include <queue>
#include <algorithm>
#include <random>


OrangeGhost::OrangeGhost(int startX, int startY, sf::Texture& normalTexture, sf::Texture& fearedTexture)
    : Ghost(startX, startY, normalTexture), normalTexture(normalTexture), fearedTexture(fearedTexture) {
    gridPos = { startX, startY };
    currentDir = { 0, 0 };
    nextDir = { 0, 0 };
    pixelPos = sf::Vector2f(gridPos.x * Board::TILE_SIZE + Board::TILE_SIZE / 2, gridPos.y * Board::TILE_SIZE + Board::TILE_SIZE / 2);
    // std::cout << "PixelPos: " << pixelPos.x << ", " << pixelPos.y << std::endl;
    sprite = sf::Sprite(normalTexture);
    //sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect({ 0, 0 }, { frameSize, frameHeight }));
    sprite.setOrigin(sf::Vector2f(static_cast<float>(frameSize) / 2.f, static_cast<float>(frameSize) / 2.f));
    sprite.setScale(sf::Vector2f(1.5f, 1.5f));
    sprite.setPosition(pixelPos);

}
void OrangeGhost::update(Board& board, float deltaTime, const sf::Vector2i& pacmanPos, const sf::Vector2i& pacmanDir) {
    sf::Vector2f targetPixel(gridPos.x * Board::TILE_SIZE + Board::TILE_SIZE / 2,
        gridPos.y * Board::TILE_SIZE + Board::TILE_SIZE / 2);
    if (std::abs(pixelPos.x - targetPixel.x) < 2 && std::abs(pixelPos.y - targetPixel.y) < 2) {
        pixelPos = targetPixel;

        auto path = getPathTo(board, gridPos, pacmanPos);
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
        //std::cout << "Zjad³eœ OrangeGhosta" << std::endl;
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

    // Zabezpieczenie przed wyjœciem poza planszê
    if (gridPos.x <= 0) {
        gridPos.x = Board::WIDTH - 2;
        pixelPos.x = gridPos.x * Board::TILE_SIZE + 2 + Board::TILE_SIZE / 2;
    }
    else if (gridPos.x >= Board::WIDTH - 1) {
        gridPos.x = 1;
        pixelPos.x = gridPos.x * Board::TILE_SIZE + 2 + Board::TILE_SIZE / 2;
    }
}
std::vector<sf::Vector2i> OrangeGhost::getPathTo(Board& board, sf::Vector2i start, sf::Vector2i pacmanPos) {
    std::vector<sf::Vector2i> directions = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    };

    // Tryb "FEARED" 
    if (isFeared(board)) {
        speed = 50.f;
        sprite = sf::Sprite(fearedTexture);
        sprite.setTextureRect(sf::IntRect({ 0, 0 }, { frameSize, frameHeight }));
        sprite.setOrigin(sf::Vector2f(static_cast<float>(frameSize) / 2.f, static_cast<float>(frameHeight) / 2.f));
        sprite.setScale(sf::Vector2f(1.5f, 1.5f));
        sprite.setPosition(pixelPos);

        std::vector<sf::Vector2i> options;
        for (const auto& dir : directions) {
            sf::Vector2i next = start + dir;
            if (!board.isWall(next.x, next.y)) {
                options.push_back(next);
            }
        }

        if (options.empty()) return {};
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, options.size() - 1);
        sf::Vector2i randomDirection = options[dis(gen)];

        return { randomDirection };
    }

    // Tryb "CHASE" – Clyde zmienia swoj¹ strategiê w zale¿noœci od odleg³oœci do Pacmana
    speed = 80.f;
    sprite = sf::Sprite(normalTexture);
    sprite.setTextureRect(sf::IntRect({ 0, 0 }, { frameSize, frameHeight }));
    sprite.setOrigin(sf::Vector2f(static_cast<float>(frameSize) / 2.f, static_cast<float>(frameHeight) / 2.f));
    sprite.setScale(sf::Vector2f(1.5f, 1.5f));
    sprite.setPosition(pixelPos);

    std::queue<sf::Vector2i> frontier;
    std::map<sf::Vector2i, sf::Vector2i, Vec2iLess> cameFrom;

    frontier.push(start);
    cameFrom[start] = start;

    // Zasada "CHASE" dla Clyda: Gdy jest w pobli¿u Pacmana (na odleg³oœæ 8 kroków), zmienia strategiê
    if (std::abs(start.x - pacmanPos.x) + std::abs(start.y - pacmanPos.y) > 8) {
        // CHASE – BFS do Pacmana
        frontier.push(start);
        cameFrom[start] = start;

        int bfsSteps = 0;
        const int maxSteps = 1000;
        while (!frontier.empty() && bfsSteps++ < maxSteps) {
            sf::Vector2i current = frontier.front();
            frontier.pop();

            if (current == pacmanPos) break;

            for (const auto& dir : directions) {
                sf::Vector2i next = current + dir;

                if (!board.isWall(next.x, next.y) && cameFrom.find(next) == cameFrom.end()) {
                    frontier.push(next);
                    cameFrom[next] = current;
                }
            }
        }

        // Œcie¿ka do Pacmana
        std::vector<sf::Vector2i> path;
        if (cameFrom.find(pacmanPos) == cameFrom.end()) return path;

        for (sf::Vector2i current = pacmanPos; current != start; current = cameFrom[current]) {
            path.push_back(current);
        }

        std::reverse(path.begin(), path.end());
        return path;
    }
    else {
        // SCATTER – uciekaj do lewego dolnego rogu planszy
        sf::Vector2i cornerGoal(1, board.HEIGHT - 2);

        // BFS do rogu
        frontier.push(start);
        cameFrom[start] = start;

        while (!frontier.empty()) {
            sf::Vector2i current = frontier.front();
            frontier.pop();

            if (current == cornerGoal) break;

            for (const auto& dir : directions) {
                sf::Vector2i next = current + dir;

                if (!board.isWall(next.x, next.y) && cameFrom.find(next) == cameFrom.end()) {
                    frontier.push(next);
                    cameFrom[next] = current;
                }
            }
        }

        // Œcie¿ka do rogu
        std::vector<sf::Vector2i> path;
        if (cameFrom.find(cornerGoal) == cameFrom.end()) return path;

        for (sf::Vector2i current = cornerGoal; current != start; current = cameFrom[current]) {
            path.push_back(current);
        }

        std::reverse(path.begin(), path.end());
        return path;
    }

}
void OrangeGhost::draw(sf::RenderWindow& window) {
    //window.draw(sprite);
    sprite.setPosition(pixelPos);
    //std::cout << "RedGhost position: " << sprite.getPosition().x << ", " << sprite.getPosition().y << std::endl;

    window.draw(sprite);


}
bool OrangeGhost::isFeared(Board& board) {
    return board.isCherryActive();
}