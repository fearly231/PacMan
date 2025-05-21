#include "PinkGhost.h"

#include <iostream>
#include <map>
#include <queue>
#include <algorithm>

PinkGhost::PinkGhost(int startX, int startY, sf::Texture& normalTexture, sf::Texture& fearedTexture)
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

void PinkGhost::update(Board& board, float deltaTime, const sf::Vector2i& pacmanPos,  sf::Vector2i& pacmanDir) {
    sf::Vector2f targetPixel(gridPos.x * Board::TILE_SIZE + Board::TILE_SIZE / 2,
        gridPos.y * Board::TILE_SIZE + Board::TILE_SIZE / 2);

    if (std::abs(pixelPos.x - targetPixel.x) < 2 && std::abs(pixelPos.y - targetPixel.y) < 2) {
        pixelPos = targetPixel;

        auto path = getPathTo(board, gridPos, pacmanPos, pacmanDir);
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
        std::cout << "Zjad³eœ PinkGhosta" << std::endl;
        eated.openFromFile("assets/music/pacman_eatghost.wav");
        eated.play();
        
        gridPos.x = Board::WIDTH / 2 - 1;
        gridPos.y = Board::HEIGHT / 2 - 1;

        pixelPos = sf::Vector2f(gridPos.x * Board::TILE_SIZE + Board::TILE_SIZE / 2,
            gridPos.y * Board::TILE_SIZE + Board::TILE_SIZE / 2);
        comingBack.openFromFile("assets/music/eatedghost.ogg");
        comingBack.setVolume(30);
        comingBack.play();
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

std::vector<sf::Vector2i> PinkGhost::getPathTo(Board& board, sf::Vector2i start, sf::Vector2i pacmanPos, sf::Vector2i pacmanDir) {
    std::vector<sf::Vector2i> directions = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    };

    // Tryb "FEARED" – wybieramy ruch najdalej od Pacmana
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

        // ZnajdŸ pole najbardziej oddalone od Pacmana
        auto farthest = *std::max_element(options.begin(), options.end(),
            [&pacmanPos](const sf::Vector2i& a, const sf::Vector2i& b) {
                int da = std::abs(a.x - pacmanPos.x) + std::abs(a.y - pacmanPos.y);
                int db = std::abs(b.x - pacmanPos.x) + std::abs(b.y - pacmanPos.y);
                return da < db;
            });

        return { farthest };
    }

    // Tryb "CHASE" – Pinky chce przewidzieæ ruch Pac-Mana i pod¹¿aæ do przodu
    speed = 80.f;
    sprite = sf::Sprite(normalTexture);
    sprite.setTextureRect(sf::IntRect({ 0, 0 }, { frameSize, frameHeight }));
    sprite.setOrigin(sf::Vector2f(static_cast<float>(frameSize) / 2.f, static_cast<float>(frameHeight) / 2.f));
    sprite.setScale(sf::Vector2f(1.5f, 1.5f));
    sprite.setPosition(pixelPos);

    // W przypadku Pinky'ego, cel nie jest dok³adnie pozycj¹ Pac-Man, ale to, gdzie Pac-Man ma siê udaæ.
    // Pinky stara siê wyprzedziæ Pac-Mana o 4 kratki w kierunku, w którym Pac-Man siê porusza.
    sf::Vector2i predictedPos = pacmanPos + pacmanDir * 4;



    // Sprawdzenie granic planszy
    if (predictedPos.x < 0 || predictedPos.y < 0 ||
        predictedPos.x >= board.WIDTH || predictedPos.y >= board.HEIGHT ||
        board.isWall(predictedPos.x, predictedPos.y)) {
        predictedPos = pacmanPos;  // fallback do pacmanPos
    }
    std::queue<sf::Vector2i> frontier;
    std::map<sf::Vector2i, sf::Vector2i, Vec2iLess> cameFrom;

    frontier.push(start);
    cameFrom[start] = start;

    int bfsSteps = 0;
    const int maxSteps = 1000;
    while (!frontier.empty() && bfsSteps++ < maxSteps) {
        sf::Vector2i current = frontier.front();
        frontier.pop();

        if (current == predictedPos) break;

        for (const auto& dir : directions) {
            sf::Vector2i next = current + dir;

            if (!board.isWall(next.x, next.y) && cameFrom.find(next) == cameFrom.end()) {
                frontier.push(next);
                cameFrom[next] = current;
            }
        }
    }

    std::vector<sf::Vector2i> path;
    if (cameFrom.find(predictedPos) == cameFrom.end()) return path;

    for (sf::Vector2i current = predictedPos; current != start; current = cameFrom[current]) {
        path.push_back(current);
    }

    std::reverse(path.begin(), path.end());
    return path;
}

void PinkGhost::draw(sf::RenderWindow& window) {
    sprite.setPosition(pixelPos);
    window.draw(sprite);
}

bool PinkGhost::isFeared(Board& board) {
    return board.isCherryActive();
}